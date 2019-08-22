#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"

/* Private functions declaration */
int get_cursor();
void set_cursor(int offset);
int get_screen_offset(int col, int row);
void clear_screen();
int handle_scrolling(int cursor_offset);
void print_char ( char character , int col , int row , char attribute_byte );


/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

/*  Prints string at the cursor position with default attributes (WHITE_ON_BLACK) */
void kprint(char * str){
    print_str(str, WHITE_ON_BLACK);
}

/* Prints string at the cursor position with given attributes */
void print_str(char * str, char attribute_byte){
    print_str_at(str, -1, -1, attribute_byte);
}

/* Prints string at the given position and with set attributes */
void print_str_at(char * str, int col, int row, char attribute_byte){
    /* Print error message when col or row exceeds screen boundaries */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        print_str_at("PRINT ERROR: position out of screen range", 0, MAX_ROWS-1, RED_ON_WHITE);
        return;
    }
    /* If possible, print first character at given coordinates, position of the next will be handled
     * by video memory */
    if (str[0] != '\0')
        print_char(str[0], col, row, attribute_byte);
    int i = 1;
    while (str[i] != '\0'){
        print_char(str[i], -1, -1, attribute_byte);
        i++;
    }
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


/**********************************************************
 * Private Kernel functions                               *
 **********************************************************/


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
    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return;
    }
    /* If col and row are non - negative , use them for offset . */
    else if ( col >= 0 && row >= 0) {
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
    offset = handle_scrolling(offset);
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


/*  Advance the text cursor, scrolling the video buffer if necessary.  */
int handle_scrolling(int cursor_offset){
    // If the cursor is within the screen, return it unmodified
    if (cursor_offset < MAX_ROWS*MAX_COLS*2){
        return cursor_offset;
    }

    /*  Shuffle the rows back one */
   int i;
   for (i = 1; i < MAX_ROWS; ++i){
       memory_copy((char*)(get_screen_offset(0, i) + VIDEO_ADDRESS),
               (char*)(get_screen_offset(0, i-1) + VIDEO_ADDRESS),
               MAX_COLS*2);
   }

   /* Blank the last line by setting all bytes to 0  */
   char * last_line = get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
   for (i=0; i < MAX_COLS*2; ++i){
        last_line[i] = 0;
   }

   // Move the offset back one row, such that it is now on the last
   // row, rather than off the edge of the screen
   cursor_offset -= 2*MAX_COLS;

   return cursor_offset;
}
