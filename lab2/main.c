#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
#define BUFFER_SIZE 50
#define buffer "\n\nCommand History:\n"

char history[10][BUFFER_SIZE];
int count = 0;
int caught = 0;

void printHistory()
{
    int i;
    int j = 0;
    int hcount = count;
    for (i = 0; i<10;i++)
    {
        printf("%d. ", hcount);
        while (history[i][j] != '\n' && history[i][j] != '\0')
        {
            printf("%c", history[i][j]);
            j++;
        }
        printf("\n");
        j = 0;
        hcount--;
        if (hcount == 0)
            break;
    }
  
  
  
    printf("\n");
    printf("COMMAND->\n");
}

/* the signal handler function */
void handle_SIGINT()
{
    write(STDOUT_FILENO,buffer,sizeof(buffer));
    printHistory();
    caught = 1;
}


/*
setup() reads in the next command line, separating it into distinct tokens
using whitespace as delimiters. setup() sets the args parameter as a
null-terminated string.
*/


void setup(char inputBuffer[], char *args[],int *background)
{
    int length, /* # of characters in the command line */
    i,     /* loop index for accessing inputBuffer array */
    start, /* index where beginning of next command parameter is */
    ct;     /* index of where to place the next parameter into args[] */
    ct = 0;
  
    /* read what the user enters on the command line */
    length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
  
    if (caught == 1)
    {
        length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
        caught = 0;
    }
    for (i = 9;i>0; i--)
        strcpy(history[i], history[i-1]);
  
    strcpy(history[0],inputBuffer);
    count++;
    start = -1;
    if (length == 0)
        exit(0);            /* ^d was entered, end of user command stream */
    if (length < 0)
    {
        perror("error reading the command\n");
        exit(-1);           /* terminate with error code of -1 */
    }
  
    /* examine every character in the inputBuffer */
    for (i=0;i<length;i++)
    {
        switch (inputBuffer[i])
        {
            case ' ':
            case '\t' :               /* argument separators */
                if(start != -1)
                {
                    args[ct] = &inputBuffer[start];    /* set up pointer */
                    ct++;
                }
                inputBuffer[i] = '\0'; /* add a null char; make a C string */
                start = -1;
                break;
              
            case '\n':                 /* should be the final char examined */
                if (start != -1)
                {
                    args[ct] = &inputBuffer[start];
                    ct++;
                }
                inputBuffer[i] = '\0';
                args[ct] = NULL; /* no more arguments to this command */
                break;
              
            default :             /* some other character */
                if (start == -1)
                    start = i;
                if (inputBuffer[i] == '&')
                {
                    *background = 1;
                    inputBuffer[i] = '\0';
                }
        }
    }
  
    args[ct] = NULL; /* just in case the input line was > 80 */
}

int main(void)
{
    char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
    int background;             /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE/+1];/* command line (of 80) has max of 40 arguments */
  
    pid_t pid;
    int i;
  
  
  
    /* set up the signal handler */
    struct sigaction handler;
    handler.sa_handler = handle_SIGINT;
    sigaction(SIGINT, &handler, NULL);
  
    while (1)
    {            /* Program terminates normally inside setup */
        background = 0;
        printf(" COMMAND->");
        setup(inputBuffer,args,&background);       /* get next command */
        
        //printf("%s, %s, %d\n",inputBuffer, args[1], background);
        /* the steps are:
         (1) fork a child process using fork()*/
        pid = fork();
        if (pid < 0)
        {
          
            printf("Fork failed.\n");
            exit (1);
        }
      
        // (2) the child process will invoke execvp()
        else if (pid == 0)
        {
            if (execvp (args[0], args) == -1)
                printf("Error executing command\n");
        }
      
        // (3) if background == 0, the parent will wait,
        // otherwise returns to the setup() function.
        else
        {
            if (background == 0)
                wait(NULL);
        }*/
    }
}