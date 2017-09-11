#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

typedef struct node {
	char  name[64];
	char type;
	struct node* child;
	struct node* parent;
	struct node* sibling;
} Node;

Node *dir,*root, *cwd;                             /* root and CWD pointers */
char line[128];                               /* user input line */
char command[64], pathname[64];               /* user inputs */
char dname[128],bname[128];
char *cmd[] = { "mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
"reload", "save", "menu", "quit", 0 };

int init(void) 
{

	char r[10] = "/";
	root = (Node *)malloc(sizeof(Node));

	root->type = 'D';
	strcpy(root->name, r);
	root->sibling = 0;
	root->child = 0;
	root->parent = 0;
	cwd = root;

	return 1;
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


void clearTree(Node **curnode)
{
	if (*curnode)
	{
		// clear children
		clearTree(&(*curnode)->child);
		// clear siblings
		clearTree(&(*curnode)->sibling);
		//clear self
		free(*curnode);
		*curnode = NULL;
	}
}
int reinit()
{
	clearTree(&root);
	init();
	printf("Tree Reinitialized");

	return 1;
}

int menu()
{
	printf("Command List :\n");
	printf("mkdir\n");
	printf("rmdir\n");
	printf("ls\n");
	printf("cd\n");
	printf("pwd\n");
	printf("creat\n");
	printf("rm\n");
	printf("reload\n");
	printf("save\n");
	printf("menu\n");
	printf("quit\n");

	return 1;
}
int getDirectory()
{
	int found =0;
	Node*temp  = (Node *)malloc(sizeof(Node));
	char tempStr[128];

	strcpy(tempStr, pathname);
	strcpy(dname, dirname(tempStr));   // dname="/a/b"

	strcpy(tempStr, pathname);
	strcpy(bname, basename(tempStr));

	dir = (Node *)malloc(sizeof(Node));

	if(temp !=0)
	{
		if(dname[0] == '/')
		{
			temp = root;
		}
		else
		{
			temp = cwd;
		}
		//check if we need to go further than root
		if(strcmp(dname,"/")!=0)
		{
			char *s;
			s = strtok(dname, "/");
			temp = temp->child;
			while(temp)
			{
				if(strcmp(temp->name,s)==0)
				{
					break;
				}
				temp = temp ->sibling;
			}

			if(temp == 0)
			{
				printf("Directory not found!\n");
				return 0;
			}
			if (temp->type == 'F')
			{
				printf("Found a file, not a directory\n");
				return 0;
			}

			//while we still have more tokens
			while(s = strtok(0,"/"))
			{
				temp = temp->child;
				while(temp)
				{
					if(strcmp(temp->name,s)==0)
					{
						break;
					}
					temp = temp ->sibling;
				}

				if(temp == 0)
				{
					printf("Directory not found!\n");
					return 0;
				}
				if (temp->type == 'F')
				{
					printf("Found a file, not a directory\n");
					return 0;
				}
			}
		}
	}
	dir = temp;
	return 1;

}
int insertNode(char type)
{
	int found = getDirectory();
	Node*temp  = (Node *)malloc(sizeof(Node));
	Node* newNode= (Node *)malloc(sizeof(Node));
	if(found ==1)
	{
		temp = dir;
		if(temp->child !=0)
		{
			temp = temp-> child;
			//check if baby already exists
			while(temp)
			{
				if (strcmp(temp->name, bname) == 0)
				{
					printf("Baby already exists!\n");
					return 0;
				}
				else if (temp->sibling == NULL) 
				{
					break;
				}

				else // continue to search
				{
					temp = temp->sibling;
				}
			}

		}

		newNode->child = 0;
		newNode->parent = dir;
		newNode->sibling = 0;
		newNode->type = type;
		strcpy(newNode->name, bname);
		
		if(dir->child ==0)
		{
			dir->child = newNode;
		}
		else
		{
			temp->sibling = newNode;
		}
		if(type == 'F')
		{
			printf("File created\n");
		}
		else
		{
			printf("Directory created\n");
		}
		return 1;
		
	}

}
int removeNode(char type)
{
	int found =0;
	Node* tempFront = (Node *)malloc(sizeof(Node));
	Node* tempBack = (Node *)malloc(sizeof(Node));

	found = getDirectory();
	if(found)
	{
		tempFront = dir;
		if(tempFront->child)
		{
			tempBack = 0;
			tempFront = tempFront->child;
			while(tempFront)
			{
				if(strcmp(tempFront->name,bname)==0)
				{
					break;
				}
				else if(tempFront ->sibling ==0)
				{
					printf("no path to specified element\n");
					return 0;
				}
				else
				{
					tempBack = tempFront;
					tempFront = tempFront->sibling;
				}
			}
			if((type == 'D')&&(tempFront->type == 'D'))
			{
				if(tempBack == 0)
				{
					//no siblings, so delete away !
					tempFront->parent->child = 0;

				}
				else
				{
					tempBack->sibling = tempFront->sibling;
				}
				free(tempFront);
				return 1;
			}
			else if((type =='F')&&(tempFront->type == 'F'))
			{
				if(tempBack == 0)
				{
					//no siblings, so delete away !
					tempFront->parent->child = 0;

				}
				else
				{
					tempBack->sibling = tempFront->sibling;
				}
				free(tempFront);
				return 1;
			}
		}
		else
		{
			return 0;
		}
	}


}
int mkdir()
{
	return insertNode('D');
}
int creat()
{
	return insertNode('F');
}
int rmdir()
{
	return removeNode('D');
}
int rm()
{
	return removeNode('F');
}
int cd()
{
	int found =0;
	Node* tempFront = (Node *)malloc(sizeof(Node));
	Node* tempBack = (Node *)malloc(sizeof(Node));

	found = getDirectory();
	if(found)
	{
		tempFront = dir;
		if(tempFront->child)
		{
			tempBack =0;
			tempFront = tempFront ->child;
			while(tempFront)
			{
				if(strcmp(tempFront->name,bname)==0)
				{
					break;
				}
				else if(tempFront ->sibling ==0)
				{
					printf("no path to specified element\n");
					return 0;
				}
				else
				{
					tempBack = tempFront;
					tempFront = tempFront->sibling;
				}
			}
			if(tempFront->type == 'D')
			{
				cwd = tempFront;
				printf("cd successful\n");
				return 1;
			}
			printf("cd unsuccessful\n");
			return 0;
		}
	}
}
int ls()
{
	int found =0;
	Node* temp = (Node *)malloc(sizeof(Node));

	

	if(pathname == 0)
	{
		temp = cwd;
		while (temp) // list the contents of this directory
			{
				printf("%c\t%s\n", temp->type, temp->name);
				temp = temp->sibling;
			}
	}
	else if(strcmp(pathname,"/")==0)
	{
		temp = root;
		if(temp->child)
		{
			temp = temp->child;
			while (temp) // list the contents of this directory
			{
				printf("%c\t%s\n", temp->type, temp->name);
				temp = temp->sibling;
			}
		}
	}
	else
	{
		found = getDirectory();
		if (found)
		{
			temp = dir;
			if(temp->child)
			{

				temp = temp-> child;
				//check if baby already exists
				while(temp)
				{
						if (strcmp(temp->name, bname) == 0)
						{
							
							break;
						}
						else if (temp->sibling == NULL) 
						{
							printf("no path to specified element\n");
							return 0;
						}

						else // continue to search
						{
							temp = temp->sibling;
						}
				}

				
				if(temp->type == 'D')
				{
					temp = temp->child;
					while (temp) // list the contents of this directory
					{
						printf("%c\t%s\n", temp->type, temp->name);
						temp = temp->sibling;
					}
					return 1;
				}
			}
			return 0;
		}
	}
}
int pwd()
{
	Node* temp = (Node *)malloc(sizeof(Node));
	char cwdPath[64];
	char tempStr[64][64];
	int l =0,i=0,j=0;
	size_t len ;

	temp = cwd;
	

	while(temp->parent) // add path in reverse
	{
		strcpy(tempStr[l],temp->name);
		l++;
		
		temp = temp->parent;
	}
	

	if (l > 0)
	{
		for (i = l;i>0;i--)
		{
			strcat(cwdPath,"/");
			strcat(cwdPath,tempStr[i-1]);
		}
		len = strlen(cwdPath);
		cwdPath[len+1] = '\0';
		printf("%s\n",cwdPath);
	}
	else
	{
		strcpy(cwdPath  ,"/");
		printf("%s\n",cwdPath);
	}

	return 1;

}
int recSave(FILE* stream,Node*current) // recursively save the file
{
	//create temps
	Node* temp = (Node *)malloc(sizeof(Node));
	Node* saveTemp = (Node *)malloc(sizeof(Node));
	char cwdPath[64];
	char tempStr[64][64];
	int l =0,i=0,j=0;
	size_t len ;

	cwd = current;

	temp = cwd;
	while(temp->parent) // add path in reverse
	{
		strcpy(tempStr[l],temp->name);
		l++;
		temp = temp->parent;
	}

	if (l >0)
	{
		for (i = l;i>0;i--)
		{
			strcat(cwdPath,"/");
			strcat(cwdPath,tempStr[i-1]);
		}
		len = strlen(cwdPath);
		cwdPath[len+1] = '\0';
		printf("%s\n",cwdPath);
	}
	else
	{
		strcpy(cwdPath  ,"/");
		printf("%s\n",cwdPath);
	}

	fprintf(stream, "%c %s\n", current->type, cwdPath);
	saveTemp = saveTemp -> child;
	while(saveTemp)
	{
		recSave(stream,saveTemp);
		saveTemp = saveTemp ->sibling;
	}
	

}
int save()
{
	Node* cwdRef= (Node *)malloc(sizeof(Node)); // keep reference to cwd
	

	FILE* outfile = fopen(pathname,"w+");
	cwdRef = cwd;

	recSave(outfile,root);

	fclose(outfile);
	cwd = cwdRef;

	return 1;

}


int reload()
{
	FILE *input = fopen(pathname, "r");
	char type;
	clearTree(&root);

	init();

	while(!feof(input))
	{
		fscanf(input,"%c %s\n",&type,&pathname);

		if(strcmp(pathname,"/")!=0)// dont make root again
		{
			if (type == 'D')
			{
				mkdir();
			}
			else if (type == 'F')
			{
				creat();
			}
			else
			{
				printf("cannot load file! - Not a valid D or F\n");
				clearTree(&root);
				init();
				return 0;
			}
		}
	}
	fclose(input);
	cwd = root;

	return 1;
}

int main(void)
{
	int index, r;
	int running =1;
	char s[128];
	init();

	while (running)
	{
		
		printf("Input a command : ");
		fgets(s, 128, stdin); // input at most 128 chars BUT has \n at end		
		s[strlen(s) - 1] = 0;

		sscanf(s, "%s %s", command, pathname);		
		index = findCmd(command);
		switch (index) 
		{
		case 0: mkdir();    		break;
		case 1: rmdir();    		break;
		case 2: ls();       		break;
		case 3: cd();       		break;
		case 4: pwd();           	break;
		case 5: creat();    		break;
		case 6: rm();				break;
		case 7: reload();			break;
		case 8: save();				break;
		case 9: menu();				break;
		case 10: running= 0;		break;
		}
	}

	//dirName("ab/cde/fg.out");
	//getchar();
	return 0;
}