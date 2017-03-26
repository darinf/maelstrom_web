var suppressDraw = false;

function log(x) {
  document.getElementById("log").innerText += x + "\n";
}

class Animator {
  constructor(closure) {
    this.closure_ = closure;
    this.requestID_ = undefined;
  }
  schedule() {
    if (this.requestID_)
      return;
    this.requestID_ = window.requestAnimationFrame(this.onAnimate_.bind(this));
  }
  cancel() {
    if (this.requestID_)
      window.cancelAnimationFrame(this.requestID_);
  }
  onAnimate_() {
    this.requestID_ = undefined;
    this.closure_();
  }
}

class CanvasController {
  constructor(renderingContext) {
    this.animator_ = new Animator(this.onDraw_.bind(this));
    this.renderingContext_ = renderingContext;
    this.drawList_ = new Array();

    this.worker_ = new Worker("worker.js");
    this.worker_.onmessage = this.onHandleMessage_.bind(this);
  }

  start() {
    this.worker_.postMessage({command: "start", params: []});
  }

  // Commands from the worker:

  draw(buffer, x, y, width, height) {
    this.drawList_.push([buffer, x, y, width, height]);
    this.animator_.schedule();
  }

  // Private methods:

  onHandleMessage_(e) {
    this[e.data.command].apply(this, e.data.params);
  }

  onDraw_() {
    while (this.drawList_.length > 0)
      this.drawImage_.apply(this, this.drawList_.shift());
  }

  drawImage_(buffer, x, y, width, height) {
    if (!suppressDraw) {
      var imageData = new ImageData(new Uint8ClampedArray(buffer), width, height);
      this.renderingContext_.putImageData(imageData, x, y, 0, 0, width, height);
    }

    this.worker_.postMessage({command: "reclaim", params: [buffer]}, [buffer]);
  }
}

var worker;
var canvasController;

function start() {
  var canvas = document.getElementById("canvas");
  var context = canvas.getContext("2d");

  canvasController = new CanvasController(context);
  canvasController.start();
}
