#include "SDL_surface.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "SDL_rwops.h"

#include "myerror.h"

#include "jslib/canvas.h"

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

void DumpBitmapHeader(BMPFileHeader* header) {
#define DUMP(field) mesg(#field ": %u", (uint32_t) header->field)
  DUMP(type);
  DUMP(size);
  DUMP(reserved);
  DUMP(bitmap_offset);
  DUMP(header_size);
  DUMP(width);
  DUMP(height);
  DUMP(planes);
  DUMP(bits_per_pixel);
  DUMP(compression);
  DUMP(bitmap_size);
  DUMP(horizontal_resolution);
  DUMP(vertical_resolution);
  DUMP(num_colors);
  DUMP(num_important_colors);
#undef DUMP 
}

inline Uint32 PitchFromWidth(Sint32 w) {
  return (w + 3) & ~3;
}

//----

struct Impl : SDL_Surface {
  Impl(int w, int h, int flags);
  ~Impl();

  int color_key_;
};

Impl::Impl(int w, int h, int flags) {
  refcount = 1;
  format = NULL;
  this->w = w;
  this->h = h;
  pitch = 0;
  pixels = NULL;
  this->flags = flags;
  color_key_ = -1;
}

Impl::~Impl() {
  free(pixels);
}

struct Index1Impl : Impl {
  Index1Impl(int w, int h, Uint32 flags);

  SDL_PixelFormat format_;
  SDL_Palette palette_;
  SDL_Color palette_colors_[2];
};

Index1Impl::Index1Impl(int w, int h, Uint32 flags)
    : Impl(w, h, flags) {
  pitch = w / 8;
  if (w % 8)
    pitch++;

  pixels = calloc(1, pitch * h);

  format = &format_;
  format->BitsPerPixel = 1;
  format->BytesPerPixel = 0;  // ??
  format->Rshift = 0;
  format->Gshift = 0;
  format->Bshift = 0;
  format->Rmask = 0;
  format->Gmask = 0;
  format->Bmask = 0;
  format->palette = &palette_;
  format->palette->colors = palette_colors_;
  format->palette->ncolors = 2;

  memset(palette_colors_, 0, sizeof(palette_colors_));
  palette_colors_[0].r = 0xFF;
  palette_colors_[0].g = 0xFF;
  palette_colors_[0].b = 0xFF;
  palette_colors_[1].r = 0x00;
  palette_colors_[1].g = 0x00;
  palette_colors_[1].b = 0x00;
}

struct Index8Impl : Impl {
  Index8Impl(int w, int h, Uint32 flags);

  SDL_PixelFormat format_;
  SDL_Palette palette_;
  SDL_Color palette_colors_[256];
};

Index8Impl::Index8Impl(int w, int h, Uint32 flags)
    : Impl(w, h, flags) {
  pitch = PitchFromWidth(w);
  pixels = calloc(1, pitch * h);

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
}

//----

Uint8* Index8PixelsAt(SDL_Surface* surface, int x, int y) {
  Uint8* pixels = static_cast<Uint8*>(surface->pixels);
  return pixels + (surface->pitch * y) + x;
}

Uint8 Index1PixelAt(const Uint8* row_start, int x) {
  return (row_start[x / 8] >> (7 - (x % 8))) & 1;
}

//----

Index8Impl* video_surface = NULL;
//XXX PP_Resource graphics_2d = 0;

}  // namespace

SDL_Surface* SDL_SetVideoMode(int width, int height, int depth, int video_flags) {
  if (video_surface) {
    error("SDL_SetVideoMode: video surface already initialized!");
    return NULL;
  }

  if (depth != 8) {
    error("SDL_SetVideoMode: only 8 bit color depth is supported!");
    return NULL;
  }

  if (width <= 0) {
    error("width: must be a positive value!");
    return NULL;
  }

  if (height <= 0) {
    error("height: must be a positive value!");
    return NULL;
  }

#if 0
  PP_Size size;
  size.width = width;
  size.height = height;
  graphics_2d = ppb.graphics_2d->Create(g_instance, &size, PP_TRUE);

  ppb.instance->BindGraphics(g_instance, graphics_2d);
#endif

  jslib_canvas_init(width, height);

  video_surface = new Index8Impl(width, height, video_flags);
  return video_surface;
}

SDL_Surface* SDL_GetVideoSurface() {
  return video_surface;
}

SDL_Surface* SDL_CreateRGBSurface(int video_flags, int width, int height, int depth, int a, int b, int c, int d) {
  switch (depth) {
    case 1:
      return new Index1Impl(width, height, video_flags);
    case 8:
      return new Index8Impl(width, height, video_flags);
  }
  error("SDL_CreateRGBSurface [depth=%u]: unsupported color depth!", depth);
  return NULL;
}

void SDL_FreeSurface(SDL_Surface* surface) {
  if (!surface)
    return;

  Impl* impl = static_cast<Impl*>(surface);
  if (--impl->refcount == 0)
    delete impl;
}

void SDL_BlitSurface(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dst, SDL_Rect* dst_rect) {
  //mesg("SDL_BlitSurface [src=%p, dst=%p]", src, dst);
  SDL_Rect default_src_rect;
  if (!src_rect) {
    default_src_rect.x = 0;
    default_src_rect.y = 0;
    default_src_rect.w = src->w;
    default_src_rect.h = src->h;
    src_rect = &default_src_rect;
  }
  SDL_LowerBlit(src, src_rect, dst, dst_rect);
}

void SDL_LowerBlit(SDL_Surface* src, SDL_Rect* src_rect, SDL_Surface* dst, SDL_Rect* dst_rect) {
/*
  mesg("SDL_LowerBlit [src=%p, src_rect=(%u,%u,%u,%u), dst=%p, dst_rect=(%u,%u,%u,%u)]",
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
    error("SDL_LowerBlit: width and height of src and dst much match!");
    return;
  }

  Impl* src_impl = static_cast<Impl*>(src);

  if (src->format->BitsPerPixel == 8 &&
      dst->format->BitsPerPixel == 8) {
    // Assume palettes match!

    SDL_Palette* src_palette = src->format->palette;
    SDL_Palette* dst_palette = dst->format->palette;

    bool palettes_match =
        src_palette->ncolors == dst_palette->ncolors &&
            memcmp(src_palette->colors,
                   dst_palette->colors,
                   src_palette->ncolors * sizeof(SDL_Color)) == 0;

    if (palettes_match) {
      for (int row = 0; row < src_rect->h; ++row) {
        if (src_impl->color_key_ == -1) {
          memcpy(Index8PixelsAt(dst, dst_rect->x, dst_rect->y + row),
                 Index8PixelsAt(src, src_rect->x, src_rect->y + row),
                 src_rect->w);
        } else {
          Uint8* dst_pixels = Index8PixelsAt(dst, dst_rect->x, dst_rect->y + row);
          Uint8* src_pixels = Index8PixelsAt(src, src_rect->x, src_rect->y + row);
          for (int col = 0; col < src_rect->w; ++col) {
            if (src_pixels[col] != src_impl->color_key_)
              dst_pixels[col] = src_pixels[col];
          }
        }
      }
    } else {
      for (int row = 0; row < src_rect->h; ++row) {
        Uint8* dst_pixels = Index8PixelsAt(dst, dst_rect->x, dst_rect->y + row);
        Uint8* src_pixels = Index8PixelsAt(src, src_rect->x, src_rect->y + row);
        for (int col = 0; col < src_rect->w; ++col) {
          if (src_pixels[col] != src_impl->color_key_) {
            SDL_Color color = src_palette->colors[src_pixels[col]];
            dst_pixels[col] = SDL_MapRGB(dst->format, color.r, color.g, color.b);
          }
        }
      }
    }
  } else if (src->format->BitsPerPixel == 1 &&
             dst->format->BitsPerPixel == 8) {
    // Map src palette to dst palette.
    Uint8 mapped_colors[2];
    for (int j = 0; j < 2; ++j) {
      const SDL_Color& color = src->format->palette->colors[j];
      mapped_colors[j] =
          (Uint8) SDL_MapRGB(dst->format, color.r, color.g, color.b);
    }

    for (int row = 0; row < src_rect->h; ++row) {
      Uint8* dst_pixels = Index8PixelsAt(dst, dst_rect->x, dst_rect->y + row);
      Uint8* src_pixels_row_start =
          static_cast<Uint8*>(src->pixels) + src->pitch * (src_rect->y + row);
      for (int col = 0; col < src_rect->w; ++col) {
        Uint8 src_pixel = Index1PixelAt(src_pixels_row_start, src_rect->x + col);
        if (src_pixel != src_impl->color_key_)
          dst_pixels[col] = mapped_colors[src_pixel];
      }
    }
  }
}

int SDL_LockSurface(SDL_Surface* surface) {
  //error("SDL_LockSurface [%p]", surface);
  return 0;
}

void SDL_UnlockSurface(SDL_Surface* surface) {
  //error("SDL_UnlockSurface [%p]", surface);
}

void SDL_UpdateRects(SDL_Surface* surface, int num_rects, SDL_Rect* rects) {
  //mesg("SDL_UpdateRects [%p num=%u]\n", surface, num_rects);
  //for (int i = 0; i < num_rects; ++i)
  //  mesg(" (%u,%u,%u,%u)\n", rects[i].x, rects[i].y, rects[i].w, rects[i].h);

  if (surface != video_surface) {
    mesg("SDL_UpdateRects: can only be called on the video surface!\n");
    return;
  }

  //XXX PP_ImageDataFormat image_format = ppb.image_data->GetNativeImageDataFormat();

  for (int i = 0; i < num_rects; ++i) {
    int size_width = rects[i].w;
    int size_height = rects[i].h;

    // Skip empty rects.
    if (!size_width || !size_height)
      continue;

    //XXX PP_Resource image_data = ppb.image_data->Create(
    //XXX     g_instance,
    //XXX     image_format,
    //XXX     &size,
    //XXX     PP_FALSE);

    //XXX PP_ImageDataDesc image_desc;
    //XXX ppb.image_data->Describe(image_data, &image_desc);

    struct ImageDesc {
      struct Size {
        int width;
        int height;
      } size;
      int stride;
    };
    ImageDesc image_desc;
    image_desc.size.width = size_width;
    image_desc.size.height = size_height;
    image_desc.stride = size_width * 4;  // num bytes to next row

    void* image_pixels = malloc(image_desc.size.height * image_desc.stride); //XXX ppb.image_data->Map(image_data);
    Uint32* image_pixels_end = (Uint32*) (((Uint8*) image_pixels) + image_desc.size.height * image_desc.stride);

    //mesg("about to copy pixels [size=(%u,%u), stride=%u, pixels=%p]", image_desc.size.width, image_desc.size.height, image_desc.stride, image_pixels);

    // Copy pixels
    for (int row = 0; row < rects[i].h; ++row) {
      Uint8* src_pixels = Index8PixelsAt(surface, rects[i].x, rects[i].y + row);
      Uint8* dst_pixels = static_cast<Uint8*>(image_pixels) + row * image_desc.stride;

      //mesg("about to write row [%u] of pixels", row);
      
      for (int col = 0; col < rects[i].w; ++col) {
        Uint8 index = src_pixels[col];
        SDL_Color color = surface->format->palette->colors[index];

        Uint32 packed_color;
        /*
        switch (image_format) {
          case PP_IMAGEDATAFORMAT_BGRA_PREMUL:
          */
            // RGBA
            packed_color = color.r | (color.g << 8) | (color.b << 16) | 0xFF000000;
            /*
            break;
          default:
            error("Oops: need code for image format!");
            return;
        }
        */

        // XXX
        //mesg("writing dst_pixels [row=%d, col=%d]", row, col);
        Uint32* dst_ptr = reinterpret_cast<Uint32*>(dst_pixels + (4 * col));
        *dst_ptr = packed_color;
      }
    }

    // Now, flush to display
    //XXX PP_Point top_left;
    //XXX top_left.x = rects[i].x;
    //XXX top_left.y = rects[i].y;
    //mesg("PaintImageData(x=%u,y=%u,w=%u,h=%u)", rects[i].x, rects[i].y, rects[i].w, rects[i].h);
    //XXX ppb.graphics_2d->PaintImageData(graphics_2d, image_data, &top_left, NULL);
    
    jslib_canvas_draw(image_pixels, rects[i].x, rects[i].y, rects[i].w, rects[i].h);

    //XXX ppb.image_data->Unmap(image_data);
    //XXX ppb.core->ReleaseResource(image_data);

    free(image_pixels);
  }

  //static PP_TimeTicks last_flush_end = 0.0;
  //PP_TimeTicks start = ppb.core->GetTimeTicks();

  //XXX ppb.graphics_2d->Flush(graphics_2d, PP_BlockUntilComplete());

  //PP_TimeTicks end = ppb.core->GetTimeTicks();
  //PP_TimeDelta delta_this = 1000.0 * (end - start);
  //PP_TimeDelta delta_last = 1000.0 * (end - last_flush_end);
  //last_flush_end = end;
  //mesg("Flushed [%0.0f %0.0f %0.2f]", delta_this, delta_last, delta_last - delta_this);
}

SDL_Surface* SDL_LoadBMP(const char* path) {
  //mesg("SDL_LoadBMP [\"%s\"]", path);

  SDL_RWops* ops = SDL_RWFromFile(path, "rb");
  if (!ops)
    return NULL;
  BMPFileHeader header;
  ReadBitmapHeader(ops, &header);

  //DumpBitmapHeader(&header);

  int palette_size = header.bitmap_offset - kBMPHeaderSize;
  Uint8* palette_data = new Uint8[palette_size];
  SDL_RWread(ops, palette_data, 1, palette_size);

  int data_size = header.size - header.bitmap_offset;
  Uint8* bitmap_data = new Uint8[data_size];
  SDL_RWread(ops, bitmap_data, 1, data_size);

  SDL_RWclose(ops);

  Index8Impl* impl = NULL;
  do {
    if (header.bits_per_pixel != 8) {
      error("SDL_LoadBMP: can only handle 8 bit depth!");
      break;
    }
    if (header.compression != 0) {
      error("SDL_LoadBMP: can only handle uncompressed data!");
      break;
    }

    //mesg("  (num_colors=%u, palette_size=%u)", header.num_colors, palette_size);

    impl = new Index8Impl(header.width, header.height, SDL_SWSURFACE);

    // Copy palette over
    for (int i = 0; i < header.num_colors; ++i) {
      Uint8* color_data = palette_data + i*4;
      impl->format->palette->colors[i].r = color_data[0];
      impl->format->palette->colors[i].g = color_data[1];
      impl->format->palette->colors[i].b = color_data[2];
    }

    // Copy pixels over
    for (int row = 0; row < header.height; ++row) {
      Uint32 pitch = PitchFromWidth(header.width);

      // bitmap_data rows are flipped upside down.
      Uint8* src_row = bitmap_data + (header.height - row - 1) * pitch;
      Uint8* dst_row = static_cast<Uint8*>(impl->pixels) + row * pitch;
      memcpy(dst_row, src_row, header.width);
    }
  } while (false);

  delete[] palette_data;
  delete[] bitmap_data;
  return impl;
}

int SDL_SaveBMP(SDL_Surface* surface, const char* path) {
  mesg("SDL_SaveBMP [%p, \"%s\"]", surface, path);
  return 0;
}

void SDL_SetGammaRamp(Uint16*, Uint16*, Uint16*) {
  //mesg("SDL_SetGammaRamp");
}

void SDL_SetColorKey(SDL_Surface* surface, Uint32 flags, Uint32 key) {
  if (surface->format->BitsPerPixel != 8 &&
      surface->format->BitsPerPixel != 1) {
    error("SDL_SetColorKey: unsupported pixel format!");
    return;
  }

  Impl* impl = static_cast<Impl*>(surface);

  if (flags == 0) {
    impl->color_key_ = -1;
  } else {
    if (key < surface->format->palette->ncolors) {
      impl->color_key_ = key;
    } else {
      error("SDL_SetColorKey: invalid color index!");
    }
  }
}

void SDL_SetColors(SDL_Surface* surface, SDL_Color* colors, int offset, int count) {
  for (int i = 0; i < count; ++i)
    surface->format->palette->colors[i + offset] = colors[i];
}

Uint32 SDL_MapRGB(SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b) {
  if (format->BitsPerPixel != 8) {
    error("SDL_MapRGB: only know how to handle 8 bit depth!");
    return 0;
  }

  Uint32 best_index = 0;
  Uint32 best_diff = -1;

  for (int i = 0; i < format->palette->ncolors; ++i) {
    const SDL_Color& color = format->palette->colors[i];
    
    Uint32 diff = 0;
    if (color.r > r) {
      diff += color.r - r;
    } else {
      diff += r - color.r;
    }
    if (color.g > g) {
      diff += color.g - g;
    } else {
      diff += g - color.g;
    }
    if (color.b > b) {
      diff += color.b - b;
    } else {
      diff += b - color.b;
    }

    if (diff < best_diff) {
      best_diff = diff;
      best_index = i;
    }

    if (best_diff == 0)
      break;
  }

  return best_index;
}
