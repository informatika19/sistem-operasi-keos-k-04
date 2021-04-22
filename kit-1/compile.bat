bcc -ansi -c -o kernel.o kernel.c 
bcc -ansi -c -o fileio.o fileio.c 
bcc -ansi -c -o fileoperation.o fileoperation.c 
bcc -ansi -c -o ls.o ls.c 
bcc -ansi -c -o math.o math.c 
bcc -ansi -c -o mkdir.o mkdir.c 
bcc -ansi -c -o text.o text.c 
nasm -f as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o fileio.o fileoperation.o ls.o math.o mkdir.o text.o kernel_asm.o 
dd if=kernel of=system.img bs=512 conv=notrunc seek=1