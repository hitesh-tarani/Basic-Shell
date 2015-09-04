#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>

#define MAXTOKLEN 1024

FILE* source;
char pwd[MAXTOKLEN+1];

typedef enum{

    nothing,cd,clr,dir,environ,echo,pause1,help,quit
}COMMANDS;

char* line;
char** args;

int status;

void do_cd()
{    
    //fprintf(stdout, "%d\n", args[1][0]);
    if(args[1]!=NULL)
    {
        if(!chdir(args[1]))
        {
            //fprintf(stdout, "%s\n", pwd);
            getcwd(pwd,MAXTOKLEN+1);          //update pwd
        }
        else
        {
            // errors in chdir
            if(errno==ENOENT)
                fprintf(stdout,"cd:%s:No such file exists\n",args[1]);
            else if(errno==ENOTDIR)
                fprintf(stdout,"cd:%s:Specified path not a directory\n",args[1]);
        }
    }
    else
    fprintf(stdout, "No file argument provided\n");
}

void do_clr()
{
    /*char clear [ 5 ] = { 27, '[', '2', 'J', 0 };
    printf ( "%s", clear );*/
    const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO,CLEAR_SCREE_ANSI,12);
}

void do_dir()
{
    DIR* dir=NULL;
    dir=opendir(pwd);
    int count;
    struct dirent *dptr = NULL;
    if(NULL == dir) 
    { 
        printf("\n ERROR : Could not open the working directory\n"); 
    }                         
    //printf("\n"); 
    // Go through and display all the names (files or folders) contained in the directory. 
    for(count = 0; NULL != (dptr = readdir(dir)); count++) 
    { 
        printf("%s  ",dptr->d_name); 
    } 
    printf("\nTotal %u\n", count);
}
void do_echo()
{
    fprintf(stdout,"%s\n",args[1]);
}

void do_pause()
{
    ;
}

void do_help()
{
    ;
}

void do_environ()
{
    ;
}

void do_quit()
{
    exit(0);
}

void execute(char* token)
{
    COMMANDS command=nothing;
    if(!(strcmp(token,"cd")))
        command=cd;
    if(!(strcmp(token,"clr")))
        command=clr;
    if(!(strcmp(token,"dir")))
        command=dir;
    if(!(strcmp(token,"environ")))
        command=environ;
    if(!(strcmp(token,"echo")))
        command=echo;
    if(!(strcmp(token,"pause")))
        command=pause1;
    if(!(strcmp(token,"help")))
        command=help;
    if(!(strcmp(token,"quit")))
        command=quit;
    if(command)
    {
    switch(command)
    {
        case cd:
            do_cd();
            break;
        case dir:
            do_dir();
            break;
        case environ:
            do_environ();
            break;
        case echo:
            do_echo();
            break;
        case pause1:
            do_pause();
            break;
        case help:
            do_help();
            break;
        case quit:
            do_quit();
            break;
        case clr:
            do_clr();
            break;
        case nothing:
            break;
        default:
            break;
    } 
    }
}

char *read_line()
{
  char *line1 = NULL;
  size_t buffersize = 0; 
  getline(&line1, &buffersize, stdin);
  return line1;
}

#define TOKEN_DELIM " \t\r\n\a"

char** parse(char* line1)
{
    int buffersize=MAXTOKLEN+1, position = 0;
    char **tokens = malloc(buffersize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line,TOKEN_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;
        if (position >= buffersize)
        {
            buffersize += MAXTOKLEN;
            tokens = realloc(tokens, buffersize * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
    
}

int main(int argc, char *argv[])
{
    /*if(argc==1)
    {
        source=stdin;
    }
    else
    {
        source=fopen(argv[1],"r");
        if(source==NULL)
        {
            fprintf(stderr, "File %s not found\n",argv[1]);
            return 0;
        }
    }*/
    //char prompt[60];
    getcwd(pwd,MAXTOKLEN+1);
    //printf("%s\n",pwd);
    do{
    fprintf(stdout, "shell:%s$ ", strrchr(pwd,'/')+1);
    line=read_line();
    args=parse(line);
    //fprintf(stdout, "%s\n", args[0]);
    execute(args[0]);
    }while(1);
    return 0;
}
