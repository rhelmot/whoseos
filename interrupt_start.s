.intel_syntax noprefix
.section .text

.global handle_all_interrupts
.type handle_all_interrupts, @function
handle_all_interrupts:
	pushad
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	lea eax, [esp+0x4]
	mov ebx, [esp+0x28]
	push eax
	push ebx
	call interrupt_routine

	add esp, 8
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov al, 0x20
	outb 0x20, al
	popad
	ret


#define MAKE_INTERRUPT_ENTRY(N)  \
	.global interrupt_entry_##N	;\
	.type interrupt_entry_##N, @function	;\
	interrupt_entry_##N##:		;\
	push 0x##N					;\
	call handle_all_interrupts	;\
	add esp, 4					;\
	iret;

#define MAKE_INTERRUPT_ENTRY_MANY(M)	\
	MAKE_INTERRUPT_ENTRY(M##0)			\
	MAKE_INTERRUPT_ENTRY(M##1)			\
	MAKE_INTERRUPT_ENTRY(M##2)			\
	MAKE_INTERRUPT_ENTRY(M##3)			\
	MAKE_INTERRUPT_ENTRY(M##4)			\
	MAKE_INTERRUPT_ENTRY(M##5)			\
	MAKE_INTERRUPT_ENTRY(M##6)			\
	MAKE_INTERRUPT_ENTRY(M##7)			\
	MAKE_INTERRUPT_ENTRY(M##8)			\
	MAKE_INTERRUPT_ENTRY(M##9)			\
	MAKE_INTERRUPT_ENTRY(M##A)			\
	MAKE_INTERRUPT_ENTRY(M##B)			\
	MAKE_INTERRUPT_ENTRY(M##C)			\
	MAKE_INTERRUPT_ENTRY(M##D)			\
	MAKE_INTERRUPT_ENTRY(M##E)			\
	MAKE_INTERRUPT_ENTRY(M##F)

	MAKE_INTERRUPT_ENTRY_MANY(0)
	MAKE_INTERRUPT_ENTRY_MANY(1)
	MAKE_INTERRUPT_ENTRY_MANY(2)
	MAKE_INTERRUPT_ENTRY_MANY(3)
	MAKE_INTERRUPT_ENTRY_MANY(4)
	MAKE_INTERRUPT_ENTRY_MANY(5)
	MAKE_INTERRUPT_ENTRY_MANY(6)
	MAKE_INTERRUPT_ENTRY_MANY(7)
	MAKE_INTERRUPT_ENTRY_MANY(8)
	MAKE_INTERRUPT_ENTRY_MANY(9)
	MAKE_INTERRUPT_ENTRY_MANY(A)
	MAKE_INTERRUPT_ENTRY_MANY(B)
	MAKE_INTERRUPT_ENTRY_MANY(C)
	MAKE_INTERRUPT_ENTRY_MANY(D)
	MAKE_INTERRUPT_ENTRY_MANY(E)
	MAKE_INTERRUPT_ENTRY_MANY(F)
