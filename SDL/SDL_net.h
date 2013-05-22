#ifndef SDL_net_h_
#define SDL_net_h_

/* placeholder */

#include "SDL_types.h"

enum {
  INADDR_NONE = 0,
};

struct IPaddress {
  int host;
  int port;
};

struct UDPpacket {
  IPaddress address;
  int channel;
  Uint8* data;
  Uint32 len;
  Uint32 maxlen;
};

typedef struct _UDPsocket {
} *UDPsocket;

typedef struct _TCPsocket {
} *TCPsocket;

typedef struct _SDLNet_SocketSet {
} *SDLNet_SocketSet;

int SDLNet_Init();
void SDLNet_Quit();

void SDLNet_ResolveHost(IPaddress*, const char* host, int port);

UDPsocket SDLNet_UDP_Open(int port);
void SDLNet_UDP_Bind(UDPsocket, int channel, IPaddress*);
int SDLNet_UDP_Send(UDPsocket, int channel, UDPpacket*);
int SDLNet_UDP_Recv(UDPsocket, UDPpacket*);

TCPsocket SDLNet_TCP_Open(IPaddress*);
int SDLNet_TCP_Send(TCPsocket, const void*, int num_bytes); 
int SDLNet_TCP_Recv(TCPsocket, void*, int max_bytes);
void SDLNet_TCP_Close(TCPsocket);

SDLNet_SocketSet SDLNet_AllocSocketSet(int);
void SDLNet_UDP_AddSocket(SDLNet_SocketSet, UDPsocket);
void SDLNet_TCP_AddSocket(SDLNet_SocketSet, TCPsocket);
int SDLNet_CheckSockets(SDLNet_SocketSet, int);

const IPaddress* SDLNet_UDP_GetPeerAddress(UDPsocket, int);

UDPpacket* SDLNet_AllocPacket(Uint32 size);

void SDLNet_Write32(Uint32, const Uint8*);
Uint32 SDLNet_Read32(Uint8*);

#endif  /* SDL_net_h_ */
