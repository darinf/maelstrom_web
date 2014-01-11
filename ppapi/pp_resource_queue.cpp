#include "pp_resource_queue.h"
#include "ppb.h"

PPResourceQueue::PPResourceQueue()
    : head_(NULL),
      tail_(NULL) {
  pthread_mutex_init(&lock_, NULL);
  pthread_cond_init(&cvar_, NULL);
}

PPResourceQueue::~PPResourceQueue() {
  pthread_cond_destroy(&cvar_);
  pthread_mutex_destroy(&lock_);

  while (head_) {
    PP_Resource resource = PopHead();
    ppb.core->ReleaseResource(resource);
  }
}

void PPResourceQueue::Put(PP_Resource resource) {
  Node* node = new Node();
  node->next = NULL;
  node->resource = resource;
  ppb.core->AddRefResource(resource);

  pthread_mutex_lock(&lock_);

  if (tail_) {
    tail_->next = node;
  } else {
    head_ = node;
  }
  tail_ = node;

  pthread_cond_broadcast(&cvar_);
  pthread_mutex_unlock(&lock_);
}

PP_Resource PPResourceQueue::GetOrFail() {
  PP_Resource result = 0;
  pthread_mutex_lock(&lock_);

  if (head_ != NULL)
    result = PopHead();

  pthread_mutex_unlock(&lock_);
  return result;
}

PP_Resource PPResourceQueue::Get() {
  PP_Resource result = 0;
  pthread_mutex_lock(&lock_);

  while (head_ == NULL)
    pthread_cond_wait(&cvar_, &lock_);
  result = PopHead();

  pthread_mutex_unlock(&lock_);
  return result;
}

PP_Resource PPResourceQueue::PeekOrFail() {
  PP_Resource result = 0;
  pthread_mutex_lock(&lock_);

  if (head_ != NULL)
    result = head_->resource;

  pthread_mutex_unlock(&lock_);
  return result;
}

PP_Resource PPResourceQueue::Peek() {
  PP_Resource result = 0;
  pthread_mutex_lock(&lock_);

  while (head_ == NULL)
    pthread_cond_wait(&cvar_, &lock_);
  result = head_->resource;

  pthread_mutex_unlock(&lock_);
  return result;
}

PP_Resource PPResourceQueue::PopHead() {
  Node* node = head_;
  head_ = head_->next;
  if (!head_)
    tail_ = NULL;
  PP_Resource result = node->resource;
  delete node;
  return result;
}
