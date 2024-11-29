unsigned char VGAC=0x07;


unsigned int alk_printf_legacy(char *message, unsigned int line);
void alk_panic(char *reason);
unsigned int alk_printf(char *message);
void alk_setcolor(unsigned char color);

void alk_clear_screen();

int curline;
unsigned int alk_printf(char *message) {
    curline++;
    char *vidmem = (char *) 0xb8000;
    unsigned int i = 0;

    if (curline >= 23) {
        alk_clear_screen();
        curline = 0;
    }

    i = (curline * 80 * 2);

    while (*message != 0) {
        if (*message == '\n') { // Check for a new line
            curline++;
            if (curline >= 23) {
                alk_clear_screen();
                curline = 0;
            }
            i = (curline * 80 * 2);
        } else {
            vidmem[i] = *message;
            i++;
            vidmem[i] = VGAC;
            i++;
        }
        message++;
    }

    return 1;
}

/*
Color values, Background:Foreground.
Value 	Color
0 	Black
1 	Blue
2 	Green
3 	Aqua
4 	Red
5 	Purple
6 	Yellow
7 	White
8 	Gray
9 	Light blue
a 	Light green
b 	Light aqua
c 	Light red
d 	Light purple
e 	Light yellow
f 	Bright white
*/
void alk_setcolor(unsigned char color)
{
	unsigned int i=0;
	while(i<32)
	{
		VGAC=color;
		alk_printf_legacy("                                                                                          ", i);
		i++;
	}
};


void alk_clear_screen()
{
        curline=0;
        char *vidmem = (char *) 0xb8000;
        unsigned int i=0;
        while(i < (80*25*2))
        {
                vidmem[i]=' ';
                i++;
                vidmem[i]=VGAC;
                i++;
        };
};


unsigned int alk_printf_legacy(char *message, unsigned int line)
{
        char *vidmem = (char *) 0xb8000;
        unsigned int i=0;

        i=(line*80*2);

        while(*message!=0)
        {
                if(*message=='\n') // check for a new line
                {
                        line++;
                        i=(line*80*2);
                        *message++;
                } else {
                        vidmem[i]=*message;
                        *message++;
                        i++;
                        vidmem[i]=VGAC;
                        i++;
                };
        };

        return(1);
}




void alk_panic(char *reason)
{
        alk_clear_screen();
        alk_setcolor(0x07);
        alk_printf_legacy("Kernel Panic!", 0);
        if (reason=="init"){
                alk_printf_legacy("Kernel not syncing.",1);
                alk_printf_legacy("No action to do, init not found.",2);
                alk_printf_legacy("Please read docs/2 init.txt", 3);
        } else if (reason=="triggered") {
                alk_printf_legacy("Kernel not syncing.", 1);
                alk_printf_legacy("alk_panic triggered manually",2);
        };
        while (1){
                asm volatile ("cli"); // Clear interrupt flag 
                asm volatile ("hlt"); // Halt instruction
        }
        
}