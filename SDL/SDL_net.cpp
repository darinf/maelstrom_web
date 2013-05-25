#include "SDL_net.h"

#include <string.h>

int SDLNet_Init() { return 0; }
void SDLNet_Quit() {}

void SDLNet_ResolveHost(IPaddress*, const char* host, int port) {}

struct UDPsocket_Impl : _UDPsocket {
  IPaddress peer;
};

UDPsocket SDLNet_UDP_Open(int port) {
  return new UDPsocket_Impl();
}

void SDLNet_UDP_Bind(UDPsocket, int channel, IPaddress*) {}
int SDLNet_UDP_Send(UDPsocket, int channel, UDPpacket*) { return 0; }
int SDLNet_UDP_Recv(UDPsocket, UDPpacket*) { return 0; }

TCPsocket SDLNet_TCP_Open(IPaddress*) { return 0; }
int SDLNet_TCP_Send(TCPsocket, const void*, int num_bytes) { return 0; }
int SDLNet_TCP_Recv(TCPsocket, void*, int max_bytes) { return 0; }
void SDLNet_TCP_Close(TCPsocket) {}

struct SocketSet_Impl : _SDLNet_SocketSet {
};

SDLNet_SocketSet SDLNet_AllocSocketSet(int) {
  return new SocketSet_Impl();
}
void SDLNet_UDP_AddSocket(SDLNet_SocketSet, UDPsocket) {}
void SDLNet_TCP_AddSocket(SDLNet_SocketSet, TCPsocket) {}
int SDLNet_CheckSockets(SDLNet_SocketSet, int) { return 0; }

const IPaddress* SDLNet_UDP_GetPeerAddress(UDPsocket s, int) {
  return &static_cast<UDPsocket_Impl*>(s)->peer;
}

UDPpacket* SDLNet_AllocPacket(Uint32 size) {
  UDPpacket* packet = new UDPpacket;
  memset(packet, 0, sizeof(*packet));
  packet->data = new Uint8[size];
  packet->maxlen = size;
  return packet;
}

void SDLNet_Write32(Uint32, const Uint8*) {}
Uint32 SDLNet_Read32(Uint8*) { return 0; }
