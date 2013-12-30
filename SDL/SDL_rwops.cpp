#include "SDL_rwops.h"

#include <stdio.h>
#include <string.h>
#include <ppapi/c/pp_completion_callback.h>
#include <ppapi/c/pp_errors.h>

#include "SDL/SDL_endian.h"
#include "myerror.h"
#include "ppb.h"

#define NOIMPL() \
  error("Function not implemented: %s\n", __FUNCTION__)

extern PP_Instance g_instance;

namespace {

struct Impl : SDL_RWops {
  Impl() : file_io_(0), offset_(0) {
  }
  ~Impl() {
    if (file_io_)
      ppb.core->ReleaseResource(file_io_);
  }

  bool Open(PP_Resource file_ref, const char* mode);
  Sint32 Read(void* buf, Uint32 size, Uint32 count);
  Sint32 Write(const void* buf, Uint32 size, Uint32 count);

  PP_Resource file_io_;
  int offset_;
};

bool Impl::Open(PP_Resource file_ref, const char* mode) {
  int32_t open_flags;
  if (!strcmp(mode, "rb") || !strcmp(mode, "r")) {
    open_flags = PP_FILEOPENFLAG_READ;
  } else if (!strcmp(mode, "r+")) {
    open_flags = PP_FILEOPENFLAG_READ |
                 PP_FILEOPENFLAG_WRITE;
  } else if (!strcmp(mode, "wb") || !strcmp(mode, "w")) {
    open_flags = PP_FILEOPENFLAG_WRITE |
                 PP_FILEOPENFLAG_CREATE |
                 PP_FILEOPENFLAG_TRUNCATE;
  } else {
    error("Open(mode=%s): not implemented", mode);
    return false;
  }

  file_io_ = ppb.file_io->Create(g_instance);
  if (!file_io_)
    return false;

  int32_t rv = ppb.file_io->Open(
      file_io_, file_ref, open_flags, PP_BlockUntilComplete());
  if (rv != PP_OK) {
    ppb.core->ReleaseResource(file_io_);
    file_io_ = 0;
    return false;
  }

  return true;
}

Sint32 Impl::Read(void* buf, Uint32 size, Uint32 count) {
#if 0
  // Read as much as possible in one go.
  int32_t rv, buf_offset = 0, buf_size = size * count;
  do {
    fprintf(stderr, "Read [%p](from=%d, size=%d)\n", this, offset_, buf_size);
    rv = ppb.file_io->Read(file_io_,
                           offset_,
                           static_cast<char*>(buf) + buf_offset,
                           buf_size,
                           PP_BlockUntilComplete());
    if (rv < 0) {
      fprintf(stderr, "PPB_FileIO::Read failed: %d\n", rv);
      return -1;
    }
    offset_ += rv;
    buf_offset += rv;
    buf_size -= rv;
  } while (rv > 0 && buf_size > 0);

  return rv;
#endif

  //fprintf(stderr, "Read [%p](%p,%u,%u)\n", this, buf, size, count);

  int32_t rv = ppb.file_io->Read(file_io_,
                                 offset_,
                                 static_cast<char*>(buf),
                                 size * count,
                                 PP_BlockUntilComplete());
  if (rv < 0) {
    fprintf(stderr, "PPB_FileIO::Read failed: %d\n", rv);
    return -1;
  }

  offset_ += rv;

  if (rv % size)
    fprintf(stderr, "Oops: read more than what is being reported?\n");

  return rv / size;
}

Sint32 Impl::Write(const void* buf, Uint32 size, Uint32 count) {
  //fprintf(stderr, "Read [%p](%p,%u,%u)\n", this, buf, size, count);

  int32_t rv = ppb.file_io->Write(file_io_,
                                  offset_,
                                  static_cast<const char*>(buf),
                                  size * count,
                                  PP_BlockUntilComplete());
  if (rv < 0) {
    fprintf(stderr, "PPB_FileIO::Write failed: %d\n", rv);
    return -1;
  }

  offset_ += rv;

  if (rv % size)
    fprintf(stderr, "Oops: wrote more than what is being reported?\n");

  return rv / size;
}

}  // namespace

//----

const char* SDL_GetError() { return 0; }
void SDL_SetError(const char*) {}

SDL_RWops* SDL_RWFromFile(const char* path, const char* mode) {
  PP_Resource file_system =
      ppb.file_system->Create(g_instance,
                              PP_FILESYSTEMTYPE_LOCALTEMPORARY);
  if (!file_system)
    return NULL;

  Impl* impl = NULL;

  int32_t rv = ppb.file_system->Open(file_system, 0, PP_BlockUntilComplete());
  if (rv == PP_OK) {
    PP_Resource file_ref = ppb.file_ref->Create(file_system, path);
    if (file_ref) {
      impl = new Impl();
      if (!impl->Open(file_ref, mode)) {
        delete impl;
        impl = NULL;
      }
      ppb.core->ReleaseResource(file_ref);
    }
  }

  ppb.core->ReleaseResource(file_system); 
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
  return impl->Read(buf, size, count);
}

Sint32 SDL_RWwrite(SDL_RWops* ops, const void* buf, Uint32 size, Uint32 count) {
  Impl* impl = static_cast<Impl*>(ops);
  return impl->Write(buf, size, count);
}

Uint32 SDL_WriteBE32(SDL_RWops*, Uint32) { NOIMPL(); return 0; }
Uint32 SDL_WriteLE32(SDL_RWops*, Uint32) { NOIMPL(); return 0; }

Uint32 SDL_RWseek(SDL_RWops* ops, Sint32 offset, int whence) {
  //fprintf(stderr, "SDL_RWseek [%p:%d:%d]\n", ops, offset, whence);
  switch (whence) {
    case SEEK_SET:
      static_cast<Impl*>(ops)->offset_ = offset;
      break;
    default:
      fprintf(stderr, "SDL_RWseek whence (%d) not supported!\n", whence);
  }
  return static_cast<Impl*>(ops)->offset_;
}

Uint32 SDL_RWtell(SDL_RWops* ops) {
  return static_cast<Impl*>(ops)->offset_;
}
