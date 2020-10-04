#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_SIZE 100
void print_file_content(char *file_name, int line_size);
FILE *r_file;
int main(int argc, char *argv[])
{
    if (!argc) exit(1);
    for (int i=1; i<argc; i++) print_file_content(argv[i], LINE_SIZE);
    return 0;
}

void print_file_content(char *file_name, int line_size){
    r_file = fopen(file_name, "r");
    if(r_file == NULL){
        printf("wcat: cannot open file\n");
        exit(1);
    }
    char content[line_size];
    while(fgets(content, line_size, r_file) != NULL) printf("%s", content);
    fclose(r_file);
}
