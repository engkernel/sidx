ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
	. = 1M;
	.text : ALIGN(4096)
	{
		*(.text)
	}

	.rodata : ALIGN(4096)
	{
		*(.rodata)
	}

	.data : ALIGN(4096)
	{
		*(.data)
	}

	.bss : ALIGN(4096)
	{
		*(COMM)
		*(.bss)
	}

	kernel_end_phys = .;
}
