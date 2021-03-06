mergeInto(LibraryManager.library, {
  jslib_canvas_init: function(width, height) {
    //console.log("jslib_canvas_init(" + width + ", " + height + ")");
  },

  jslib_canvas_draw: function(pixelsPtr, x, y, width, height) {
    //console.log("jslib_canvas_draw(" + x + ", " + y + ", " + width + ", " + height + ")");

    var numBytes = width * height * 4;
    var mappedPixels = new Uint8Array(Module.HEAPU8.buffer, pixelsPtr, numBytes);

    worker_draw(mappedPixels, x, y, width, height);
  },
});
