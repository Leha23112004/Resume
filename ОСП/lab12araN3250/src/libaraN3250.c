#define _XOPEN_SOURCE 500
#define _DEFAULT_SOURCE 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <fcntl.h>

#include <stddef.h>
#include <stdint.h>

#include <ctype.h>


#include "plugin_api.h"

static char* g_lib_name = "libaraN3250";

static char* g_lib_purpose = "Find files with desired MAC address";

static char* g_lib_author = "Ahrarov Ali Rustamovich";

//Debug mode
char* DEBUG;

static struct plugin_option g_lib_options[] = {
    {
        {"mac-addr-bin", required_argument, NULL, 0},
        "Desired MAC address which is looked for in files"
    }
};

static int g_lib_options_len = sizeof(g_lib_options) / sizeof(g_lib_options[0]);


int plugin_get_info(struct plugin_info* ppi){
    if(!ppi){
        fprintf(stderr, "(%s) ppi was not initialized\n", g_lib_name);
        return -1;
    }

    ppi->plugin_author = g_lib_author;
    ppi->plugin_purpose = g_lib_purpose;
    ppi->sup_opts_len = g_lib_options_len;
    ppi->sup_opts = g_lib_options;

    return 0;
}



int is_mac(char* mac) {
    int i = 0;
    int s = 0;

    while (*mac) {
       if (isxdigit(*mac)) {
          i++;
       }
       else if (*mac == ':' || *mac == '-') {

          if (i == 0 || i / 2 - 1 != s)
            break;

          ++s;
       }
       else {
           s = -1;
       }


       ++mac;
    }

    return (i == 12 && (s == 5 || s == 0));
}



int plugin_process_file(const char *fname, struct option in_opts[], size_t in_opts_len){
    in_opts_len = in_opts_len;
    DEBUG = getenv("LAB1DEBUG");

    char* desired_mac = (char*)in_opts[0].flag;

    //Проверяем, правильно ли пользователь задал желаемое значение

    if(!is_mac(desired_mac)){
        fprintf(stderr, "%s. Incorrect format of input MAC address\n", g_lib_name);
        return -1;
    }
    

    if(DEBUG){
        fprintf(stderr, "%s. Analyzing file:\n\t%s\n", g_lib_name, fname);
    }

    //Загружаем файл в память и проверяем, есть ли там MAC адрес
    int fd = open(fname, O_RDWR);

	if(fd == -1){
		fprintf(stderr, "%s. Impossible to open file:\n\t%s\n\n", g_lib_name, fname);
		return 1;
	}

    struct stat* sb = malloc(sizeof(struct stat));
    int result = fstat(fd, sb);
    
    result = result;

	char* file_content = mmap(NULL, sb->st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if(file_content == MAP_FAILED){
		fprintf(stderr, "%s. Impossible to map file with the following path %s\n\n", g_lib_name, fname);
        free(sb);
		return 1;
	}

    //Длина MAC адреса = 17
    char substr[18];

    int found = 0;

    for(int i = 0; i < sb->st_size - 17; i++){
        
        for(int j = 0; j < 17; j++){
            substr[j] = file_content[i + j];
        }

        substr[17] = '\0';

        if(is_mac(substr) && strcmp(desired_mac, substr) == 0){
            found = 1;
            break;
        }
    }

    munmap(file_content, sb->st_size);

    free(sb);

    if(!found){
        return 1;
    }

    return 0;
}