/*
 * path.h
 *
 *      Author: Ceballos, Matias
 *      		Zimmel, Ezequiel
 */

#ifndef PATH_H_
#define PATH_H_

int get_path_entries(char* path[]);
void search_File_Dir(char* arch, char* paths[], char *execPath);
int read_input(char* argv[], char* cadena);
int check_Redirect(char* argv[], char fileName[]);
int check_Pipe(char* argv[], char* argv1[], char* argv2[]);
void exec_Pipeline(char* argv1[], char* argv2[], char* paths[]);

#endif /* PATH_H_ */
