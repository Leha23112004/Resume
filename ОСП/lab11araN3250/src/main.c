#define _XOPEN_SOURCE 500
#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdint.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>


char* debug;

char* desired_sequence;


struct option long_opts[] = {
	{"help", no_argument, NULL, 'h'},
	{"version", no_argument, NULL, 'v'}
};

const char* opts = "hv";


void check_file(char* file){
	FILE* fp;
	size_t len = 0;
	char* line = NULL;
	ssize_t read;

	fp = fopen(file, "r");

	if(!fp){
		fprintf(stderr, "Bad file: %s\n", file);
		return;
	}

	int found = 0;

	while((read = getline(&line, &len, fp)) != -1){
		if(debug){
			fprintf(stderr, "Read line:\n\t%s\n", line);
		}

		found = strstr(line, desired_sequence) != NULL ? 1 : 0;

		if(found){
			break;
		}
	}

	if(found){
		printf("Found filter string in file:\n\t%s\n\t\t%s\n", file, line);
	}

	if(line){
		free(line);
	}

	fclose(fp);
}


void explore(char* current_dir){
    struct dirent* dir;
	DIR* d;

    d = opendir(current_dir);

	if(!d){
		fprintf(stderr, "Bad dir: %s\n", current_dir);
		return;
	}

	char buf[PATH_MAX];

	while((dir = readdir(d)) != NULL){

		//Получаем полный путь к файлу
		for(int i = 0; i < PATH_MAX; i++){
			buf[i] = '\0';
		}

		strcat(buf, current_dir);
		if(buf[strlen(buf) - 1] != '/'){
			strcat(buf, "/");
		}
		strcat(buf, dir->d_name);


		if(debug){
			fprintf(stderr, "Current dir:\n\t%s\n", buf);
		}

		if(dir->d_type == DT_REG){
			check_file(buf);
		}
		else if(dir->d_type == DT_DIR && dir->d_name[0] != '.'){
			explore(buf);
		}
	}

	closedir(d);
}


int main(int argc, char* argv[]){

	debug = getenv("LAB11DEBUG");

	int opt;

	int option_ind = -1;

	while((opt = getopt_long(argc, argv, opts, long_opts, &option_ind)) != -1){

		switch (opt)
		{
		case 'v':
			printf("lab11araN3250 version 1.\nAhrarov Ali Rustamovich N3250\n2-nd variant\n");
			return 0;
		case 'h':
			printf("Usage: ./lab11araN3250 [options...] <dir path> <bytes> \nOptions:\n%s: %s\n%s: %s\n", 
				"-h, --help",
				"Get help for commands",
				"-v, --version",
				"Show info"
			);
			return 0;
		}

		option_ind = -1;
	}


	if(argc < 3){
		fprintf(stderr, "%s\n%s\n", "Few arguments", "Usage: ./lab11araN3250 [options...] <dir_path> <bytes>");

		exit(EXIT_FAILURE);
	}

	char* bytes = argv[2];

	int len = strlen(bytes);

	if(len < 4 || bytes[0] != '0' || bytes[1] != 'x'){
		fprintf(stderr, "Mimimal byte sequency looks like 0xhh\n");
		exit(EXIT_FAILURE);
	}

	//Количество сивмолов в последовательности обязательно чётное
	if(len % 2 != 0){
		fprintf(stderr, "Byte sequency has to have even count of bytes\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 2; i < len; i++){
		if(!(('a' <= bytes[i] && bytes[i] <= 'f') || ('A' <= bytes[i] && bytes[i] <= 'F') || 
			('0' <= bytes[i] && bytes[i] <= '9'))){
			fprintf(stderr, "Byte sequency has to consist of hex numbers\n");
			exit(EXIT_FAILURE);
		}
	}

	//Преобразуем в строку
	char buf[3];

	desired_sequence = malloc(sizeof(char) * (len / 2 + 1));

	for(int i = 0; i < (int)sizeof(char) * (len / 2 + 1); i++){
		desired_sequence[i] = '\0';
	}
	
	for(int i = 2; i < len; i += 2){
		for(int j = 0; j < 2; j++){
			buf[j] = bytes[i + j];
		}
		buf[2] = '\0';

		desired_sequence[(i - 2) / 2] = (char)((int) strtol(buf, NULL, 16));
	}

	desired_sequence[len / 2] = '\0';

	if(debug){
		fprintf(stderr, "Got string:\n\t%s\n", desired_sequence);
	}

	char* current_dir = argv[1];

	explore(current_dir);

	free(desired_sequence);

	return 0;
}