#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>
#include<termios.h>

#define MAXTOKLEN 1024

FILE* source;
char pwd[MAXTOKLEN+1];
char PATH[MAXTOKLEN+1];
char HOME[MAXTOKLEN+1];

char temp[MAXTOKLEN+1];

typedef enum{

    nothing,cd,clr,dir,environ,echo,pause1,help,quit
}COMMANDS;

char* line;
char** args;

int status;

void do_cd()
{
    int i;
    //fprintf(stdout, "%d\n", args[1][0]);
    if(args[1]!=NULL)
    {
        if(args[1][0]=='~' && args[1][1]=='/')
        {
            chdir(HOME);
            for(i=2;i<strlen(args[1]);i++)
            {
                temp[i-2]=args[1][i];
            }
            temp[i-2]='\0';
            //fprintf(stdout, "%s\n", temp);
            chdir(temp);
            getcwd(pwd,MAXTOKLEN+1);
        }
        else if(!chdir(args[1]))
        {
            //fprintf(stdout, "%s\n", pwd);
            getcwd(pwd,MAXTOKLEN+1);          //update pwd
        }
        else
        {
            // errors in chdir
            if(errno==ENOENT)
                fprintf(stdout,"cd:%s:No such directory exists\n",args[1]);
            else if(errno==ENOTDIR)
                fprintf(stdout,"cd:%s:Specified path not a directory\n",args[1]);
        }
    }
    else
        fprintf(stdout, "No directory argument provided\n");
    fprintf(stdout, "Current Directory: %s\n", pwd);
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
    if(args[1]!=NULL)
    {
    dir=opendir(args[1]);
    int count;
    struct dirent *dptr = NULL;
    if(NULL == dir)
    { 
        fprintf(stdout,"Could not open the working directory\n"); 
    }
    else
    {
        //printf("\n"); 
        // Go through and display all the names (files or folders) contained in the directory. 
        for(count = 0; NULL != (dptr = readdir(dir)); count++) 
        { 
            printf("%s \n",dptr->d_name); 
        }
        printf("\nTotal %u\n", count);
    }
    }
    else
    {
    dir=opendir(pwd);
    int count;
    struct dirent *dptr = NULL;
    //printf("\n"); 
    // Go through and display all the names (files or folders) contained in the directory. 
    for(count = 0; NULL != (dptr = readdir(dir)); count++) 
    { 
        printf("%s \n",dptr->d_name); 
    }
    printf("\nTotal %u\n", count);
    }   
}

void do_echo()
{
    int i=1;
    while(args[i]!=NULL)
    {
        fprintf(stdout,"%s ",args[i]);
        i++;
    }
    fprintf(stdout,"\n");
}

void do_pause()
{
    if(args[1]!=NULL)
    {
        fprintf(stderr,"pause: Too many arguments\n");
        return ;
    }
    fprintf(stdout,"Press Enter to resume\n");
    tcflow(STDOUT_FILENO, TCOOFF);
    char temp_char='\0';
    while(temp_char!='\n')
    {
        fscanf(source,"%c",&temp_char);
    }
    tcflush(STDOUT_FILENO, TCIOFLUSH);
    tcflow(STDOUT_FILENO, TCOON);
}

void do_help()
{
      if(args[1]==NULL)
    {
      printf("Please enter the arguement.\nFollowing are the allowed arguements\n");
      printf("  cd\n  clr\n  pause\n  dir\n  environ\n  echo\n  quit\n");
      printf("Sample input\nhelp <arguement>\n"); 
    }
  else if(!strcmp(args[1],"cd"))
    {
      printf("NAME\n\tcd - change directory\n\nSYNOPSIS\n\tcd [directory]\n\nDESCRIPTION\n\t​Changes the current default directory to [directory] \n​If the [directory] argument is not present, the current directory is reported.\n");
    } 
  else if(!strcmp(args[1],"clr"))
    {
      printf("NAME\n\tclr - clear screen\n\nSYNOPSIS\n\tclr\n\nDESCRIPTION\n\tClears the terminal.\n");
    }
  else if(!strcmp(args[1],"pause"))
    {
      printf("NAME\n\tpause - pause the shell\n\nSYNOPSIS\n\tpause\n\nDESCRIPTION\n\tPauses the shell until enter is pressed.\n");
    }
  else if(!strcmp(args[1],"dir"))
    {
      printf("NAME\n\tdir - list directory files\n\nSYNOPSIS\n\tdir [directory]\n\nDESCRIPTION\n\t­Lists the contents of directory.\n");
    }
  else if(!strcmp(args[1],"echo"))
    {
      printf("NAME\n\techo - display line of text\n\nSYNOPSIS\n\techo [string]\n\nDESCRIPTION\n\tEcho the string to the standard output.\n");
    }
  else if(!strcmp(args[1],"environ"))
    {
      printf("NAME\n\tenviron - displays the environment variable bindings\n\nSYNOPSIS\n\tenviron\n\nDESCRIPTION\n\tLists all the environment strings.\n");
    }
  else if(!strcmp(args[1],"quit"))
    {
      printf("NAME\n\tquit - exit shell\n\nSYNOPSIS\n\tquit\n\nDESCRIPTION\n\tQuits the shell.");
    }
  else
    {
      printf("Wrong argument\n");
      printf("Please enter the correct arguement.\nFollowing are the allowed arguements\n");
      printf("  cd\n  clr\n  pause\n  dir\n  environ\n  echo\n  quit\n");
      printf("Sample input\nhelp <arguement>\n");
    }

}

void do_environ()
{
    if(args[1]!=NULL)
    {
        fprintf(stderr,"env: Too many arguments\n");
        return ;
    }
    fprintf(stdout,"PWD=%s\n",pwd);
    fprintf(stdout,"HOME=%s\n",HOME);
    fprintf(stdout,"PATH=%s\n",PATH);
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
            fprintf(stdout,"%s: Command not found\n",args[0]);
            break;
        default:
            break;
    } 
}

char *read_line()
{
    char *line1 = NULL;
    size_t buffersize = 0; 
    if((getline(&line1, &buffersize, source))!=-1)
        return line1;
    else
        exit(0);
    
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

int main(int argc, char *argv[], char *evnp[])
{
    strcpy(HOME,getenv("HOME"));
    if(argc==1)
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
    }
    int i;
    getcwd(pwd,MAXTOKLEN+1);
    strcpy(PATH,pwd);
    for(i=1;i<strlen(argv[0]);i++)
    {
        temp[i-1]=argv[0][i];
    }
    temp[i-1]='\0';
    //fprintf(stdout, "%s\n", temp);
    strcat(PATH,temp);
    //printf("%s\n",pwd);
    do{
    if(source==stdin)
        fprintf(stdout, "shell:%s$ ", strrchr(pwd,'/')+1);
    else
        fprintf(stdout,"\n");
    line=read_line();
    args=parse(line);
    //fprintf(stdout, "%s\n", args[0]);
    if(args[0]!=NULL)
        execute(args[0]);
    }while(1);
    return 0;
}
