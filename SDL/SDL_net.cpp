#include "SDL_net.h"

int SDLNet_Init() { return 0; }
void SDLNet_Quit() {}

void SDLNet_ResolveHost(IPaddress*, const char* host, int port) {}

UDPsocket SDLNet_UDP_Open(int port) { return 0; }
void SDLNet_UDP_Bind(UDPsocket, int channel, IPaddress*) {}
int SDLNet_UDP_Send(UDPsocket, int channel, UDPpacket*) { return 0; }
int SDLNet_UDP_Recv(UDPsocket, UDPpacket*) { return 0; }

TCPsocket SDLNet_TCP_Open(IPaddress*) { return 0; }
int SDLNet_TCP_Send(TCPsocket, const void*, int num_bytes) { return 0; }
int SDLNet_TCP_Recv(TCPsocket, void*, int max_bytes) { return 0; }
void SDLNet_TCP_Close(TCPsocket) {}

SDLNet_SocketSet SDLNet_AllocSocketSet(int) { return 0; }
void SDLNet_UDP_AddSocket(SDLNet_SocketSet, UDPsocket) {}
void SDLNet_TCP_AddSocket(SDLNet_SocketSet, TCPsocket) {}
int SDLNet_CheckSockets(SDLNet_SocketSet, int) { return 0; }

const IPaddress* SDLNet_UDP_GetPeerAddress(UDPsocket, int) { return 0; }

UDPpacket* SDLNet_AllocPacket(Uint32 size) { return 0; }

void SDLNet_Write32(Uint32, const Uint8*) {}
Uint32 SDLNet_Read32(Uint8*) { return 0; }
