#include <stddef.h>
#include <stdint.h>

struct ifaddrs; /* forward declaration to avoid including ifaddrs.h */

int getifaddrs(struct ifaddrs** ifap) {
  (void)ifap;
  return -1; /* not available */
}

void freeifaddrs(struct ifaddrs* ifa) {
  (void)ifa;
}

unsigned if_nametoindex(const char* ifname) {
  (void)ifname;
  return 0;
}
