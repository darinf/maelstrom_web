#include "SDL_surface.h"

#include <stdlib.h>
#include <stdio.h>
#include "SDL_rwops.h"

namespace {

//----

struct BMPFileHeader { 
  Uint16 type; 
  Uint32 size; 
  Uint32 reserved; 
  Uint32 bitmap_offset;
  Uint32 header_size; 
  Sint32 width; 
  Sint32 height; 
  Uint16 planes; 
  Uint16 bits_per_pixel; 
  Uint32 compression; 
  Uint32 bitmap_size;
  Sint32 horizontal_resolution;
  Sint32 vertical_resolution;
  Uint32 num_colors; 
  Uint32 num_important_colors; 
};

void ReadBitmapHeader(SDL_RWops* ops, BMPFileHeader* header) {
  header->type                  = SDL_ReadLE16(ops);
  header->size                  = SDL_ReadLE32(ops);
  header->reserved              = SDL_ReadLE32(ops);
  header->bitmap_offset         = SDL_ReadLE32(ops);
  header->header_size           = SDL_ReadLE32(ops);
  header->width                 = SDL_ReadLE32(ops);
  header->height                = SDL_ReadLE32(ops);
  header->planes                = SDL_ReadLE16(ops);
  header->bits_per_pixel        = SDL_ReadLE16(ops); 
  header->compression           = SDL_ReadLE32(ops);
  header->bitmap_size           = SDL_ReadLE32(ops);
  header->horizontal_resolution = SDL_ReadLE32(ops);
  header->vertical_resolution   = SDL_ReadLE32(ops);
  header->num_colors            = SDL_ReadLE32(ops);
  header->num_important_colors  = SDL_ReadLE32(ops);
}

//----

struct Impl : SDL_Surface {
  Impl();
};

Impl::Impl() {
  refcount = 1;
  format = NULL;
  w = 0;
  h = 0;
  pitch = 0;
  pixels = NULL;
  flags = 0;
}

}  // namespace

SDL_Surface* SDL_SetVideoMode(int width, int height, int depth, int video_flags) { return 0; }
SDL_Surface* SDL_GetVideoSurface() { return 0; }
SDL_Surface* SDL_CreateRGBSurface(int video_flags, int width, int height, int depth, int a, int b, int c, int d) { return 0; }

void SDL_FreeSurface(SDL_Surface* surface) {
  if (!surface)
    return;

#if 0
  Impl* impl = static_cast<Impl*>(surface);
  if (--impl->refcount == 0)
    delete impl;
#endif
}

void SDL_BlitSurface(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dest, SDL_Rect* dst_rect) {}
void SDL_LowerBlit(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dest, SDL_Rect* dst_rect) {}
int SDL_LockSurface(SDL_Surface*) { return 0; }
void SDL_UnlockSurface(SDL_Surface*) {}
void SDL_UpdateRects(SDL_Surface*, int num_rects, SDL_Rect* rects) {}

SDL_Surface* SDL_LoadBMP(const char* path) {
  fprintf(stderr, "SDL_LoadBMP\n");
#if 0
  SDL_RWops* ops = SDL_RWFromFile(path, "rb");
  if (!ops)
    return NULL;
  BMPFileHeader header;
  ReadBitmapHeader(ops, &header);
  SDL_RWclose(ops);

  fprintf(stderr, "bmp_header.width = %d\n", header.width);
  fprintf(stderr, "bmp_header.width = %d\n", header.height);

  fflush(stderr);

  Impl* impl = new Impl();
  return impl;
#endif
  return NULL;
}

int SDL_SaveBMP(SDL_Surface*, const char* path) { return 0; }

void SDL_SetGammaRamp(Uint16*, Uint16*, Uint16*) {}

void SDL_SetColorKey(SDL_Surface*, int flags, Uint8) {}
void SDL_SetColors(SDL_Surface*, SDL_Color* colors, int offset, int count) {}
Uint32 SDL_MapRGB(SDL_PixelFormat*, Uint8 r, Uint8 g, Uint8 b) { return 0; }
