

.text

.global cas
.global main

no_cero:

  movq %rdx , (%rdi)

  xorq %rax, %rax
  retq

cas:

  cmpq (%rdi),%rsi
  
  jne no_cero

  xorq %rax ,%rax

  retq 

main:

  call cas
  xorq %rax ,%rax
  
  retq