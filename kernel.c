unsigned char old_VGAC;
#include "libraries/hextouchar.h"
#include "libraries/yield.h"
#include "libraries/aurora_utils.h"
#include "libraries/select.h"
void delay();
int init();
#include "libraries/keyboard.h" /*Keyboard handling*/
int finit(); /*included init*/
#define Repository "Tarvey/AuroraKernel"
void alk_poweroff() {
    // Generate a system shutdown using BIOS interrupt 0x15
    asm volatile (
        "movw $0x5307, %ax\n\t"  // Specify 0x5307 (BIOS shutdown)
        "movw $0x0001, %bx\n\t"  // Specify 0x0001 (shutdown)
        "movw $0x03, %cx\n\t"    // Specify 0x03 (turn off the system)
        "int $0x15\n\t"          // Trigger interrupt 0x15
    );
}



/*Call another init system here*/
int init()
{
	finit();
        
}

/*finit*/
void ash() {
        alk_printf_legacy("> ", 24);
    char buffer[256];
    int index = 2;
    buffer[0] = '>';
    buffer[1] = ' ';

    init_pics(0x20, 0x28); // Initialize the PICs once before the loop
    while (1) {
        char c = 0;
        do {
            char new_c = inb(0x60); // Read a character from the keyboard port
            if (new_c != c) { // If the character is new
                c = new_c;
                if (c > 0 && c < 128) { // Only process valid scan codes
                    char translated = scancode_to_char[(int)c]; // Translate the scan code
                    if (translated) { // If the translated character is valid
                        if (translated == '\n' || translated == '\r') { // Check if Enter key is pressed
                            alk_printf(buffer);
                            buffer[index] = '\0'; // Null-terminate the string
                            
                            char *command = buffer+2;
                             char *first_word = command; char *second_word = ((void*)0);
                             for (int i = 0; command[i] != '\0'; i++) {
                                if (command[i] == ' ') {
                                    command[i] = '\0'; // Terminate first word
                                    second_word = command + i + 1; // Point to second word
                                    break;
                                }
                            }
                            old_VGAC=VGAC;
                            VGAC=0xF0;
                            alk_printf_legacy(">                                                           ", 24); // Print the accumulated string
                            VGAC=old_VGAC;
                            if (index > 2 && my_strcmp(buffer + 2, "panic") == 0) {
                                alk_panic("triggered"); // Display help if the command is "help"
                            } else if (index > 2 && my_strcmp(buffer + 2, "menu") == 0) {
                                if (handle_input()==2){alk_printf("Good!");}
                            } else if (first_word && my_strcmp(first_word, "setcolor") == 0) {
                                if (second_word) {
                                    unsigned char color = (unsigned char)hex_to_uchar(second_word);
                                    VGAC = color; // Save the color value to VGAC
                                    alk_printf("Color set successfully.");
                                } else {
                                    alk_printf("Error: No color value provided.");
                                }
                            }
                            index = 2; // Reset the buffer index for the next input
                            buffer[0] = '>';
                            buffer[1] = ' ';
                        } else if (translated == '\b') { // Handle backspace
                            if (index > 2) { // Ensure we don't go before the initial prompt
                                index--; // Move the index back
                                buffer[index] = ' '; // Null-terminate the string
                                old_VGAC=VGAC;
                                VGAC=0xF0;
                                alk_printf_legacy(buffer, 24); // Update the display
                                VGAC=old_VGAC;
                            }
                        } else {
                            buffer[index++] = translated; // Store the character in the buffer
                            if (index >= 256) { // Prevent buffer overflow
                                index = 255; // Keep index within bounds
                            }
                            buffer[index] = '\0'; // Null-terminate the string for display
                            old_VGAC=VGAC;
                            VGAC=0xF0;
                            alk_printf_legacy(buffer, 24); // Update the display
                            VGAC=old_VGAC;
                        }
                    }
                }
            }
        } while (1); // Infinite loop to keep reading characters
    }
}

void finit_tasks(){

}
int finit() {
    alk_printf("Welcome to finit!");
    alk_printf("Add stuff here using a function, they start with finit_");
    ash();
    
    while (1);
}


/* simple kernel written in C */
void alk()
{
	/* Plugins and startup */
        alk_clear_screen();
        alk_setcolor(0x0F);
        alk_printf("Welcome to the Aurora Lightweight Kernel!");
        init();
        alk_panic("init");
};





