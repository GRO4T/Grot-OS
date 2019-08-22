/*  

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
    for (int i = 0; i < 25; ++i){
        print_str("\nHello World", NULL);
    }
}
 */
/* 
#include "../drivers/screen.h"

#define NULL 0
#define DELAY 100000000

//prepares string filled with characters corresponding to line number
char * make_test_row(char * line, int n, int line_number){
    for (int i = 0; i < n - 1; ++i){
        line[i] = i + '0';
    }
    line[n] = '\n';
}

void main () {
    clear_screen();
    
    char line[MAX_COLS+1];
    for (int i = 0; i < 30; ++i){
        make_test_row(line, MAX_COLS+1, i);
        print_str(line, NULL);
        //apply some delay
        for (int j = 0; j < DELAY; ++j){
            int x = 2;
        }
    }
}
*/
#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
//#include "../test/screen_unit_test.h"

#define NULL 0

void main () {/*  
    // Create a pointer to a char , and point it to the first text cell of
    // video memory ( i . e . the top - left of the screen )
    char * video_memory = (char*) 0xb8000;
    // At the address pointed to by video_memory , store the character ’X ’
    // ( i . e . display ’X ’ in the top - left of the screen ).
    *video_memory = 'X';
   
    clear_screen();

    int i;
    for (i = 0; i < 27; ++i){
        char str[81];
        for (int j = 0; j < 80; ++j){
            str[j] = i + '0';
        }
        str[80] = '\0';
        print_str(str, NULL);
        int x = 100000000;
        for (int j = 0; j < 10000000; ++j){
            int x =2;
        }
    }

    */
    clear_screen();
    kprint("fslj");
    isr_install();
    /* Test the interrupts */
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
}
