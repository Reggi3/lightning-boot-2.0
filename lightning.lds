OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{ 
	. = 0x0;
	. = ALIGN(4);
	.text	:
	{
	  startup.o	(.init)
	  *(.text)
	}

	. = ALIGN(4);
	.rodata : { *(.rodata) }
	. = ALIGN(4);
	.data : { *(.data) }
	. = ALIGN(4);
	.got : { *(.got) }
	
  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  }

  .init_array     :
  {
     PROVIDE_HIDDEN (__init_array_start = .);
     KEEP (*(SORT(.init_array.*)))
     KEEP (*(.init_array))
     PROVIDE_HIDDEN (__init_array_end = .);
  }
  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array))
    KEEP (*(SORT(.fini_array.*)))
    PROVIDE_HIDDEN (__fini_array_end = .);
  }

	. = 0x4000;
	__kernel_params__ = .;
	. = __kernel_params__ + 0x800;

	. = ALIGN(4);
  __bss_start = .;
  __bss_start__ = .;
	.bss : { *(.bss) }
  _bss_end__ = . ; __bss_end__ = . ;

	. = 0x8000;
	__kernel_load__ = .;
	. = __kernel_load__ + 0x200000;

	. = ALIGN(4);
  __stack_end__ = .;
	. = __stack_end__ + 0x20000;
	__stack_start__ = .;
}


