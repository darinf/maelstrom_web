#include "SDL_event.h"

#include <stdio.h>
#include <unistd.h>

#include <ppapi_main/ppapi_event.h>
#include <ppapi_main/ppapi_main.h>

int SDL_PollEvent(SDL_Event* event) { 
  PPAPIEvent* ppapi_event = PPAPI_AcquireEvent();
  if (!ppapi_event)
    return 0;

  fprintf(stderr, "got input event [type=%u]!\n", ppapi_event->event_type);

  int result = 0;
  if (ppapi_event->event_type == PP_INPUTEVENT_TYPE_MOUSEDOWN) {
    PPAPIMouseEvent* mouse_event = reinterpret_cast<PPAPIMouseEvent*>(ppapi_event);
    event->type = SDL_MOUSEBUTTONDOWN;
    event->button.button = 1;
    event->button.x = mouse_event->location.x;
    event->button.y = mouse_event->location.y;
    /*
    switch (mouse_event->button) {
      case PP_INPUTEVENT_MOUSEBUTTON_LEFT:
        event->button = 
    */
    result = 1;
  }

  //XXX
  PPAPI_ReleaseEvent(ppapi_event);
  return result;
}

int SDL_WaitEvent(SDL_Event* event) {
  while (!SDL_PollEvent(event)) {
    fprintf(stderr, "polling for input event...\n");
    usleep(30000);
  }
  return 1;
}

const char* SDL_GetKeyName(SDLKey) { return 0; }
void SDL_EnableUNICODE(int) {}
