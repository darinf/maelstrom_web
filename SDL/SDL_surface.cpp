#include "SDL_surface.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "SDL_rwops.h"

#include <ppapi_main/ppapi_main.h>
#include "ppb.h"

namespace {

//----

const int kBMPHeaderSize = 2 + 6*4 + 2*2 + 6*4;

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

struct IndexedImpl : Impl {
  IndexedImpl(int w, int h, Uint32 flags);
  ~IndexedImpl();

  SDL_PixelFormat format_;
  SDL_Palette palette_;
  SDL_Color palette_colors_[256];
  int transparent_color_;
};

IndexedImpl::IndexedImpl(int w, int h, Uint32 flags) {
  this->w = w;
  this->h = h;
  this->pitch = w;
  this->flags = flags;

  pixels = new Uint8[w * h];

  format = &format_;
  format->BitsPerPixel = 8;
  format->BytesPerPixel = 1;
  format->Rshift = 0;
  format->Gshift = 0;
  format->Bshift = 0;
  format->Rmask = 0;
  format->Gmask = 0;
  format->Bmask = 0;
  format->palette = &palette_;
  format->palette->colors = palette_colors_;
  format->palette->ncolors = 256;

  memset(palette_colors_, 0, sizeof(palette_colors_));

  transparent_color_ = -1;
}

IndexedImpl::~IndexedImpl() {
  delete[] (Uint8*) pixels;
}

//----

Uint8* IndexedPixelsAt(SDL_Surface* surface, int x, int y) {
  Uint8* pixels = static_cast<Uint8*>(surface->pixels);
  return pixels + (surface->pitch * y) + x;
}

//----

IndexedImpl* video_surface = NULL;
PP_Resource graphics_2d = 0;

}  // namespace

SDL_Surface* SDL_SetVideoMode(int width, int height, int depth, int video_flags) {
  if (video_surface) {
    fprintf(stderr, "SDL_SetVideoMode: video surface already initialized!\n");
    return NULL;
  }

  if (depth != 8) {
    fprintf(stderr, "SDL_SetVideoMode: only 8 bit color depth is supported!\n");
    return NULL;
  }

  PP_Size size;
  size.width = width;
  size.height = height;
  graphics_2d = ppb.graphics_2d->Create(PPAPI_GetInstanceId(), &size, PP_TRUE);

  ppb.instance->BindGraphics(PPAPI_GetInstanceId(), graphics_2d);

  video_surface = new IndexedImpl(width, height, video_flags);
  return video_surface;
}

SDL_Surface* SDL_GetVideoSurface() {
  return video_surface;
}

SDL_Surface* SDL_CreateRGBSurface(int video_flags, int width, int height, int depth, int a, int b, int c, int d) {
  if (depth != 8) {
    fprintf(stderr, "SDL_CreateRGBSurface [depth=%u]: only 8 bit color depth is supported!\n", depth);
    return NULL;
  }

  return new IndexedImpl(width, height, video_flags);
}

void SDL_FreeSurface(SDL_Surface* surface) {
  if (!surface)
    return;

  Impl* impl = static_cast<Impl*>(surface);
  if (--impl->refcount == 0)
    delete impl;
}

void SDL_BlitSurface(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dst, SDL_Rect* dst_rect) {
  //fprintf(stderr, "SDL_BlitSurface [src=%p, dst=%p]\n", src, dst);
  SDL_LowerBlit(src, src_rect, dst, dst_rect);
}

void SDL_LowerBlit(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dst, SDL_Rect* dst_rect) {
/*
  fprintf(stderr, "SDL_LowerBlit [src=%p, src_rect=(%u,%u,%u,%u), dst=%p, dst_rect=(%u,%u,%u,%u)]\n",
      src,
      src_rect->x,
      src_rect->y,
      src_rect->w,
      src_rect->h,
      dst,
      dst_rect->x,
      dst_rect->y,
      dst_rect->w,
      dst_rect->h);
*/

  if (src_rect->w != dst_rect->w ||
      src_rect->h != dst_rect->h) {
    fprintf(stderr, "SDL_LowerBlit: width and height of src and dst much match!\n");
    return;
  }

  if (src->format->BitsPerPixel != 8 ||
      dst->format->BitsPerPixel != 8) {
    fprintf(stderr, "SDL_LowerBlit: src and dst must both be 8 bit depth!\n");
    return;
  }

  for (int row = 0; row < src_rect->h; ++row) {
    memcpy(IndexedPixelsAt(dst, dst_rect->x, dst_rect->y + row),
           IndexedPixelsAt(src, src_rect->x, src_rect->y + row),
           src_rect->w);
  }
}

int SDL_LockSurface(SDL_Surface* surface) {
  //fprintf(stderr, "SDL_LockSurface [%p]\n", surface);
  return 0;
}

void SDL_UnlockSurface(SDL_Surface* surface) {
  //fprintf(stderr, "SDL_UnlockSurface [%p]\n", surface);
}

void SDL_UpdateRects(SDL_Surface* surface, int num_rects, SDL_Rect* rects) {
/*
  fprintf(stderr, "SDL_UpdateRects [%p num=%u]", surface, num_rects);
  for (int i = 0; i < num_rects; ++i)
    fprintf(stderr, " (%u,%u,%u,%u)", rects[i].x, rects[i].y, rects[i].w, rects[i].h);
  fprintf(stderr, "\n");
*/

  if (surface != video_surface) {
    fprintf(stderr, "SDL_UpdateRects: can only be called on the video surface!\n");
    return;
  }

  PP_ImageDataFormat image_format = ppb.image_data->GetNativeImageDataFormat();

  for (int i = 0; i < num_rects; ++i) {
    PP_Size size;
    size.width = rects[i].w;
    size.height = rects[i].h;

    PP_Resource image_data = ppb.image_data->Create(
        PPAPI_GetInstanceId(),
        image_format,
        &size,
        PP_FALSE);

    PP_ImageDataDesc image_desc;
    ppb.image_data->Describe(image_data, &image_desc);

    void* image_pixels = ppb.image_data->Map(image_data);
    Uint32* image_pixels_end = (Uint32*) (((Uint8*) image_pixels) + image_desc.size.height * image_desc.stride);

    //fprintf(stderr, "about to copy pixels [size=(%u,%u), stride=%u, pixels=%p]\n", image_desc.size.width, image_desc.size.height, image_desc.stride, image_pixels); fflush(stderr);

    // Copy pixels
    for (int row = 0; row < rects[i].h; ++row) {
      Uint8* src_pixels = IndexedPixelsAt(surface, rects[i].x, rects[i].y + row);
      Uint8* dst_pixels = static_cast<Uint8*>(image_pixels) + row * image_desc.stride;

      //fprintf(stderr, "about to write row [%u] of pixels\n", row); fflush(stderr);
      
      for (int col = 0; col < rects[i].w; ++col) {
        Uint8 index = src_pixels[col];
        SDL_Color color = surface->format->palette->colors[index];

        Uint32 packed_color;
        switch (image_format) {
          case PP_IMAGEDATAFORMAT_BGRA_PREMUL:
            packed_color = color.b | (color.g << 8) | (color.r << 16);
            break;
          default:
            fprintf(stderr, "Oops: need code for image format!\n");
            return;
        }

        // XXX
        //fprintf(stderr, "writing dst_pixels [row=%d, col=%d]\n", row, col); fflush(stderr);
        Uint32* dst_ptr = reinterpret_cast<Uint32*>(dst_pixels + (4 * col));
        *dst_ptr = packed_color;
      }
    }

    // Now, flush to display
    PP_Point top_left;
    top_left.x = rects[i].x;
    top_left.y = rects[i].y;
    //fprintf(stderr, "PaintImageData(x=%u,y=%u,w=%u,h=%u)\n", rects[i].x, rects[i].y, rects[i].w, rects[i].h);
    ppb.graphics_2d->PaintImageData(graphics_2d, image_data, &top_left, NULL);

    ppb.image_data->Unmap(image_data);
    ppb.core->ReleaseResource(image_data);
  }

  ppb.graphics_2d->Flush(graphics_2d, PP_BlockUntilComplete());
}

SDL_Surface* SDL_LoadBMP(const char* path) {
  //fprintf(stderr, "SDL_LoadBMP [\"%s\"]\n", path);

  SDL_RWops* ops = SDL_RWFromFile(path, "rb");
  if (!ops)
    return NULL;
  BMPFileHeader header;
  ReadBitmapHeader(ops, &header);

  int palette_size = header.bitmap_offset - kBMPHeaderSize;
  Uint8* palette_data = new Uint8[palette_size];
  SDL_RWread(ops, palette_data, 1, palette_size);

  int data_size = header.size - header.bitmap_offset;
  Uint8* bitmap_data = new Uint8[data_size];
  SDL_RWread(ops, bitmap_data, 1, data_size);

  SDL_RWclose(ops);

  IndexedImpl* impl = NULL;
  do {
    if (header.bits_per_pixel != 8) {
      fprintf(stderr, "SDL_LoadBMP: can only handle 8 bit depth!\n");
      break;
    }
    if (header.compression != 0) {
      fprintf(stderr, "SDL_LoadBMP: can only handle uncompressed data!\n");
      break;
    }

    //fprintf(stderr, "  (num_colors=%u, palette_size=%u)\n", header.num_colors, palette_size);

    impl = new IndexedImpl(header.width, header.height, SDL_SWSURFACE);

    // Copy palette over
    for (int i = 0; i < header.num_colors; ++i) {
      Uint8* color_data = palette_data + i*4;
      impl->format->palette->colors[i].r = color_data[0];
      impl->format->palette->colors[i].g = color_data[1];
      impl->format->palette->colors[i].b = color_data[2];
    }

    // Copy pixels over
    for (int row = 0; row < header.height; ++row) {
      // bitmap_data rows are flipped upside down.
      Uint8* src_row = bitmap_data + (header.height - row - 1) * header.width;
      Uint8* dst_row = static_cast<Uint8*>(impl->pixels) + row * header.width;
      memcpy(dst_row, src_row, header.width);
    }
  } while (false);

  delete[] palette_data;
  delete[] bitmap_data;
  return impl;
}

int SDL_SaveBMP(SDL_Surface* surface, const char* path) {
  fprintf(stderr, "SDL_SaveBMP [%p, \"%s\"]\n", surface, path);
  return 0;
}

void SDL_SetGammaRamp(Uint16*, Uint16*, Uint16*) {
  //fprintf(stderr, "SDL_SetGammaRamp\n");
}

void SDL_SetColorKey(SDL_Surface* surface, Uint32 flags, Uint32 key) {
  if (surface->format->BitsPerPixel != 8) {
    fprintf(stderr, "SDL_SetColorKey: only supported for 8 bit depth!\n");
    return;
  }

  IndexedImpl* impl = static_cast<IndexedImpl*>(surface);

  if (flags == 0) {
    impl->transparent_color_ = -1;
  } else {
    if (key < surface->format->palette->ncolors) {
      impl->transparent_color_ = key;
    } else {
      fprintf(stderr, "SDL_SetColorKey: invalid color index!\n");
    }
  }
}

void SDL_SetColors(SDL_Surface* surface, SDL_Color* colors, int offset, int count) {
  for (int i = 0; i < count; ++i)
    surface->format->palette->colors[i + offset] = colors[i];
}

Uint32 SDL_MapRGB(SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b) {
  if (format->BitsPerPixel != 8) {
    fprintf(stderr, "SDL_MapRGB: only know how to handle 8 bit depth!\n");
    return 0;
  }

  for (int i = 0; i < format->palette->ncolors; ++i) {
    if (format->palette->colors[i].r == r &&
        format->palette->colors[i].g == g &&
        format->palette->colors[i].b == b)
      return i;
  }

  fprintf(stderr, "SDL_MapRGB: didn't find an exact match!\n");

  return 0;
}
