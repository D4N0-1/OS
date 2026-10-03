#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char ch;
char *delims = " -\r\t\n./,";
char *nums = "0123456789";
void
sixfive(int fd)
{
  int n;
  int sum = 0;
  enum state { DELIM, NUMBER, OTHER };
  enum state s = NUMBER;
  while ((n = read(fd, &ch, 1)) > 0) {
    if (strchr(delims, ch)) {
      if (s == NUMBER) {
        if (sum % 5 == 0 || sum % 6 == 0) {
          fprintf(1, "%d\n", sum);
        }
        sum = 0;
      }
      s = DELIM;
    } else if (strchr(nums, ch)) {
      if (s == DELIM) {
        sum = atoi(&ch);
        s = NUMBER;
      } else if (s == NUMBER) {
        sum = sum * 10;
        sum = sum + atoi(&ch);
      }
    } else {
      s = OTHER;
    }
  }
  if (n < 0) {
    fprintf(2, "sixfive: read error\n");
    exit(1);
  }
  if (sum % 5 == 0 || sum % 6 == 0) {
    fprintf(1, "%d\n", sum);
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if (argc <= 1) {
    sixfive(0);
    exit(0);
  }

  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], O_RDONLY)) < 0) {
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }
    sixfive(fd);
    close(fd);
  }
  exit(0);
}
