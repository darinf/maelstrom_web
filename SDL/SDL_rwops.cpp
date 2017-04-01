#include "SDL_rwops.h"

#include <emscripten.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "SDL/SDL_endian.h"
#include "myerror.h"

#define NOIMPL() \
  error("Function not implemented: %s\n", __FUNCTION__)

static int g_called_sync_fs = 0;

void CallSyncFS() {
  if (++g_called_sync_fs > 1)
    return;
  EM_ASM(
    FS.syncfs(false, function(err) {
      ccall('DidSyncFS');
    });
  );
}

extern "C" void EMSCRIPTEN_KEEPALIVE DidSyncFS() {
  bool call_again = false;
  if (g_called_sync_fs > 1)
    call_again = true;
  g_called_sync_fs = 0;
  if (call_again)
    CallSyncFS();
}

namespace {

struct Impl : SDL_RWops {
  Impl() : fp(NULL), opened_for_writing(false) {
  }

  ~Impl() {
    if (fp) {
      fclose(fp);
      if (opened_for_writing)
        CallSyncFS();
    }
  }

  FILE* fp;
  bool opened_for_writing;
};

}  // namespace

//----

const char* SDL_GetError() { return 0; }
void SDL_SetError(const char*) {}

SDL_RWops* SDL_RWFromFile(const char* path, const char* mode) {
  mesg("SDL_RWFromFile(\"%s\", \"%s\")\n", path, mode);

  Impl* impl = new Impl();
  impl->fp = fopen(path, mode);
  if (!impl->fp) {
    delete impl;
    return NULL;
  }

  impl->opened_for_writing = (strchr(mode, 'w') != NULL);

  return impl;
}

int SDL_RWclose(SDL_RWops* ops) {
  delete static_cast<Impl*>(ops);
  return 0;
}

Uint16 SDL_ReadBE16(SDL_RWops* ops) {
  Uint16 be;
  if (SDL_RWread(ops, &be, sizeof(be), 1) != 1)
    return 0;

  return SDL_SwapBE16(be);
}

Uint32 SDL_ReadBE32(SDL_RWops* ops) {
  Uint32 be;
  if (SDL_RWread(ops, &be, sizeof(be), 1) != 1)
    return 0;

  return SDL_SwapBE32(be);
}

Uint16 SDL_ReadLE16(SDL_RWops* ops) {
  Uint16 le;
  if (SDL_RWread(ops, &le, sizeof(le), 1) != 1)
    return 0;

  return SDL_SwapLE16(le);
}

Uint32 SDL_ReadLE32(SDL_RWops* ops) {
  Uint32 le;
  if (SDL_RWread(ops, &le, sizeof(le), 1) != 1)
    return 0;

  return SDL_SwapLE32(le);
}

Sint32 SDL_RWread(SDL_RWops* ops, void* buf, Uint32 size, Uint32 count) {
  Impl* impl = static_cast<Impl*>(ops);
  return (Sint32) fread(buf, size, count, impl->fp);
}

Sint32 SDL_RWwrite(SDL_RWops* ops, const void* buf, Uint32 size, Uint32 count) {
  Impl* impl = static_cast<Impl*>(ops);
  return (Sint32) fwrite(buf, size, count, impl->fp);
}

Uint32 SDL_WriteBE32(SDL_RWops* ops, Uint32 n) {
  Uint32 be = SDL_SwapBE32(n);

  if (SDL_RWwrite(ops, &be, sizeof(be), 1) != 1)
    return 0;

  return 1;
}

Uint32 SDL_WriteLE32(SDL_RWops*, Uint32) { NOIMPL(); return 0; }

Uint32 SDL_RWseek(SDL_RWops* ops, Sint32 offset, int whence) {
  Impl* impl = static_cast<Impl*>(ops);
  return fseek(impl->fp, offset, whence);
}

Uint32 SDL_RWtell(SDL_RWops* ops) {
  Impl* impl = static_cast<Impl*>(ops);
  return ftell(impl->fp);
}
