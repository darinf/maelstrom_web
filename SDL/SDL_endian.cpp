#include "SDL_endian.h"
#include <algorithm>

template <typename T>
void swap_endian(T* pX)
{
    char* raw = reinterpret_cast<char*>(pX);
    std::reverse(raw, raw + sizeof(T));
}

Uint16 SDL_SwapBE16(Uint16 n) {
  swap_endian(&n);
  return n;
}

Uint32 SDL_SwapBE32(Uint32 n) {
  swap_endian(&n);
  return n;
}

Uint16 SDL_SwapLE16(Uint16 n) {
  return n;
}

Uint32 SDL_SwapLE32(Uint32 n) {
  return n;
}
