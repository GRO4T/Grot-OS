void memory_copy(char * source, char * dest, int no_bytes){
    for (int i = 0; i < no_bytes; ++i){
        *(dest + i) = *(source + i);
    }
}

void memory_copy_int(int * source, int * dest, int no_bytes){
    for (int i = 0; i < no_bytes; ++i){
        *(dest + i) = *(source + i);
    }
}
