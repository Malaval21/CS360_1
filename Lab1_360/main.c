#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
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

void init(void);
int findCmd(char *command);
char*GetDirname(char*pathname);
char*GetBasename(char*pathname);


void init(void) {
	char r[10] = "/";
	root = (Node *)malloc(sizeof(Node));
	root->type = 'D';
	strcpy(root->name, r);
	root->sibling = 0;
	root->child = 0;
	root->parent = 0;
	cwd = root;
}

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
char*GetDirname(char*pathname)
{
	char * pch;
	char s[128];
	int num;
	pch = strrchr(pathname, '/');// last occurence of / char

	num = pch - pathname;
	if (num != 0)
	{
		strncpy(s, pathname, num);
		strcat(s, "\0");
		return s;
	}
	return "/";
}
char*GetBasename(char*pathname)
{
	char *ssc;
	int l = 0;
	ssc = strstr(pathname, "/");
	do {
		l = strlen(ssc) + 1;
		pathname = &pathname[strlen(pathname) - l + 2];
		ssc = strstr(pathname, "/");
	} while (ssc);
	return pathname;
}
bool checkSiblingsUpdateCwd(Node* node, char*token)
{
	//bool found = false;
	Node* temp = node;
	while (temp)
	{
		if ((strcmp(temp->name, token) == 0) && (temp->type == 'D')) // is the directory we are looking for
		{
			cwd = temp;
			return true;
		}
		temp = temp->sibling;
	}
	return false;
}
bool checkChildUpdateCwd(char*token)//check starting from cwd
{
	if ((strcmp(cwd->child->name, token) == 0) && (cwd->child->type == 'D'))
	{
		cwd = cwd->child;
		return true;
	}
	else
	{
		return checkSiblingsUpdateCwd(cwd, token);
	}
}
bool pathToNode(char*dirname, char*basename)//return true if path exists , else false
{
	char *s;
	s = strtok(dirname, "/");
	if (dirname[0] == '/')//absolute pathname
	{
		
		cwd = root;
	}
	if (s) // tokens in path name
	{
		if ((cwd->child)&&(checkChildUpdateCwd(s) == false)) // assure that first directory past root exists
		{
			return false;
		}								//tokenize the string again

		while (s = strtok(0, "/")) // keep calling strtok() with NULL string until it return 0 pointer
		{
			if ((cwd->child)&&(checkChildUpdateCwd(s) == false)) // assure that first directory past root exists
			{
				return false;
			} // check children and siblings , have to be dir too
		}
		//cwd should be in the right place right now

		// check if baby already there
		if((cwd->child)&&(checkChildUpdateCwd(basename)))
		{
			return false;
		}
		//no such directory exists
	}
	return true;


}

int mkdir(char *pathname)
{

	char temp[128];
	char dname[64], bname[32];
	Node* newNode= (Node *)malloc(sizeof(Node)); ;
	Node*t;

	strcpy(temp, pathname);
	strcpy(dname, GetDirname(temp));   // dname="/a/b"

	strcpy(temp, pathname);
	strcpy(bname, GetBasename(temp));   // bname="c"

	if (pathToNode(dname, bname) == true)
	{
		// insert the node 
		
		newNode->child = 0;
		newNode->parent = cwd;
		newNode->sibling = cwd->child;
		newNode->type = 'D';
		strcpy(newNode->name, bname);
		cwd->child = newNode;
		

	}
	return 0;

}

int rmdir(char *pathname)
{

}
int ls(char *pathname)
{
	
	Node*t;	

	if (pathToNode(pathname, "") == true)
	{		
		t = cwd->child;

		while (t)
		{
			printf("%c %s\n", t->type, t->name);
			t = t->sibling;
		}
	}
	return 0;

}
int cd(char *pathname)
{
	if (pathToNode(pathname,"") == true)
	{
		printf("cd successful\n");
	}
	else
	{
		printf("cd unsuccessful\n");
	}
	return 0;
}
int pwd(Node*cwd)
{
	Node*t = cwd;
	if (t)
	{
		pwd(t->parent);
		if ((t->parent) && (strcmp(t->parent->name,"/")))
		{
			printf("%s", t->name);
		}
		else
		{
			printf("/%s", t->name);
		}
	}
	
}
int creat(char *pathname)
{
	char temp[128];
	char dname[64], bname[32];
	Node* newNode = (Node *)malloc(sizeof(Node)); ;
	Node*t;

	strcpy(temp, pathname);
	strcpy(dname, GetDirname(temp));   // dname="/a/b"

	strcpy(temp, pathname);
	strcpy(bname, GetBasename(temp));   // bname="c"

	if (pathToNode(dname, bname) == true)
	{
		// insert the node 

		newNode->child = 0;
		newNode->parent = cwd;
		newNode->sibling = cwd->child;
		newNode->type = 'F';
		strcpy(newNode->name, bname);
		cwd->child = newNode;


	}
	return 0;
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
int quit()
{

}

int main(void)
{
	int index, r;
	char s[128];
	init();

	while (1)
	{
		printf("Input a command : ");
		fgets(s, 128, stdin); // input at most 128 chars BUT has \n at end
		s[strlen(s) - 1] = 0;

		sscanf(s, "%s %s", command, pathname);
		index = findCmd(command);
		switch (index) {
		case 0: mkdir(pathname);    break;
		case 1: rmdir(pathname);    break;
		case 2: ls(pathname);       break;
		case 3: cd(pathname);       break;
		case 4: pwd(cwd);           break;
		case 5: creat(pathname);    break;
		case 6: rm(pathname);		break;
		case 7: reload(pathname);	break;
		case 8: save(pathname);		break;
		case 9: quit();				break;
		}
	}
	//dirName("ab/cde/fg.out");
	//getchar();
	return 0;
}
