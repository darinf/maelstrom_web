#include <emscripten.h>

#include "SDL_event.h"
#include "SDL_timer.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

//XXX #include <ppapi/c/ppb_input_event.h>
#include "myerror.h"
//XXX #include "ppb.h"
//XXX #include "pp_resource_queue.h"

#if 0
struct LinkedEvent {
  LinkedEvent* next;
  SDL_Event data;
};

static LinkedEvent* next_event = NULL;
#endif

static int ToUnicode(uint32_t key_code) {
  // NOTE: We should really handle PP_INPUTEVENT_TYPE_CHAR instead,
  // but this hacky approach is good enough for a subset of ASCII.

  if (key_code >= SDLK_0 && key_code <= SDLK_9)
    return (int) key_code;

  if (key_code >= SDLK_a && key_code <= SDLK_z) {
  /*
    uint32_t modifiers = ppb.input_event->GetModifiers(input_event);
    if (modifiers & PP_INPUTEVENT_MODIFIER_SHIFTKEY)
      return (int) key_code;
   */

    return (int) key_code + 32;
  }

  if (key_code == SDLK_BACKSPACE || key_code == SDLK_DELETE)
    return '\b';

  if (key_code == SDLK_RETURN)
    return '\r';

  return 0;
}

static void BuildEvent(int type, int key_code, SDL_Event* event) {
  event->type = (Uint32) type;
  event->key.keysym.sym = (SDLKey) key_code;
  event->key.keysym.mod = 0;
  event->key.keysym.unicode = ToUnicode(key_code);
  if (event->type == SDL_KEYDOWN) {
    event->key.state = SDL_PRESSED;
  } else if (event->type == SDL_KEYUP) {
    event->key.state = SDL_RELEASED;
  }
}

static void ReadEvent(SDL_Event* event) {
  int type = EM_ASM_INT({
    return worker_get_event_type();
  });
  int code = EM_ASM_INT({
    return worker_get_event_code();
  });
  mesg("ReadEvent: type=%d code=%d\n", type, code);
  BuildEvent(type, code, event);
}

#if 0
extern "C" void EMSCRIPTEN_KEEPALIVE Maelstrom_OnInputEvent(int type, int key_code) {
  LinkedEvent* event = new LinkedEvent();
  event->next = NULL;
  event->data.type = (Uint32) type;
  event->data.key.keysym.sym = (SDLKey) key_code;
  event->data.key.keysym.mod = 0;
  event->data.key.keysym.unicode = ToUnicode(key_code);
  if (event->data.type == SDL_KEYDOWN) {
    event->data.key.state = SDL_PRESSED;
  } else if (event->data.type == SDL_KEYUP) {
    event->data.key.state = SDL_RELEASED;
  }

  // Append to linked list
  if (next_event) {
    // Find last, and make this new event the last.
    LinkedEvent* last = next_event;
    while (last->next)
      last = last->next;
    last->next = event;
  } else {
    next_event = event;
  }
}
#endif

#if 0
extern PPResourceQueue g_input_queue;

static bool TranslateEvent(PP_Resource input_event, SDL_Event* result) {
  bool translated = true;

  PP_InputEvent_Type input_event_type = ppb.input_event->GetType(input_event);

  switch (input_event_type) {
    case PP_INPUTEVENT_TYPE_MOUSEDOWN: {
      PP_Point position = ppb.mouse_input_event->GetPosition(input_event);
      //mesg("mousedown [x=%u, y=%u]", position.x, position.y);
      result->type = SDL_MOUSEBUTTONDOWN;
      result->button.button = 1;
      result->button.x = position.x;
      result->button.y = position.y;
      break;
    }
    case PP_INPUTEVENT_TYPE_KEYDOWN: {
      uint32_t key_code = ppb.keyboard_input_event->GetKeyCode(input_event);
      //mesg("keydown [key_code=%u]", key_code);
      result->type = SDL_KEYDOWN;
      result->key.state = SDL_PRESSED;
      result->key.keysym.sym = key_code;
      result->key.keysym.mod = 0;
      result->key.keysym.unicode = ToUnicode(key_code, input_event);
      break;
    }
    case PP_INPUTEVENT_TYPE_KEYUP: {
      uint32_t key_code = ppb.keyboard_input_event->GetKeyCode(input_event);
      //mesg("keyup [key_code=%u]", key_code);
      result->type = SDL_KEYUP;
      result->key.state = SDL_RELEASED;
      result->key.keysym.sym = key_code;
      result->key.keysym.mod = 0;
      result->key.keysym.unicode = ToUnicode(key_code, input_event);
      break;
    }
    default:
      //mesg("unknown input event");
      translated = false;
      break;
  }
  return translated;
}
#endif

int SDL_PollEvent(SDL_Event* event) { 
/*
  if (!event)
    return next_event ? 1 : 0;

  if (!next_event)
    return 0;

  LinkedEvent* temp = next_event;
  next_event = temp->next;

  memcpy(event, &temp->data, sizeof(SDL_Event));
  delete temp;

  return 1;
*/

  int has_event = EM_ASM_INT({
    return worker_poll_event();
  });
  if (!has_event)
    return 0;

  ReadEvent(event);
  return 1;
}

int SDL_WaitEvent(SDL_Event* event) {
  if (SDL_PollEvent(event))
    return 1;

  // This blocks until SDL_PollEvent would succeed.
  EM_ASM({
    worker_wait_event();
  });

  ReadEvent(event);
  return 1;
}

const char* SDL_GetKeyName(SDLKey key) {
  switch (key) {
    case SDLK_BACKSPACE:
      return "backspace";
    case SDLK_TAB:
      return "tab";
    case SDLK_RETURN:
      return "return";
    case SDLK_PAUSE:
      return "pause";
    case SDLK_ESCAPE:
      return "escape";
    case SDLK_SPACE:
      return "space";
    case SDLK_LEFT:
      return "left";
    case SDLK_UP:
      return "up";
    case SDLK_RIGHT:
      return "right";
    case SDLK_DELETE:
      return "delete";
    case SDLK_0:
      return "0";
    case SDLK_1:
      return "1";
    case SDLK_2:
      return "2";
    case SDLK_3:
      return "3";
    case SDLK_4:
      return "4";
    case SDLK_5:
      return "5";
    case SDLK_6:
      return "6";
    case SDLK_7:
      return "7";
    case SDLK_8:
      return "8";
    case SDLK_9:
      return "9";
    case SDLK_a:
      return "a";
    case SDLK_c:
      return "c";
    case SDLK_l:
      return "l";
    case SDLK_p:
      return "p";
    case SDLK_q:
      return "q";
    case SDLK_x:
      return "x";
    case SDLK_z:
      return "z";
    case SDLK_F1:
      return "F1";
    case SDLK_F3:
      return "F3";
  }
  return "###";
}

void SDL_EnableUNICODE(int) {}
