/*
 * builtin.h
 *
 *      Author: Ceballos, Matias
 *      		Zimmel, Ezequiel
 */

#ifndef BUILTIN_H_
#define BUILTIN_H_

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_pwd(char **args);
int lsh_num_builtins(void);

extern char *builtin_str[];

extern char *builtin_desc[];

extern int (*builtin_func[]) (char **);

#endif /* BUILTIN_H_ */
