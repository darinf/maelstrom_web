#ifndef SDL_endian_h_
#define SDL_endian_h_

/* placeholder */

#include "SDL_types.h"

Uint16 SDL_SwapBE16(Uint16);
Uint32 SDL_SwapBE32(Uint32);

Uint16 SDL_SwapLE16(Uint16);
Uint32 SDL_SwapLE32(Uint32);

#define SDL_BIG_ENDIAN 0
#define SDL_LITTLE_ENDIAN 1
#define SDL_BYTEORDER SDL_LITTLE_ENDIAN

#endif  /* SDL_endian_h_ */
