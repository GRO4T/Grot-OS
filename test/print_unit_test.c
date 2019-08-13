#include "../drivers/screen.h"

#define NULL 0
#define DELAY 100000000

//prepares string filled with characters corresponding to line number
char * make_test_row(char * line, int n, int line_number){
    for (int i = 0; i < n - 1; ++i){
        line[i] = i + '0';
    }
    line[n] = '\0';
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
