#include "screen.h"
#include "ports.h"

int get_cursor();
void set_cursor(int offset);
int get_screen_offset(int col, int row);
void clear_screen();

void print_str(char * str, int col, int row, char attribute_byte){
    int i = 0;
    while (str[i] != '\0'){
        print_char(str[i], col, row, attribute_byte);
        i++;
    }
}

/* Print a char on the screen at col , row , or at cursor position */
void print_char ( char character , int col , int row , char attribute_byte ) {
    /* Create a byte ( char ) pointer to the start of video memory */
    unsigned char * vidmem = (unsigned char *) VIDEO_ADDRESS;
    /* If attribute byte is zero , assume the default style . */
    if (!attribute_byte ) {
        attribute_byte = WHITE_ON_BLACK;
    }
    /* Get the video memory offset for the screen location */
    int offset ;
    /* If col and row are non - negative , use them for offset . */
    if ( col >= 0 && row >= 0) {
        offset = get_screen_offset(col, row);
        /* Otherwise , use the current cursor position . */
    } else {
        offset = get_cursor();
    }
    //If we see a newline character , set offset to the end of
    //    current row , so it will be advanced to the first col
    //    of the next row .
    if ( character == '\n') {
        int rows = offset / (2* MAX_COLS );
        offset = get_screen_offset(79, rows);
        // Otherwise , write the character and its attribute// video memory at our calculated offset .
    } else {
        vidmem [ offset ] = character ;
        vidmem [ offset + 1] = attribute_byte;
    }
        // Update the offset to the next character cell , which is
        // two bytes ahead of the current cell .
    offset += 2;
    // Make scrolling adjustment , for when we reach the bottom
    // of the screen .
    //TODO offset = handle_scrolling(offset);
    // Update the cursor position on the screen device .
    set_cursor(offset);
}
//gets cursor offset
int get_cursor(){
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; // High byte: << 8
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; //Position * size of character cell
}
//sets cursor offset
void set_cursor(int offset){
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}
// calculate character's offset in video memory
int get_screen_offset(int col, int row){
   return (row * MAX_COLS + col) * 2; // each character has 2 bytes (ASCII code, attributes)
}

void clear_screen(){
    int row = 0;
    int col = 0;

    for (row=0; row < MAX_ROWS; ++row){
        for (col=0; col < MAX_COLS; ++col){
            print_char(' ', col, row, WHITE_ON_BLACK);
        }
    }
    set_cursor(get_screen_offset(0, 0));
}
