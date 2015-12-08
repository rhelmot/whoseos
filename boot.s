.intel_syntax noprefix		; please let the suffering end

; Multiboot header, signifies to smart bootloader (GRUB!) that we are a
; multiboot compatible kernel and can be treated with proper respect

.section .multiboot
.align 4

.set MAGIC, 0x1badb002
.set FLAGS, 3
.set CHECKSUM, -(MAGIC + FLAGS)

.long MAGIC
.long FLAGS
.long CHECKSUM

; Bootstrap stack, we don't want to deal with allocating pages for the stack
; so let's just code one in

.section .bootstrap_stack

stack_top:
.skip 0x4000
stack_bottom:

; THE TEXT SECTION WE HAVE ARRIVED

.section .text

.global _start				; start function, bootloader jumps here
.type _start, @function
_start:
	mov esp, stack_top
	call kernel_main

	cli
	hlt
.end:
	jmp .end

.size _start, . - _start	; end of start function
