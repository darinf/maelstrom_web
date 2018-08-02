importScripts("pipe2.js");

console.log("AudioContext=" + this.AudioContext);

var eventPipeReader;
var renderPipeWriter;
var currentEvent;

var Module = {
  preRun: [
    function() {
      // Ensure resources are downloaded.
      addRunDependency("filesystem");

      FS.mkdir("/Maelstrom");
      FS.mount(IDBFS, {}, "/Maelstrom");

      FS.syncfs(true, function (err) {
        var filesRemaining = 0;

        function doneWithFile() {
          if (--filesRemaining == 0) {
            FS.syncfs(false, function (err) {
              removeRunDependency("filesystem");
            });
          }
        }

        try {
          FS.mkdir("/Maelstrom/Images");
        } catch (e) {
        }

        [
          "Maelstrom_Fonts",
          "Maelstrom_Sounds",
          "Maelstrom_Sprites",
          "Maelstrom-Scores",
          "icon.bmp",
          "Images/Maelstrom_Icon#100.cicn",
          "Images/Maelstrom_Icon#101.cicn",
          "Images/Maelstrom_Icon#102.cicn",
          "Images/Maelstrom_Icon#103.cicn",
          "Images/Maelstrom_Icon#104.cicn",
          "Images/Maelstrom_Icon#110.cicn",
          "Images/Maelstrom_Icon#128.cicn",
          "Images/Maelstrom_Icon#129.cicn",
          "Images/Maelstrom_Icon#130.cicn",
          "Images/Maelstrom_Icon#131.cicn",
          "Images/Maelstrom_Icon#132.cicn",
          "Images/Maelstrom_Icon#133.cicn",
          "Images/Maelstrom_Icon#134.cicn",
          "Images/Maelstrom_Icon#135.cicn",
          "Images/Maelstrom_Icon#136.cicn",
          "Images/Maelstrom_Icon#137.cicn",
          "Images/Maelstrom_Titles#100.bmp",
          "Images/Maelstrom_Titles#101.bmp",
          "Images/Maelstrom_Titles#102.bmp",
          "Images/Maelstrom_Titles#128.bmp",
          "Images/Maelstrom_Titles#129.bmp",
          "Images/Maelstrom_Titles#130.bmp",
          "Images/Maelstrom_Titles#133.bmp",
          "Images/Maelstrom_Titles#134.bmp",
          "Images/Maelstrom_Titles#135.bmp",
          "Images/Maelstrom_Titles#999.bmp"
        ].forEach(function(filename) {
          var filepath = "/Maelstrom/" + filename;
          var exists;
          try {
            exists = (FS.lookupPath(filepath).node != null);
          } catch (e) {
            exists = false;
          }
          if (!exists) {
            filesRemaining++;
            fetch(filename.replace('#', '%23'), {credentials: "include"}).then(function(response) {
              response.arrayBuffer().then(function(buffer) {
                FS.writeFile(filepath, new Uint8Array(buffer), {encoding: "binary"});
                doneWithFile();
              });
            });
          }
        });
        if (filesRemaining == 0)
          removeRunDependency("filesystem");
      });
    }
  ],
  postRun: [],
  print: function(text) {
    if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
    console.log(text);
  },
  printErr: function(text) {
    if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
    console.error(text);
  },
  setStatus: function(text) {
    if (!Module.setStatus.last) Module.setStatus.last = { time: Date.now(), text: '' };
    if (text === Module.setStatus.text) return;
    console.log("status: " + text);
  },
  totalDependencies: 0,
  monitorRunDependencies: function(left) {
    this.totalDependencies = Math.max(this.totalDependencies, left);
    Module.setStatus(left ? 'Preparing... (' + (this.totalDependencies-left) + '/' + this.totalDependencies + ')' : 'All downloads complete.');
  }
};

function createBuffer(size) {
  var data = new Uint8ClampedArray(size);
  return data.buffer;
}

function worker_draw(pixels, x, y, width, height) {
  // TODO: is there a way to avoid this extra copy?

  var uint32 = new Uint32Array(4 + width * height);
  uint32[0] = x;
  uint32[1] = y;
  uint32[2] = width;
  uint32[3] = height;
  uint32.set(new Uint32Array(pixels.buffer, pixels.byteOffset, width * height), 4);

  renderPipeWriter.write(new Int8Array(uint32.buffer));

  postMessage({command: "do_draw", params:[]});
}

function worker_play_sound(samples) {
  //console.log("worker_play_sounds: N=" + samples.byteLength);
  postMessage({command: "do_sound", params:[samples.buffer, samples.byteLength]}, [samples.buffer]);
}

function worker_set_volume(volume) {
  postMessage({command: "do_set_volume", params:[volume]});
}

function setCurrentEvent(int8) {
  var json = new TextDecoder('utf-8').decode(int8);
  var data = JSON.parse(json);

  var type;
  switch (data[0]) {
    case "keydown":
      type = 1;  // SDL_KEYDOWN
      break;
    case "keyup":
      type = 2;  // SDL_KEYUP
      break;
    case "mousedown":
      type = 3;  // SDL_MOUSEBUTTONDOWN
      break;
    default:
      throw "oops: unknown event type [" + data[0] + "]";
  }

  currentEvent = {type: type, code: data[1], x: data[2], y: data[3]};
}

function worker_get_event_type() {
  return currentEvent.type;
}

function worker_get_event_code() {
  return currentEvent.code;
}

function worker_get_event_x() {
  return currentEvent.x;
}

function worker_get_event_y() {
  return currentEvent.y;
}

function worker_poll_event() {
  var int8 = eventPipeReader.tryRead();
  if (!int8) {
    currentEvent = null;
    return 0;
  }
  setCurrentEvent(int8);
  return 1;
}

function worker_wait_event() {
  var int8 = eventPipeReader.read();  // Blocks until we have some data
  setCurrentEvent(int8);
}

function start(eventPipeSAB, renderPipeSAB) {
  var eventPipe = new PipeBuffer();
  eventPipe.initializeFromSAB(eventPipeSAB);
  eventPipeReader = new MessagePipeReader(eventPipe);

  var renderPipe = new PipeBuffer();
  renderPipe.initializeFromSAB(renderPipeSAB);
  renderPipeWriter = new MessagePipeWriter(renderPipe);

  fetch("maelstrom.wasm", {credentials: "include"}).then(function(response) {
    response.arrayBuffer().then(function(buffer) {
      Module["wasmBinary"] = buffer;
      importScripts("maelstrom.js");
    });
  });
}

onmessage = function(e) {
  switch (e.data.command) {
    case "start":
      start(e.data.params[0], e.data.params[1]);
      break;
  }
}
