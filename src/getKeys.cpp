#include <eccodes.h>
#include <string>
#include <iostream>
#define MAX_VAL_LEN  4096

int main(int argc, char* argv[]) {

    /* To skip read only and computed keys
     unsigned long key_iterator_filter_flags=CODES_KEYS_ITERATOR_SKIP_READ_ONLY |
                                             CODES_KEYS_ITERATOR_SKIP_COMPUTED;
     */
    unsigned long key_iterator_filter_flags = CODES_KEYS_ITERATOR_ALL_KEYS |
                                              CODES_KEYS_ITERATOR_SKIP_DUPLICATES;
 
    /* Choose a namespace. E.g. "ls", "time", "parameter", "geography", "statistics", null (for all keys)*/
    const char* name_space = NULL;    
    const char* path = "";

    // Get arguments
    if(argc > 1) { path       = argv[1]; }
    if(argc > 2) { name_space = argv[2]; }

    FILE* file = fopen(path,"r");

    if(!file) { 
        printf("Can not open file.\n");
        exit(1); 
    }

    codes_handle* handle;

    int error;
    int msg_count=0;
 
    char value[MAX_VAL_LEN];
    size_t vlen = MAX_VAL_LEN;
    
    // Iterate over all messages
    while((handle = codes_handle_new_from_file(0, file, PRODUCT_GRIB, &error)) != NULL) {

        codes_keys_iterator* kiter = NULL;
        
        // Before printing, so message start at n°1
        msg_count++;
        
        printf("\n-- MSG N°%d --\n",msg_count);

        // Get all keys filtered by flags and namespace
        kiter = codes_keys_iterator_new(handle, key_iterator_filter_flags, name_space);

        // Iterate over all keys
        while(codes_keys_iterator_next(kiter)) {
            
            // Get key name
            const char* name = codes_keys_iterator_get_name(kiter);

            // codedValue and values are array so we skip to not crash
            if(strcmp(name, "codedValues") == 0) { continue; }
            if(strcmp(name, "values")      == 0) { continue; }

            // Use like some kind of index, like an i in a for loop
            // So we need to reassign value after use
            vlen = MAX_VAL_LEN;

            // Reset array value
            memset(value, 0, vlen);

            // Get value associated to key name
            codes_get_string(handle, name, value, &vlen);
            printf("%s = %s\n",name,value);
 
            /* Alternative way of getting the string value */
            // This function does not use the key name
            // So we can not filter the key which we want or not
            // If we use name_space = NULL, the program will crash ..
            // .. trying to store an array as a char[]
            // CODES_CHECK(codes_keys_iterator_get_string(kiter, value, &vlen), 0);
        }
 
        codes_keys_iterator_delete(kiter);
        codes_handle_delete(handle);
    }

    fclose(file);
    printf("\n");
    return 0;
}