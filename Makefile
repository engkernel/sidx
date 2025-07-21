FILES = ./build/boot/head.s.o ./build/kernel/kmain.o ./build/utils/string.o ./build/utils/vga.o ./build/utils/log.o ./build/mm/mm.o ./build/io/io.s.o ./build/interrupt/isr_wrapper.s.o ./build/interrupt/idt.s.o ./build/interrupt/isr.o ./build/interrupt/idt.o ./build/mm/mmap.o ./build/mm/pmm.o ./build/mm/buddy.o ./build/task/task.s.o ./build/task/task.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin
	
./bin/boot.bin: ./src/arch/x86/boot/boot.s ./src/arch/x86/boot/boot.ld
	as -I./src/arch/x86/boot --32 --gstabs ./src/arch/x86/boot/boot.s -o ./build/boot/boot.o
	ld -m elf_i386 -T ./src/arch/x86/boot/boot.ld ./build/boot/boot.o -o $@ 

./build/boot/head.s.o: ./src/arch/x86/boot/head.s
	as --32 --gstabs ./src/arch/x86/boot/head.s -o ./build/boot/head.s.o

./build/kernel/kmain.o:
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel/kmain.c -o ./build/kernel/kmain.o

./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin --freestanding -O0 -nostdlib ./build/kernelfull.o

./build/utils/string.o: ./src/utils/string.c
	i686-elf-gcc $(INCLUDES) -I./src/utils $(FLAGS) -std=gnu99 -c ./src/utils/string.c -o ./build/utils/string.o

./build/utils/vga.o: ./src/utils/vga.c
	i686-elf-gcc $(INCLUDES) -I./src/utils $(FLAGS) -std=gnu99 -c ./src/utils/vga.c -o ./build/utils/vga.o
	
./build/utils/log.o: ./src/utils/log.c
	i686-elf-gcc $(INCLUDES) -I./src/utils $(FLAGS) -std=gnu99 -c ./src/utils/log.c -o ./build/utils/log.o

./build/mm/mm.o: ./src/mm/mm.c
	i686-elf-gcc $(INCLUDES) -I./src/mm $(FLAGS) -std=gnu99 -c ./src/mm/mm.c -o ./build/mm/mm.o

./build/mm/pmm.o: ./src/mm/pmm.c
	i686-elf-gcc $(INCLUDES) -I./src/mm $(FLAGS) -std=gnu99 -c ./src/mm/pmm.c -o ./build/mm/pmm.o

./build/mm/buddy.o: ./src/mm/buddy.c
	i686-elf-gcc $(INCLUDES) -I./src/mm $(FLAGS) -std=gnu99 -c ./src/mm/buddy.c -o ./build/mm/buddy.o

./build/mm/kheap.o: ./src/mm/kheap.c
	i686-elf-gcc $(INCLUDES) -I./src/mm $(FLAGS) -std=gnu99 -c ./src/mm/kheap.c -o ./build/mm/kheap.o

./build/io/io.s.o: ./src/arch/x86/io/io.s
	as --32 --gstabs ./src/arch/x86/io/io.s -o ./build/io/io.s.o

./build/interrupt/isr_wrapper.s.o: ./src/arch/x86/interrupt/isr_wrapper.s
	as --32 --gstabs ./src/arch/x86/interrupt/isr_wrapper.s -o ./build/interrupt/isr_wrapper.s.o

./build/interrupt/idt.s.o: ./src/arch/x86/interrupt/idt.s
	as --32 --gstabs ./src/arch/x86/interrupt/idt.s -o ./build/interrupt/idt.s.o

./build/interrupt/isr.o: ./src/arch/x86/interrupt/isr.c
	i686-elf-gcc $(INCLUDES) -I./src/arch/x86/interrupt $(FLAGS) -std=gnu99 -c ./src/arch/x86/interrupt/isr.c -o ./build/interrupt/isr.o

./build/interrupt/idt.o: ./src/arch/x86/interrupt/idt.c
	i686-elf-gcc $(INCLUDES) -I./src/arch/x86/interrupt $(FLAGS) -std=gnu99 -c ./src/arch/x86/interrupt/idt.c -o ./build/interrupt/idt.o

./build/mm/mmap.o: ./src/arch/x86/mmap/mmap.c
	i686-elf-gcc $(INCLUDES) -I./src/arch/x86/mmap $(FLAGS) -std=gnu99 -c ./src/arch/x86/mmap/mmap.c -o ./build/mm/mmap.o

./build/task/tss.s.o: ./src/task/tss.s
	as --32 --gstabs ./src/task/tss.s -o ./build/task/tss.s.o

./build/task/task.s.o: ./src/task/task.s
	as --32 --gstabs ./src/task/task.s -o ./build/task/task.s.o

./build/task/task.o: ./src/task/task.c
	i686-elf-gcc $(INCLUDES) -I./src/task $(FLAGS) -std=gnu99 -c ./src/task/task.c -o ./build/task/task.o


clean: 
	rm -rf ./bin/boot.bin
	rm -rf ./build/kernelfull.o
	rm -rf ./bin/os.bin
	rm -rf ./bin/kernel.bin
	rm -rf ${FILES}

