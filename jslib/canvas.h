#ifndef JSLIB_CANVAS_H_
#define JSLIB_CANVAS_H_

extern "C" {

extern void jslib_canvas_init(int width, int height);
extern void jslib_canvas_draw(void* pixels, int x, int y, int width, int height);

}  // extern "C"

#endif  // JSLIB_CANVAS_H_
