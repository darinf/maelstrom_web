const kViewportWidth = 640;
const kViewportHeight = 480;

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

        FS.mkdir("/Maelstrom/Images");

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

function draw(buffer, x, y, width, height) {
  postMessage({command: "draw", params: [buffer, x, y, width, height]}, [buffer]);
}

function reclaimBuffer(buffer) {
}

function start() {
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
      start();
      break;
    case "reclaim":
      reclaimBuffer(e.data.params[0]);
      break;
  }
}
