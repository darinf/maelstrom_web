#include <emscripten.h>

#include "SDL_event.h"
#include "SDL_timer.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "myerror.h"

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

static void BuildEvent(int type, int code, int x, int y, SDL_Event* event) {
  event->type = (Uint32) type;
  event->key.keysym.sym = (SDLKey) code;
  event->key.keysym.mod = 0;
  event->key.keysym.unicode = ToUnicode(code);
  if (event->type == SDL_KEYDOWN) {
    event->key.state = SDL_PRESSED;
  } else if (event->type == SDL_KEYUP) {
    event->key.state = SDL_RELEASED;
  } else if (event->type == SDL_MOUSEBUTTONDOWN) {
    event->button.button = 1;
    event->button.x = x;
    event->button.y = y;
  }
}

static void ReadEvent(SDL_Event* event) {
  int type = EM_ASM_INT({
    return worker_get_event_type();
  });
  int code = EM_ASM_INT({
    return worker_get_event_code();
  });
  int x = EM_ASM_INT({
    return worker_get_event_x();
  });
  int y = EM_ASM_INT({
    return worker_get_event_y();
  });
  BuildEvent(type, code, x, y, event);
}

int SDL_PollEvent(SDL_Event* event) { 
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
