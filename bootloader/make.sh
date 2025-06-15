nasm -f bin loader.asm -o loader.bin
nasm -f bin boot.asm -o boot.bin
dd if=/dev/zero of=os.img bs=512 count=2048
dd if=boot.bin of=os.img bs=512 count=1 conv=notrunc
dd if=loader.bin of=os.img bs=512 seek=2 conv=notrunc