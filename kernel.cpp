/*
    kernel.cpp : custom OS kernel written in C++

    Based on the code from https://wiki.osdev.org/Bare_Bones.

    Created by Syed Taqi Haider
*/

// Custom printf function
void printf(char *str) {
    unsigned short *videoMem = (unsigned short *)0xB8000000;

    for (int i = 0; str[i] != '\0'; i++) {
        videoMem[i] = (videoMem[i] & 0xFF00) | str[i];
    }
}

// kernel main function called by loader.s
extern "C" void kernelMain(void *multiboot_struct, unsigned int magic_num)
{
    printf("Welcome to OS++");

    while(1);
}

