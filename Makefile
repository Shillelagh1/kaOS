build:
	-mkdir tmp
	@echo
	#
	# ==== Boostrap ====
	nasm -felf32 src/boot.s -o tmp/boot.o
	
	@echo
	#
	# ==== Kernel Core ====
	$$HOME/opt/cross/bin/i686-elf-g++ -c src/kernel_core.cpp -o tmp/kernel_core.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

	@echo
	#
	# ==== Console Core ====
	$$HOME/opt/cross/bin/i686-elf-g++ -c src/kernel_console.cpp -o tmp/kernel_console.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

	@echo
	#
	# ==== Libc ====
	$$HOME/opt/cross/bin/i686-elf-g++ -c src/libc.cpp -o tmp/libc.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

	@echo
	#
	# ==== Linking ====
	$$HOME/opt/cross/bin/i686-elf-gcc -T src/LINKER.ld -o tmp/kaOS.bin -ffreestanding -nostdlib tmp/boot.o tmp/kernel_core.o tmp/libc.o tmp/kernel_console.o -lgcc

	@echo
	#
	# ==== Multiboot Test ====
	grub-file --is-x86-multiboot tmp/kaOS.bin
	# ===> Success!
	
	-mkdir bin
	mkdir -p tmp/isodir/boot/grub
	cp tmp/kaOS.bin tmp/isodir/boot/kaOS.bin
	cp src/grub.cfg tmp/isodir/boot/grub/grub.cfg
	grub-mkrescue -o bin/kaOS.iso tmp/isodir

	@echo
	#
	# ==== Cleanup ====
	rm -rf tmp

run: build
	qemu-system-i386 -cdrom bin/kaOS.iso -monitor stdio

exp: build
	-rm /media/sf_shared/kaOS.iso
	cp bin/kaOS.iso /media/sf_shared/kaOS.iso
	
