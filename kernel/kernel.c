#include "../drivers/screen.h"

#define NULL 0

void main () {
    // Create a pointer to a char , and point it to the first text cell of
    // video memory ( i . e . the top - left of the screen )
    char * video_memory = (char*) 0xb8000;
    // At the address pointed to by video_memory , store the character ’X ’
    // ( i . e . display ’X ’ in the top - left of the screen ).
    *video_memory = 'X';
   
    clear_screen();
    for (int i = 0; i < 20; ++i){
        print_str("\nHello World", -1, -1, NULL);
    }
}
