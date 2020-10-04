#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void unzip_files(int n_files, char** file_name_arr);
int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("wunzip: file1 [file2 ...]\n");
		return 1;
    }
	unzip_files(argc-1, argv);
    return(0);
}

void unzip_files(int n_files, char** file_name_arr){
	char *file_name;
	FILE *r_file;
	char ch;
	int occurences;
	for(int i = 1; i <= n_files; i++){
		file_name = file_name_arr[i];
		r_file = fopen(file_name, "r");
		while(fread(&occurences, sizeof(int), 1, r_file)){
			fread(&ch, sizeof(char), 1, r_file);
			while(occurences--){
				printf("%c", ch);
			}
		}
		fclose(r_file);
	}
}