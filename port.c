#include <signal.h>
#include <unistd.h>

int access(const char* pathname, int mode) {
  return 0;
}

sighandler_t signal(int signum, sighandler_t handler) {
  return 0;
}
