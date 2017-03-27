mergeInto(LibraryManager.library, {
  js_open_file: function(path_ptr, mode_ptr) {
    var path = Pointer_stringify(path_ptr);
    var mode = Pointer_stringify(mode_ptr);
    console.log("js_open_file(\"" + path + "\", \"" + mode + "\")");
    return -1;
  },
  js_close_file: function(fd) {
    console.log("js_close_file(" + fd + ")");
  }
});
