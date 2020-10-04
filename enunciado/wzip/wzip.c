#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define LINE_SIZE 5000
#define END_STR '\0'
#define NEW_LINE '\n'


typedef struct{
	char *name;
	int n_lines;
	char **content;
}FileTxt;

int count_char_ocurrences(char ch, int start, char *str);
int get_n_lines_file(char *fname);
FileTxt **get_files(int n_files, int start_file, char *files[]);
int get_max_lines(int n_files, FileTxt **files);
int get_priority_index(int start, int n_elements, int *index_arr, char **lines);
void compress_files(int n_files, int start_file, char *files[]);

int main(int argc, char *argv[]) {
	if(argc < 2){
		printf("wzip: file1 [file2 ...]\n");
		exit(1);
	}
	int n_files = argc-1;
	compress_files(n_files, 1, argv);
    return(0);
}

int count_char_ocurrences(char ch, int start, char *str){
	int counter = 0;
	for(int i = start;; i++){
		if(ch == str[i]) counter++;
		else break;
	}
	return counter;
}

int get_n_lines_file(char *fname){
	int counter = 0;
	FILE *r_file;
	r_file = fopen(fname, "r");
	char str[LINE_SIZE];
	while(fgets(str, LINE_SIZE, r_file) != NULL) counter++;
	fclose(r_file);
	return counter;
}

char **read_file(int n_lines, char *file_name){
	char *str = (char*)malloc(LINE_SIZE*sizeof(char));
	FILE *r_file;
	int j = 0;
	r_file = fopen(file_name, "r");
	char **file_info;
	file_info = (char**)malloc(n_lines*sizeof(char*));
	while(fgets(str, LINE_SIZE, r_file) != NULL){
		file_info[j] = str;
		str = (char*)malloc(LINE_SIZE*sizeof(char));
		j++;
	}
	fclose(r_file);
	return file_info;
}

FileTxt **get_files(int n_files, int start_file, char *files[]){
	FileTxt **files_txt = (FileTxt**)malloc(n_files*sizeof(FileTxt*));
	for(int i = start_file; i <= n_files; i++){
		FileTxt *file_i = (FileTxt*)malloc(sizeof(FileTxt));
		file_i -> name = files[i];
		file_i -> n_lines = get_n_lines_file(files[i]);
		file_i -> content = read_file(file_i -> n_lines, files[i]);
		files_txt[i-1] = file_i;
	}
	return files_txt;
}

int get_max_lines(int n_files, FileTxt **files){
	int max_lines = 0;
	FileTxt *file_i;
	for(int i = 0; i < n_files; i++){
		file_i = files[i];
		if(file_i -> n_lines > max_lines) max_lines = file_i -> n_lines;
	}
	return max_lines;
}

int get_priority_index(int start, int n_lines, int *index_arr, char **lines){
	char *line;
	int index = start; 
	for(;index < n_lines; index++){
		line = lines[index];
		if(line[index_arr[index]] != END_STR) break;
	}
	return index;
}
void compress_files(int n_files, int start_file, char *files[]){
	if(n_files > 0){
	FileTxt **files_txt = get_files(n_files, start_file, files);
	FileTxt *file_i;
	char end_of_sequence[] = {END_STR};
	char new_line = NEW_LINE;
	int continue_count = 0;
	int max_lines = get_max_lines(n_files, files_txt);
	int *index_arr;
    char **lines;
	int priority_index;
	char letter;
	int n_ocurrences;
	int total_ocurrences = 0;
	for(int line = 0; line < max_lines; line ++){
		index_arr = (int*)malloc(n_files*sizeof(int));
		lines = (char **)malloc(n_files*sizeof(char*));
		for(int i = 0; i < n_files; i++){
			file_i = files_txt[i];
			index_arr[i] = 0;
			if(line + 1 <=  file_i -> n_lines) lines[i] = file_i -> content[line];
			else lines[i] = end_of_sequence;
		}
		priority_index = get_priority_index(0, n_files, index_arr, lines);
		letter = lines[priority_index][index_arr[priority_index]];
		if(letter != '\n'){
			continue_count = 0;
			if(total_ocurrences){
				fwrite(&total_ocurrences, 1, sizeof(int) , stdout);
			    fwrite(&new_line, 1, sizeof(char) , stdout);
			}
		}else continue_count = 1;
		while(priority_index < n_files){
			letter = lines[priority_index][index_arr[priority_index]];
			if (!continue_count) total_ocurrences = 0;
			for(int file_k = priority_index;  file_k < n_files; file_k++){
				n_ocurrences = count_char_ocurrences(letter, index_arr[file_k], lines[file_k]);
				index_arr[file_k] += n_ocurrences;
				total_ocurrences += n_ocurrences;
			 }
			 if(letter != '\n' || line == max_lines-1){
				 fwrite(&total_ocurrences, 1, sizeof(int) , stdout);
				 fwrite(&letter, 1, sizeof(char) , stdout);
			}
			priority_index = get_priority_index(priority_index, n_files, index_arr, lines);	

		}
		free(index_arr);
		free(lines);
	}
	free(files_txt);
	}
}

