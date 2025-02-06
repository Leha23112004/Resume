#define _XOPEN_SOURCE 500
#define _DEFAULT_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ftw.h>

#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/limits.h>
#include <dirent.h>


#include <stddef.h>
#include <sys/types.h>

#include <dlfcn.h>


#include "plugin_api.h"


char* DEBUG;

//0 - логическое И, 1 - логическое ИЛИ
int mode_and_or = 0;


//Логическое НЕ
int mode_not = 0;


//Структура плагина
struct plugin_helper {
    void* plugin_so_lib;

    char* plugin_name;

    int (*plugin_process_file)(const char *fname, struct option in_opts[],size_t in_opts_len);

    struct plugin_info* p_info; 

    struct option* taken_opts;

    int taken_opts_counter;
};

//Список найденных плагинов
struct plugin_helper* found_plugins;
//Счётчик найденных плагинов
int found_counter = 0;


//Список всех опций от всех плагинов
struct option* defined_opts;
//Счётчик этих опций
int defined_opts_counter = 0;

//Путь до плагинов
char* plugins_dir_path;


struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};

static int app_long_options_len = sizeof(long_options) / sizeof(long_options[0]);

const char* short_options = "P:AONvh";



void free_lists();

void print_help();


int check_plugins_compatibility();

int load_plugins(char* plugins_path);


int filter_function(const char* fpath, const struct stat* sb, int typeflag, struct FTW* ftwbuf);


int main(int argc, char* argv[]){
    DEBUG = getenv("LAB1DEBUG");

    char plugins_dir_path[PATH_MAX];
    char* r = getcwd(plugins_dir_path, PATH_MAX);

    r = r;

    for(int i = 0; i < argc; i++){
        if(argv[i][0] == '-' && argv[i][1] == 'P' && i + 1 < argc && argv[i + 1][0] != '-'){
            strcpy(plugins_dir_path, argv[i + 1]);
            break;
        }
    }

    if(DEBUG){
        fprintf(stderr, "Dir with plugins:\n\t%s\n", plugins_dir_path);
    }

    int l = load_plugins(plugins_dir_path);

    if(l != 0){
        fprintf(stderr, "Error occured while importing plugins");
        free_lists();
        exit(EXIT_FAILURE);
    } 

    defined_opts = realloc(defined_opts, sizeof(struct option) * (defined_opts_counter + app_long_options_len));

    //Для работы с опциями плагинов в getopt_long
    //Необходимо установить идентификаторы опциям
    int id = app_long_options_len + 1;

    for(int i = 0; i < defined_opts_counter; i++){
        defined_opts[i].val = id + i;
    }

    id = app_long_options_len + 1;

    for(int i = 0; i < found_counter; i++){
        for(int j = 0; j < (int)found_plugins[i].p_info->sup_opts_len; j++){
            found_plugins[i].p_info->sup_opts[j].opt.val = id;
            id++;
        }
    }

    defined_opts[defined_opts_counter] = long_options[0];
    defined_opts[defined_opts_counter + 1] = long_options[1];
    defined_opts[defined_opts_counter + 2] = long_options[2];

    int opt;

	int option_index = -1;


    while((opt = getopt_long(argc, argv, short_options, defined_opts, &option_index)) != -1){

		switch (opt)
		{
		case 'h':
			print_help();
            free_lists();
			return 0;
		case 'v':
			printf("lab12araN3250 version 1.\nAhrarov Ali Rustamovich N3250\n18-th variant\n\n");
            free_lists();
			return 0;
        case 'O':
            mode_and_or = 1;
            break;
        case 'N':
            mode_not = 1;
            break;
        case 'A':
            mode_and_or = 0;
            break;
		}


        int found = 0;
        for(int i = 0; i < found_counter; i++){
            for(int j = 0; j < (int)found_plugins[i].p_info->sup_opts_len; j++){
                struct option expected_option = found_plugins[i].p_info->sup_opts[j].opt;
                int expected_option_value = expected_option.val;
                if(expected_option_value == opt){

                    if(expected_option.has_arg > 0){
                        if(!optarg){
                            fprintf(stderr, "Option %s requires an argument\n", expected_option.name);
                            free_lists();
                            exit(EXIT_FAILURE);
                        }

                        expected_option.flag = (int*)optarg;
                    }

                    if(found_plugins[i].taken_opts_counter == 0){
                        found_plugins[i].taken_opts = malloc(sizeof(struct option));
                    }else{
                        found_plugins[i].taken_opts = realloc(found_plugins[i].taken_opts, sizeof(struct option) * (found_plugins[i].taken_opts_counter + 1));
                    }

                    memcpy(&found_plugins[i].taken_opts[found_plugins[i].taken_opts_counter], &expected_option, sizeof(struct option));

                    found_plugins[i].taken_opts_counter++;

                    found = 1;

                    break;
                }

                if(found){
                    break;
                }
            }
        }
        

        if(opt == '?'){
            fprintf(stderr, "Bad optinon\n\n");
            print_help();
            free_lists();
            exit(EXIT_FAILURE);
        }

		option_index = -1;
	}
    
    char* path = argv[optind];

    if(!path){
        print_help();
        free_lists();
        return 0;
    }

    nftw(path, filter_function, 5, FTW_DEPTH | FTW_PHYS);

    free_lists();

    return 0;
}



void free_lists(){
    for(int i = 0; i < found_counter; i++){
        dlclose(found_plugins[i].plugin_so_lib);

        if(found_plugins[i].taken_opts_counter > 0){
            free(found_plugins[i].taken_opts);
        }

        free(found_plugins[i].plugin_name);     

        free(found_plugins[i].p_info);
    }

    if(found_counter > 0){
        free(found_plugins);
    }

    if(defined_opts_counter > 0){
        free(defined_opts);
    }
}


void print_help(){
    printf("Usage: ./lab12araN3250 [options...] <dir_path>\n\t%s: %s\n\t%s: %s\n\t%s: %s\n\t%s: %s\n\t%s: %s\n\t%s: %s\n", 
        "-h, --help",
        "Get help for commands",
        "-v, --version",
        "Show version number and app information",
        "-A",
        "Use AND (by default)",
        "-O",
        "Use OR",
        "-N",
        "Use NOT",
        "-P <dir>",
        "Set folder with plugins to use"
    );

    
    printf("Imported options:\n");

    for(int i = 0; i < found_counter; i++){
        struct plugin_helper plugin = found_plugins[i];
        struct plugin_info* ppi = plugin.p_info;

        printf("\t(%d) Name: %s\n", (i + 1), plugin.plugin_name);

        for(int j = 0; j < (int)ppi->sup_opts_len; j++){
            struct option opt = ppi->sup_opts[j].opt;
            const char* description = ppi->sup_opts[j].opt_descr;

            printf("\t\t(%d) %s - %s\n", (j + 1), opt.name, description);
            if(opt.has_arg){
                printf("\t\tReqires an argument\n");
            }
        }
    }
}

int load_plugins(char* plugins_path){
    DIR* d;
    struct dirent* dir;

    d = opendir(plugins_path);

    if(!d){
        fprintf(stderr, "Impossible to open directory with plugins:\n\t%s\nSTOP\n", plugins_path);
        free_lists();
        return -1;
    }

    char buf[PATH_MAX];

    while((dir = readdir(d)) != NULL){

        if(dir->d_type == DT_REG){

            int dname_len = strlen(dir->d_name);

            //Действительно ли файл - плагин?
            if(dname_len < 3 || dir->d_name[dname_len-3] != '.' || dir->d_name[dname_len-2] != 's' || dir->d_name[dname_len-1] != 'o'){
                continue;
            }

            //Собираем путь до плагина
            strcpy(buf, plugins_path);
            if(buf[strlen(buf) - 1] != '/'){
                strcat(buf, "/");
            }
            strcat(buf, dir->d_name);

            void* lib = dlopen(buf, RTLD_NOW);

            if(lib){
                //Если действительно...
                if(!found_plugins){
                    found_plugins = malloc(sizeof(struct plugin_helper));
                }
                else {
                    found_plugins = realloc(found_plugins, sizeof(struct plugin_helper) * (found_counter + 1));
                }


                int (*plugin_info)(struct plugin_info*);
                plugin_info = dlsym(lib, "plugin_get_info");

                int (*plugin_process_file)(const char *fname, struct option in_opts[],size_t in_opts_len);
                plugin_process_file = dlsym(lib, "plugin_process_file");

                struct plugin_helper temp;
                
                temp.p_info = malloc(sizeof(struct plugin_info));

                (*plugin_info)(temp.p_info);

                temp.plugin_so_lib = lib;

                temp.plugin_process_file = plugin_process_file;

                temp.taken_opts_counter = 0;

                temp.taken_opts = NULL;

                //Копируем плагин в массив плагинов
                memcpy(&found_plugins[found_counter], &temp, sizeof(struct plugin_helper));

                found_plugins[found_counter].plugin_name = malloc(sizeof(char) * (strlen(dir->d_name) + 1));
                strcpy(found_plugins[found_counter].plugin_name, dir->d_name);
                
                found_counter++;

                if(DEBUG){
                    fprintf(stderr, "(%d) Imported plugin:\n\t%s\n", found_counter, dir->d_name);
                }
            }
        }
    }

    closedir(d);

    if(check_plugins_compatibility() == -1){
        return -1;
    }

    for(int i = 0; i < found_counter; i++){
        struct plugin_info* plugin_info = found_plugins[i].p_info;
        if(!defined_opts){
            defined_opts = malloc(sizeof(struct option) * plugin_info->sup_opts_len);
        }
        else{
            defined_opts = realloc(defined_opts, sizeof(struct option) * (defined_opts_counter + plugin_info->sup_opts_len));
        }

        for(int j = 0; j < (int)plugin_info->sup_opts_len; j++){
            defined_opts[defined_opts_counter + j] = plugin_info->sup_opts[j].opt;
        }

        defined_opts_counter += plugin_info->sup_opts_len;
    }

    return 0;
}

int check_plugins_compatibility(){
    for(int i = 0; i < found_counter; i++){
        struct plugin_info* plugin_info_1 = found_plugins[i].p_info;

        for(int j = i + 1; j < found_counter; j++){
            struct plugin_info* plugin_info_2 = found_plugins[j].p_info;

            for(int k = 0; k < (int)plugin_info_1->sup_opts_len; k++){
                for(int s = 0; s < (int)plugin_info_2->sup_opts_len; s++){
                    if(strcmp(plugin_info_1->sup_opts[k].opt.name, plugin_info_2->sup_opts[s].opt.name) == 0){
                        fprintf(stderr, "Error! Two plugins have the same options:\n");

                        fprintf(stderr, "%s\n", found_plugins[i].plugin_name);
                        fprintf(stderr, "%s\n", found_plugins[j].plugin_name);

                        fprintf(stderr, "Remove one and restart\n");

                        free_lists();

                        return -1;
                    }
                }
            }
        }
    }

    return 0;
}

int filter_function(const char* fpath, const struct stat* sb, int typeflag, struct FTW* ftwbuf) {
	fpath = fpath;
	sb = sb;
	typeflag = typeflag;
	ftwbuf = ftwbuf;

    if(typeflag == FTW_F){
        
        int suits = !mode_and_or;

        int (*plugin_process_file)(const char *fname, struct option in_opts[],size_t in_opts_len);

        for(int i = 0; i < found_counter; i++){
            if(found_plugins[i].taken_opts_counter > 0){
                plugin_process_file = found_plugins[i].plugin_process_file;

                int r = (*plugin_process_file)(fpath, found_plugins[i].taken_opts, found_plugins[i].taken_opts_counter);

                if(r < 0){
                    fprintf(stderr, "Error in plugin %s\nStop app working\n", found_plugins[i].plugin_name);
                    return -1;
                }

                if(DEBUG){
                    fprintf(stderr, "Result for file:\n\t%s\nPlugin\n\t%s = %d\n", fpath, found_plugins[i].plugin_name, r);
                }

                if(mode_and_or == 0){
                    suits &= (!r && !mode_not) || (r && mode_not);
                    if(!suits){
                        break;
                    }
                }
                else{
                    suits |= (!r && !mode_not) || (r && mode_not); 
                }
            }
        }

        if(suits){
            printf("Suitable file:\n\t%s\n", fpath);
        } 
    }

	return 0;
}