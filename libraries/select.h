#include "aurora_utils.h"
#include "keyboard.h"
typedef struct {
    const char *text;
} MenuItem;
int screen_width=60;
int screen_height=24;
MenuItem menu[3] = {
    {"Option 1"},
    {"Option 2"},
    {"Option 3"}
};

int current_selection = 0;

int MENU_SIZE=3;
unsigned char OLDVGAC;
void display_menu() {
    alk_clear_screen();
    
    const int menu_height = MENU_SIZE;
    const int start_row = (screen_height - menu_height) / 2; // Center vertically

    for (int i = 0; i < MENU_SIZE; i++) {
        char buffer[80]; // Buffer to construct the display string
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

        int start_col = (screen_width - pos) / 2; // Center horizontally

        // Construct the full line with padding for centering
        char line[screen_width + 1];
        for (int j = 0; j < start_col; j++) {
            line[j] = ' ';
        }
        for (int j = 0; j < pos; j++) {
            line[start_col + j] = buffer[j];
        }
        for (int j = start_col + pos; j < screen_width; j++) {
            line[j] = ' ';
        }
        line[screen_width] = '\0'; // Null-terminate the line
        OLDVGAC=VGAC;
        VGAC=0xF0;
        alk_printf_legacy(line, start_row + i);
        VGAC=OLDVGAC;
    }
}


int handle_input(int fix) {
    char buffer[256];
    int index = 2;
    buffer[0] = '>';
    buffer[1] = ' ';

    init_pics(0x20, 0x28); // Initialize the PICs once before the loop
    display_menu(); // Display the menu initially
    int displayedmenu=0;

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
                            if (fix==1){
                                if (displayedmenu==0) {
                                displayedmenu=1;
                                } else if (displayedmenu==1){
                                    alk_clear_screen();
                                    return current_selection+1;
                                }
                            } else {
                                alk_clear_screen();
                                return current_selection+1;
                            }
                            
                            
                        }

                        index = 2; // Reset the buffer index for the next input
                        buffer[0] = '>';
                        buffer[1] = ' ';
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
                    }
                }
            }
        } while (1); // Infinite loop to keep reading characters
    }
}



