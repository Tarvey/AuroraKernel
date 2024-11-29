#include "aurora_utils.h"
#include "keyboard.h"
typedef struct {
    const char *text;
} MenuItem;

#define MENU_SIZE 3
MenuItem menu[MENU_SIZE] = {
    {"Option 1"},
    {"Option 2"},
    {"Option 3"}
};

int current_selection = 0;


unsigned char OLDVGAC;
void display_menu() {
    char buffer[80]; // Buffer to construct the display string

    for (int i = 0; i < MENU_SIZE; i++) {
        int pos = 0;
        if (i == current_selection) {
            buffer[pos++] = '-';
            buffer[pos++] = '>';
        } else {
            buffer[pos++] = ' ';
            buffer[pos++] = ' ';
        }
        buffer[pos++] = ' ';
        
        const char *text = menu[i].text;
        while (*text) {
            buffer[pos++] = *text++;
        }
        buffer[pos] = '\0'; // Null-terminate the string

        alk_printf_legacy(buffer, i);
    }
}


void handle_input() {
    char buffer[256];
    int index = 2;
    buffer[0] = '>';
    buffer[1] = ' ';

    init_pics(0x20, 0x28); // Initialize the PICs once before the loop
    display_menu(); // Display the menu initially

    while (1) {
        char c = 0;
        do {
            char new_c = inb(0x60); // Read a character from the keyboard port
            if (new_c != c) { // If the character is new
                c = new_c;
                if (c > 0 && c < 128) { // Only process valid scan codes
                    char translated = scancode_to_char[(int)c]; // Translate the scan code
                    if (translated == '\n' || translated == '\r') { // Check if Enter key is pressed
                        buffer[index] = '\0'; // Null-terminate the string

                        // Handle menu selection
                        if (buffer[2] == '\0') { // Enter key pressed
                            alk_printf("Selected: ");
                            alk_printf((char *)menu[current_selection].text);
                        }

                        index = 2; // Reset the buffer index for the next input
                        buffer[0] = '>';
                        buffer[1] = ' ';
                    } else if (translated == '\b') { // Handle backspace
                        if (index > 2) { // Ensure we don't go before the initial prompt
                            index--; // Move the index back
                            buffer[index] = ' '; // Null-terminate the string
                            alk_printf_legacy(buffer, 24); // Update the display
                        }
                    } else if (c == 0x48) { // Up arrow
                        if (current_selection > 0) {
                            current_selection--;
                            display_menu(); // Update the menu display
                        }
                    } else if (c == 0x50) { // Down arrow
                        if (current_selection < MENU_SIZE - 1) {
                            current_selection++;
                            display_menu(); // Update the menu display
                        }
                    } else {
                        buffer[index++] = translated; // Store the character in the buffer
                        if (index >= 256) { // Prevent buffer overflow
                            index = 255; // Keep index within bounds
                        }
                        buffer[index] = '\0'; // Null-terminate the string for display
                        alk_printf_legacy(buffer, 24); // Print the accumulated string
                    }
                }
            }
        } while (1); // Infinite loop to keep reading characters
    }
}




void selectionmenu()
{
    display_menu();
    handle_input();
}