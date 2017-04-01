const kViewportWidth = 640;
const kViewportHeight = 480;

var Module = {
  preRun: [
    /*
    function() {
      try {
        FS.mkdir('/Maelstrom');
        FS.mount(IDBFS, {}, '/Maelstrom');
        addRunDependency("maelstrom-fs-setup");
        FS.syncfs(true, function(err) {
          removeRunDependency("maelstrom-fs-setup");
        });
      } catch (e) {
      }
    },
    */
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
