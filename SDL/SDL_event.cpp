#include "SDL_event.h"

#include <stdio.h>
#include <unistd.h>

#include <ppapi_main/ppapi_event.h>
#include <ppapi_main/ppapi_main.h>

static Uint32 PPAPIKeyCodeToSDLKeySym(Uint32 key_code) {
  switch (key_code) {
    case 9:
      return SDLK_TAB;
    case 32:
      return SDLK_SPACE;
    case 37:
      return SDLK_LEFT;
    case 38:
      return SDLK_UP;
    case 39:
      return SDLK_RIGHT;
    case 80:
      return SDLK_p;
  }

  fprintf(stderr, "unknown key code (%u)!\n", key_code);
}

int SDL_PollEvent(SDL_Event* event) { 
  PPAPIEvent* ppapi_event = PPAPI_AcquireEvent();
  if (!ppapi_event)
    return 0;

  fprintf(stderr, "got input event [type=%u]!\n", ppapi_event->event_type);

  int result = 1;
  switch (ppapi_event->event_type) {
    case PP_INPUTEVENT_TYPE_MOUSEDOWN: {
      PPAPIMouseEvent* mouse_event = reinterpret_cast<PPAPIMouseEvent*>(ppapi_event);
      event->type = SDL_MOUSEBUTTONDOWN;
      event->button.button = 1;
      event->button.x = mouse_event->location.x;
      event->button.y = mouse_event->location.y;
      break;
    }
    case PP_INPUTEVENT_TYPE_KEYDOWN: {
      PPAPIKeyEvent* key_event = reinterpret_cast<PPAPIKeyEvent*>(ppapi_event);
      fprintf(stderr, "  key_code=%u (mod=%u)\n", key_event->key_code, ppapi_event->modifiers);
      event->type = SDL_KEYDOWN;
      event->key.state = SDL_PRESSED;
      event->key.keysym.sym = PPAPIKeyCodeToSDLKeySym(key_event->key_code);
      event->key.keysym.mod = 0;
      event->key.keysym.unicode = 0;
      break;
    }
    case PP_INPUTEVENT_TYPE_KEYUP: {
      PPAPIKeyEvent* key_event = reinterpret_cast<PPAPIKeyEvent*>(ppapi_event);
      fprintf(stderr, "  key_code=%u (mod=%u)\n", key_event->key_code, ppapi_event->modifiers);
      event->type = SDL_KEYUP;
      event->key.state = SDL_RELEASED;
      event->key.keysym.sym = PPAPIKeyCodeToSDLKeySym(key_event->key_code);
      event->key.keysym.mod = 0;
      event->key.keysym.unicode = 0;
      break;
    }
    default:
      result = 0;
      break;
  }

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
