#include "include/autoconf.h"

.section .init
.arm
.align 0

.global StartUp

.org	StartUp

ResetHandler:
	b		p1
#ifdef NAND_RELEASE
.byte	'N','A','N','D'
#else
.byte	'U','A','R','T'
#endif
.align  0
p1:
	ldr		sp,	=__stack_start__
#ifdef SELF_BOOTSTRAP
	bl		bootstrap
#endif
	b		main

.end
