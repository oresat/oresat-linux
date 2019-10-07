#include <stdio.h>
#include <stdint.h>

#ifndef FILE_RECEIVE_FOLDER 
#define FILE_RECEIVE_FOLDER "/tmp/received_files/"
#endif


int main() {
    int8_t *test = "this is a test";
    char filePath[] = FILE_RECEIVE_FOLDER;

    strcat(filePath, test);

    printf("%s\n", filePath);
    FILE *f = fopen(odFileBuffer->fileName, "wb");
    if(f != NULL) {
        uint32_t temp = fwrite(test, sizeof(char), , f);
        fclose(f);

        printf("%d\n", temp);
    }

    return 1;
}
