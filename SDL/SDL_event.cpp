#include "SDL_event.h"

#include <stdio.h>
#include <unistd.h>

#include <ppapi/c/ppb_input_event.h>
#include "myerror.h"
#include "ppb.h"
#include "pp_resource_queue.h"

extern PPResourceQueue g_input_queue;

static Uint32 PPAPIKeyCodeToSDLKeySym(Uint32 key_code) {
  switch (key_code) {
    case 9:
      return SDLK_TAB;
    case 27:
      return SDLK_ESCAPE;
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

  error("unknown key code (%u)!", key_code);
  return 0;
}

static bool TranslateEvent(PP_Resource input_event, SDL_Event* result) {
  bool translated = true;

  PP_InputEvent_Type input_event_type = ppb.input_event->GetType(input_event);

  switch (input_event_type) {
    case PP_INPUTEVENT_TYPE_MOUSEDOWN: {
      PP_Point position = ppb.mouse_input_event->GetPosition(input_event);
      mesg("mousedown [x=%u, y=%u]", position.x, position.y);
      result->type = SDL_MOUSEBUTTONDOWN;
      result->button.button = 1;
      result->button.x = position.x;
      result->button.y = position.y;
      break;
    }
    case PP_INPUTEVENT_TYPE_KEYDOWN: {
      uint32_t key_code = ppb.keyboard_input_event->GetKeyCode(input_event);
      mesg("keydown [key_code=%u]", key_code);
      result->type = SDL_KEYDOWN;
      result->key.state = SDL_PRESSED;
      result->key.keysym.sym = PPAPIKeyCodeToSDLKeySym(key_code);
      result->key.keysym.mod = 0;
      result->key.keysym.unicode = 0;
      break;
    }
    case PP_INPUTEVENT_TYPE_KEYUP: {
      uint32_t key_code = ppb.keyboard_input_event->GetKeyCode(input_event);
      mesg("keyup [key_code=%u]", key_code);
      result->type = SDL_KEYUP;
      result->key.state = SDL_RELEASED;
      result->key.keysym.sym = PPAPIKeyCodeToSDLKeySym(key_code);
      result->key.keysym.mod = 0;
      result->key.keysym.unicode = 0;
      break;
    }
    default:
      mesg("unknown input event");
      translated = false;
      break;
  }
  return translated;
}

int SDL_PollEvent(SDL_Event* event) { 
  //mesg("SDL_PollEvent");

  for (;;) {
    PP_Resource input_resource = g_input_queue.GetOrFail();
    if (!input_resource)
      return 0;

    bool translated = TranslateEvent(input_resource, event);

    ppb.core->ReleaseResource(input_resource);

    if (translated)
      break;
  }
  return 1;
}

int SDL_WaitEvent(SDL_Event* event) {
  //mesg("SDL_WaitEvent");

  for (;;) {
    PP_Resource input_resource = g_input_queue.Get();
    if (!input_resource)
      return 0;

    bool translated = TranslateEvent(input_resource, event);

    ppb.core->ReleaseResource(input_resource);

    if (translated)
      break;
  }
  return 1;
}

const char* SDL_GetKeyName(SDLKey) { return 0; }
void SDL_EnableUNICODE(int) {}
