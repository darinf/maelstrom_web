#ifndef SDL_rwops_h_
#define SDL_rwops_h_

/* placeholder */

#include "SDL_types.h"

struct SDL_RWops {
};

const char* SDL_GetError();
void SDL_SetError(const char*);

SDL_RWops* SDL_RWFromFile(const char* path, const char* mode);
int SDL_RWclose(SDL_RWops*);
Uint16 SDL_ReadBE16(SDL_RWops*);
Uint32 SDL_ReadBE32(SDL_RWops*);
Uint16 SDL_ReadLE16(SDL_RWops*);
Uint32 SDL_ReadLE32(SDL_RWops*);
Sint32 SDL_RWread(SDL_RWops*, void* buf, Uint32 size, Uint32 count);
Sint32 SDL_RWwrite(SDL_RWops*, const void* buf, Uint32 size, Uint32 count);
Uint32 SDL_WriteBE32(SDL_RWops*, Uint32);
Uint32 SDL_WriteLE32(SDL_RWops*, Uint32);
Uint32 SDL_RWseek(SDL_RWops*, Sint32 offset, int whence);
Uint32 SDL_RWtell(SDL_RWops*);

#endif  /* SDL_rwops_h_ */
