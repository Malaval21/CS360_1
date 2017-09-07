#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)


typedef struct node {
	char  name[64];
	char type;
	struct node* child;
	struct node* parent;
	struct node* sibling;
} Node;

Node *root, *cwd;                             /* root and CWD pointers */
char line[128];                               /* user input line */
char command[64], pathname[64];               /* user inputs */
char dirname[64], basename[64];               /* string holders */
char *cmd[] = { "mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
"reload", "save", "menu", "quit", 0 };
//int(*fptr[])(char *) = { (int(*)())mkdir,rmdir,ls, cd, pwd, creat, rm , reload , save,menu,quit };

int findCmd(char *command) //helps you parse the cmd list for the matching command name
{
	int i = 0;
	while (cmd[i]) {
		if (strcmp(command, cmd[i]) == 0)
			return i;
		i++;
	}
	return -1;
}
void initialize() // initialize root node / and set root, cwd to /
{
	root->name[0] = '/';
	root->child = 0;
	root->parent = &root;
	root->sibling = &root;
	root->type = 'D';
	cwd = root;
}
char*dirName(char*pathname)
{
	//char s[128];
	char *ssc;
	int l = 0;
	ssc = strstr(pathname, "/");
	do {
		l = strlen(ssc) + 1;
		pathname = &pathname[strlen(pathname) - l + 2];
		ssc = strstr(pathname, "/");
	} while (ssc);
	printf("%s\n", pathname);
}
char*bName(char*pathName)
{

}
int mkdir(char *pathname)
{
	char temp[128];
	char dname[64], bname[32];

	strcpy(temp, pathname);	

}

int rmdir(char *pathname)
{

}
int ls(char *pathname)
{

}
int cd(char *pathname)
{

}
int pwd(char *pathname)
{

}
int creat(char *pathname)
{

}
int rm(char *pathname)
{

}
int reload(char *pathname)
{

}
int save(char *pathname)
{

}
int menu(char *pathname)
{

}
int quit(char *pathname)
{

}

int main(void)
{
	/*int index, r;
	char s[128];
	initialize();
	while (1)
	{
		printf("Input a command : ");
		fgets(s[128], 128, stdin); // input at most 128 chars BUT has \n at end
		s[strlen(s) - 1] = 0;
	}
	sscanf(s, "%s %s", command, pathname);
	index = findCmd(command);
	r = fptr[index](pathname);*/

	dirName("ab/cde/fg.out");
	getchar();
	return 0;
}
