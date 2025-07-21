#ifndef __TASK_H__
#define __TASK_H__

#include <stdint.h>

typedef struct
{
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
	uint32_t esp;
	uint32_t ebp;
	uint32_t eip;
	uint32_t eflags;
	uint32_t cr3;
} registers_t;

typedef struct task
{
	registers_t regs;
	struct task* next;
} task_t;

extern void task_init();
extern void task_new(task_t* task, void(*)(), uint32_t flags, uint32_t* vm_dir);
extern void task_switch(registers_t* old, registers_t* new);
void do_it();
#endif
