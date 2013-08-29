#ifndef PP_RESOURCE_QUEUE_H_
#define PP_RESOURCE_QUEUE_H_

#include <ppapi/c/pp_resource.h>
#include <pthread.h>

// A thread-safe queue for PP_Resource objects.
class PPResourceQueue {
 public:
  PPResourceQueue();
  ~PPResourceQueue();

  void Put(PP_Resource resource);

  PP_Resource GetOrFail();
  PP_Resource Get();

 private:
  PP_Resource PopHead();

  struct Node {
    Node* next;
    PP_Resource resource;
  };

  pthread_mutex_t lock_;
  pthread_cond_t cvar_;
  Node* head_;
  Node* tail_;
};

#endif  // PP_RESOURCE_QUEUE_H_
