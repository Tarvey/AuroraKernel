# Aurora Lightweight Kernel
This is just a simple project that I'm working on, nothing big, might add more.
## Building
### Preparing
To make it cleaner, add a directory named `bin`.

Set the `addargs` variable, so if you want to add more arguments, you don't have to edit the actual building command.

On a normal shell (sh, bash, etc.): `addargs="args go here"`

On fish: `set addargs "args go here"`

Change the directory to `bin`
### Compiling
Compile the main assembly file: `nasm -f elf32 ../kernel.asm -o kasm.o`

Compile the kernel: `gcc -m32 -c ../kernel.c -o kc.o -lm`: Linux, ?: Windows

Link the assembly and compiled kernel: `ld -m elf_i386 -T ../link.ld -o kernel kasm.o kc.o $addargs`
### Test
Remove kasm.o and kc.o, they are not needed now.

Test with QEMU: qemu-system-i386 -kernel kernel 

After testing, go back to the main directory: `cd ..`
