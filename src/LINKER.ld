ENTRY(_start)

SECTIONS
{
	. = 2M;

	.text BLOCK(4K) : ALIGN(4K)
	{
		_kernel_start = .;
		*(.multiboot)
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

	_kernel_end = .;
}
