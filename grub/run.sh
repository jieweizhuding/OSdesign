# 编译 libc.c
gcc -m32 -ffreestanding -c ./kernel/libc.c -o libc.o

gcc -m32 -ffreestanding -c show.c -o show.o

gcc -m32 -ffreestanding -c string.c -o string.o

# 编译 kernel.c
gcc -m32 -ffreestanding -c kernel.c -o kernel.o

gcc -m32 -ffreestanding -c file.c -o file.o

gcc -m32 -ffreestanding -c filesystem.c -o filesystem.o

ld -T linker.ld -m elf_i386 -nostdlib kernel.o libc.o show.o string.o file.o filesystem.o -o kernel.elf


mkdir -p iso/boot/grub
cp kernel.elf iso/boot/
cp grub.cfg iso/boot/grub/
grub-mkrescue -o os.iso iso