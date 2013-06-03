#include "SDL_net.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "SDL_endian.h"

namespace {

//----

struct UDPsocket_Impl : _UDPsocket {
  UDPsocket_Impl* next_;

  IPaddress peer;
  struct {
    IPaddress address;
    UDPpacket* packet_head;
    UDPpacket* packet_tail;
  } channels[2];

  UDPsocket_Impl() { memset(this, 0, sizeof(*this)); }
};

UDPsocket_Impl* ToImpl(UDPsocket socket) {
  return static_cast<UDPsocket_Impl*>(socket);
}

//----

struct SocketSet_Impl : _SDLNet_SocketSet {
  UDPsocket_Impl* udp_sockets_head;
  UDPsocket_Impl* udp_sockets_tail;
  
  SocketSet_Impl() { memset(this, 0, sizeof(*this)); }
};

SocketSet_Impl* ToImpl(SDLNet_SocketSet set) {
  return static_cast<SocketSet_Impl*>(set);
}

//----

void FreePacket(UDPpacket* packet) {
  delete[] packet->data;
  delete packet;
}

UDPpacket* CopyPacket(UDPpacket* packet) {
  UDPpacket* copy = SDLNet_AllocPacket(packet->maxlen);
  memcpy(copy->data, packet->data, packet->len);
  copy->len = packet->len;
  return copy;
}

//----

template <typename Node>
void SListAppend(Node** head, Node** tail, Node* node) {
  if (*tail) {
    (*tail)->next_ = node;
    *tail = node;
  } else {
    *head = *tail = node;
  }
}

template <typename Node>
void SListPopFront(Node** head, Node** tail) {
  *head = (*head)->next_;
  if (!*head)
    *tail = NULL;
}

}  // namespace

//----

int SDLNet_Init() {
  return 0;
}

void SDLNet_Quit() {
}

void SDLNet_ResolveHost(IPaddress* address, const char* host, int port) {
  //fprintf(stderr, "SDLNet_ResolveHost [%s:%u]\n", host, port);
  if (host && !strcmp(host, "127.0.0.1")) {
    address->host = 0x7f000001;
  } else {
    address->host = 0;
  }
  address->port = port;
}

UDPsocket SDLNet_UDP_Open(int port) {
  //fprintf(stderr, "SDLNet_UDP_Open [port=%u]\n", port);
  return new UDPsocket_Impl();
}

void SDLNet_UDP_Bind(UDPsocket socket, int channel, IPaddress* address) {
  //fprintf(stderr, "SDLNet_UDP_Bind (socket=%p, channel=%u, host=%u, port=%u)\n",
  //    socket, channel, address->host, address->port);

  if (channel > 1) {
    fprintf(stderr, "SDLNet_UDP_Bind only supports 2 channels!\n");
    return;
  }
  UDPsocket_Impl* impl = ToImpl(socket);
  impl->channels[channel].address = *address;
}

void SDLNet_UDP_Send(UDPsocket socket, int channel, UDPpacket* packet) {
  //fprintf(stderr, "SDLNet_UDP_Send (socket=%p, channel=%u, len=%u)\n", socket, channel, packet->len);

  if (channel < 0) {
    fprintf(stderr, "channel < 0 not supported!\n");
    return;
  }

  // Send to the other channel.
  channel = (channel + 1) % 2;

  UDPsocket_Impl* impl = ToImpl(socket);

  UDPpacket** head = &impl->channels[channel].packet_head;
  UDPpacket** tail = &impl->channels[channel].packet_tail;
  SListAppend(head, tail, CopyPacket(packet));
}

int SDLNet_UDP_Recv(UDPsocket socket, UDPpacket* result) {
  //fprintf(stderr, "SDLNet_UDP_Recv [socket=%p, maxlen=%u]\n", socket, result->maxlen);

  UDPsocket_Impl* impl = ToImpl(socket);
  for (int i = 1; i >= 0; --i) {  // check in reverse order
    if (impl->channels[i].packet_head) {
      UDPpacket* packet = impl->channels[i].packet_head; 

      //fprintf(stderr, "  ... found packet on channel=%u\n", i);

      if (packet->len > result->maxlen) {
        fprintf(stderr, "need to support partial recv!\n");
        return -1;
      }

      result->address = impl->channels[i].address;
      result->channel = i;
      memcpy(result->data, packet->data, packet->len);
      result->len = packet->len;

      SListPopFront(&impl->channels[i].packet_head,
                    &impl->channels[i].packet_tail);

      FreePacket(packet);
      return 1;
    }
  }

  return -1;
}

TCPsocket SDLNet_TCP_Open(IPaddress*) {
  return 0;
}

int SDLNet_TCP_Send(TCPsocket, const void*, int num_bytes) {
  return 0;
}

int SDLNet_TCP_Recv(TCPsocket, void*, int max_bytes) {
  return 0;
}

void SDLNet_TCP_Close(TCPsocket) {
}

SDLNet_SocketSet SDLNet_AllocSocketSet(int) {
  return new SocketSet_Impl();
}

void SDLNet_UDP_AddSocket(SDLNet_SocketSet set, UDPsocket socket) {
  //fprintf(stderr, "SDLNet_UDP_AddSocket\n");

  SocketSet_Impl* impl = ToImpl(set);
  
  UDPsocket_Impl** head = &impl->udp_sockets_head;
  UDPsocket_Impl** tail = &impl->udp_sockets_tail;
  SListAppend(head, tail, ToImpl(socket));
}

void SDLNet_TCP_AddSocket(SDLNet_SocketSet, TCPsocket) {
  fprintf(stderr, "SDLNet_TCP_AddSocket not implemented!\n");
}

int SDLNet_CheckSockets(SDLNet_SocketSet set, int timeout_msec) {
  //fprintf(stderr, "SDLNet_CheckSockets\n");

  SocketSet_Impl* impl = ToImpl(set);
  for (UDPsocket_Impl* socket = impl->udp_sockets_head;
       socket != NULL;
       socket = socket->next_) {
    for (int i = 0; i < 2; ++i) {
      if (socket->channels[i].packet_head)
        return 1;
    }
  }

  // If there is no data, then throttle.
  usleep(timeout_msec * 1000);
  return 0;
}

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

void SDLNet_Write32(Uint32 value, Uint8* data) {
  Uint32 be32_value = SDL_SwapBE32(value);
  Uint32* ptr = reinterpret_cast<Uint32*>(data);
  memcpy(ptr, &be32_value, sizeof(value));
}

Uint32 SDLNet_Read32(const Uint8* data) {
  const Uint32* ptr = reinterpret_cast<const Uint32*>(data);
  Uint32 value;
  memcpy(&value, ptr, sizeof(value));
  return SDL_SwapBE32(value);
}
