#include <sys/syscall.h>

.globl _start
_start:
  movq $1, %rax           # write syscall number (SYS_write = 1)
  movq $1, %rdi           # fd=1 (stdout)
  movq $st, %rsi          # buf=st
  movq $(ed - st), %rdx   # count=ed-st
  syscall                 # invoke syscall

  movq $60, %rax          # exit syscall number (SYS_exit = 60)
  movq $1, %rdi           # status=1
  syscall                 # invoke syscall

st:
  .ascii "\033[01;31mHello, OS World\033[0m\n"
ed:
