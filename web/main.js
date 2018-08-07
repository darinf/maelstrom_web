var suppressDraw = false;

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
    this.audioContext_ = null;
    this.volume_ = 1.0;

    this.eventPipe_ = new PipeBuffer();
    this.eventPipe_.initialize(4096);
    this.eventPipeWriter_ = new MessagePipeWriter(this.eventPipe_);

    this.renderPipe_ = new PipeBuffer();
    this.renderPipe_.initialize(
        (4 * 4) +          // x, y, width, height
        (640 * 480 * 4) +  // max canvas size
        (256));            // extra buffer for message framing
    this.renderPipeReader_ = new MessagePipeReader(this.renderPipe_);

    this.worker_ = new Worker("worker.js");
    this.worker_.onmessage = this.onHandleMessage_.bind(this);

    window.addEventListener("mousedown", this.onHandleInputEvent_.bind(this), false);
    window.addEventListener("keydown", this.onHandleInputEvent_.bind(this), false);
    window.addEventListener("keyup", this.onHandleInputEvent_.bind(this), false);
  }

  start() {
    this.worker_.postMessage({command: "start", params: [this.eventPipe_.sab, this.renderPipe_.sab]});
  }

  // Commands from the worker:

  do_draw() {
    this.updateDrawList_();

    if (this.drawList_.length > 0)
      this.animator_.schedule();
  }

  do_sound(bytes, byteOffset, byteLength) {
    //console.log("do_sound: N=" + byteLength);

    var input = new Uint8Array(bytes, byteOffset, byteLength);

    if (!this.audioContext_)
      this.audioContext_ = new window.AudioContext;

    var buffer = this.audioContext_.createBuffer(1, byteLength, 11025);
    var data = buffer.getChannelData(0);
    var volume = this.volume_;
    for (var i = 0; i < byteLength; ++i)
      data[i] = volume * (input[i] - 128) / 128.0;

    var src = this.audioContext_.createBufferSource();
    src.buffer = buffer;
    src.connect(this.audioContext_.destination);
    src.start();
  }
  
  do_set_volume(volume) {
    // volume is a float between 0.0 and 1.0
    this.volume_ = volume;
  }

  // Private methods:

  onHandleMessage_(e) {
    this[e.data.command].apply(this, e.data.params);
  }

  onHandleInputEvent_(e) {
    var params = [e.type];
    if (e.type == "keydown" || e.type == "keyup") {
      params.push(e.keyCode);
      params.push(0);
      params.push(0);
    } else if (e.type == "mousedown") {
      params.push(0);

      var canvas = document.getElementById("canvas");
      params.push(e.x - canvas.offsetLeft);
      params.push(e.y - canvas.offsetTop);
    }

    var str = JSON.stringify(params);

    // The pipe buffer should be large enough for this to never fail ;-)
    if (!this.eventPipeWriter_.tryWrite(new TextEncoder().encode(str)))
      console.log("!!! dropping input event");  // Increase buffer size?

    e.preventDefault();
  }

  onDraw_() {
    this.updateDrawList_();
    while (this.drawList_.length > 0)
      this.drawImage_.apply(this, this.drawList_.shift());
  }

  drawImage_(imageData, x, y, width, height) {
    if (!suppressDraw)
      this.renderingContext_.putImageData(imageData, x, y, 0, 0, width, height);
  }

  updateDrawList_() {
    for (;;) {
      var int8 = this.renderPipeReader_.tryRead();
      if (!int8)
        return;
      var uint32 = new Uint32Array(int8.buffer, int8.byteOffset, 4);

      var x = uint32[0];
      var y = uint32[1];
      var width = uint32[2];
      var height = uint32[3];

      var imageData = new ImageData(
          new Uint8ClampedArray(int8.buffer,
                                int8.byteOffset + 16,
                                width * height * 4),
          width, height);

      this.drawList_.push([imageData, x, y, width, height]);
    }
  }
}

var worker;
var canvasController;

function start() {
  if (!this.SharedArrayBuffer) {
    document.body.innerText = "ERROR: SharedArrayBuffer support required!";
    return;
  }

  var canvas = document.getElementById("canvas");
  var context = canvas.getContext("2d");

  canvasController = new CanvasController(context);
  canvasController.start();
}
