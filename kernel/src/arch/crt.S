  .intel_syntax noprefix
  .section .text
  .globl memset
  .type memset, %function
memset:
  # Inputs:
  # rdi = destination
  # rax = value to set (only ax used)
  # rdx = size (number of bytes to set)
  push rbp
  mov rbp, rsp

  push rdi
  mov al, sil
  mov rcx, rdx
  rep stosb
  pop rax

  leave
  ret


  .globl memcpy
  .type memcpy, %function
memcpy:
  # Inputs:
  # rdi = destination
  # rsi = source
  # rdx = size (number of bytes to copy)
  push rbp
  mov rbp, rsp

  mov rax, rdi
  mov rcx, rdx
  rep movsb

  leave
  ret


  .globl memcmp
  .type memcmp, %function
memcmp:
  # Inputs:
  # rdi = pointer to block 1
  # rsi = pointer to block 2
  # rdx = size (number of bytes to compare)
  push rbp
  mov rbp, rsp

  mov rcx, rdx
  repe cmpsb
  sete al

  leave
  ret


  .globl memmove
  .type memmove, %function
memmove:
  # Inputs:
  # rdi = destination
  # rsi = source
  # rdx = size (number of bytes to move)

  test rdx, rdx
  je .done

  cmp rdi, rsi
  # If destination >= source, copy backwards
  jae .backward
.forward:
  mov rcx, rdx
  shr rcx, 3
  rep movsq

  mov rcx, rdx
  and rcx, 7
  rep movsb

  jmp .done
.backward:
  # Backward copy for overlapping regions
  lea rsi, [rsi + rdx - 1]
  lea rdi, [rdi + rdx - 1]
  mov rcx, rdx
  shr rcx, 3
  std
  rep movsq
  mov rcx, rdx
  and rcx, 7
  rep movsb
  cld        
.done:
  ret


  .globl strlen
  .type strlen, %function
strlen:
	push rbp
	mov rbp, rsp

	xor rcx, rcx
	not rcx
	xor eax,eax
	repne scasb
	sub rax, rcx
	sub rax, 2

	leave
	ret
