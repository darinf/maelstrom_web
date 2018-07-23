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

    this.eventPipe_ = new PipeBuffer();
    this.eventPipe_.initialize(4096);  // XXX
    this.eventPipeWriter_ = new PipeWriter(this.eventPipe_);

    this.renderPipe_ = new PipeBuffer();
    this.renderPipe_.initialize(  // TODO: find a better way to do this
        (4 * 4) +          // x, y, width, height
        (640 * 480 * 4));  // max canvas size
    this.renderPipeReader_ = new PipeReader(this.renderPipe_);

    this.worker_ = new Worker("worker.js");
    this.worker_.onmessage = this.onHandleMessage_.bind(this);

    //window.addEventListener("mousedown", this.onHandleInputEvent_.bind(this), false);
    window.addEventListener("keydown", this.onHandleInputEvent_.bind(this), false);
    window.addEventListener("keyup", this.onHandleInputEvent_.bind(this), false);
  }

  start() {
    this.worker_.postMessage({command: "start", params: [this.eventPipe_.sab, this.renderPipe_.sab]});
  }

  // Commands from the worker:

  do_draw() {
    //console.log("do_draw");

    this.updateDrawList_();

    if (this.drawList_.length > 0) {
      this.animator_.schedule();
    } else {
      console.log("Warning: nothing to draw in do_draw().");
    }
  }

  // Private methods:

  onHandleMessage_(e) {
    this[e.data.command].apply(this, e.data.params);
  }

  onHandleInputEvent_(e) {
    //console.log(e.type + ": " + e.keyCode);
    var params = [e.type];
    if (e.type == "keydown" || e.type == "keyup") {
      params.push(e.keyCode);
    }
    //XXX this.worker_.postMessage({command: "input", params: params}, []);

    var str = JSON.stringify(params);
    this.eventPipeWriter_.write(new TextEncoder().encode(str));

    if (this.eventPipeWriter_.hasPendingWrites()) {
      console.log("!!! deferring input event");
      setTimeout(this.onFlushPendingEvents_.bind(this), 0);
    }

    e.preventDefault();
  }

  onFlushPendingEvents_() {
    this.eventPipeWriter_.doPendingWrites();
  }

  onDraw_() {
    //XXX this.eventPipeWriter_.doPendingWrites();

    this.updateDrawList_();
    while (this.drawList_.length > 0)
      this.drawImage_.apply(this, this.drawList_.shift());
  }

  drawImage_(imageData, x, y, width, height) {
    if (!suppressDraw) {
      //XXX var imageData = new ImageData(new Uint8ClampedArray(buffer), width, height);
      this.renderingContext_.putImageData(imageData, x, y, 0, 0, width, height);
    }

    //XXX this.worker_.postMessage({command: "reclaim", params: [buffer]}, [buffer]);
  }

  updateDrawList_() {
    for (;;) {
      var int8 = this.renderPipeReader_.tryRead();
      if (!int8)
        return;
      var uint32 = new Uint32Array(int8.buffer);

      var x = uint32[0];
      var y = uint32[1];
      var width = uint32[2];
      var height = uint32[3];

      var imageData = new ImageData(new Uint8ClampedArray(int8.buffer, 4 * 4), width, height);

      this.drawList_.push([imageData, x, y, width, height]);
    }
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
