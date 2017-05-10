#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
typedef struct processstructure Processlist;
#define LINESIZE 100
#define BUFFSIZE 500
//gcc -o rsi rsi.c -lreadline

struct processstructure {
    pid_t    pid;
    char*    cmd;
    char*    status;
    Processlist*    Next;
};

char* cmdline;
Processlist* Head=NULL;

/*This is an adaptation of the code found 
 at http://www.cnblogs.com/delmory/p/3918811.html*/
 
void statusofprocss(){
	/* This function will update the current process List */
    Processlist* Current = Head;
    int detersnum;
    
    
    /* Get the sender's PID */
	//printf("checkpoint\n");
    pid_t pChild = waitpid(-1, &detersnum, WNOHANG | WUNTRACED | WCONTINUED);      
	
	int i;
	//printf("checkpoint222\n");	
    for (i=0;Current != NULL;Current = Current->Next) {
		//printf("checkpoint33333\n");	
		//printf("pchild is :%d\n", pChild);
	//printf("current pid is : %d\n ", Current->pid);
        if (WIFEXITED(detersnum) && strcmp(Current->status, "finished") != 0 && pChild == Current->pid) {
            strcpy(Current->status, "finished");
		//printf("checkpoint for finished\n");
        }
        else if (WIFSTOPPED(detersnum) && strcmp(Current->status, "stopped") != 0 && pChild == Current->pid) {
            strcpy(Current->status, "stopped");
		//printf("checkpoint for stopped\n");
        }
        else if (WIFSIGNALED(detersnum) && strcmp(Current->status, "finished") != 0 && pChild == Current->pid) {
            if (WTERMSIG(detersnum) == SIGKILL)
                strcpy(Current->status, "finished");
            else if (strcmp(Current->status, "stopped") != 0) 
                strcpy(Current->status, "Running");
			//printf("checkpoint for 3\n");
        }
        else if (WIFCONTINUED(detersnum) && strcmp(Current->status, "Running") != 0 && pChild == Current->pid) {
            strcpy(Current->status, "Running");
        }
    }
	 
 }
int main(){

   // char *input = NULL ;
    char *prompt;
    prompt = malloc(LINESIZE);
    char *input;
    char *option[sizeof(char*)*LINESIZE];
    Processlist* temp;
    char *temp_cmd;
    int countargs=0;
    int checknum=0;

    for(;;){
		signal(SIGCHLD, statusofprocss);
		
		 for (temp = Head;temp != NULL;temp = temp->Next) {
            if (strcmp(temp->status, "finished") == 0) {
                removenode(temp->pid);
            }
        }
    //option = (char*)malloc(sizeof(char*)*LINESIZE);
    strcat(prompt, "PMan : ");
    strcat(prompt, " > ");
	//update_bg_procss();
    cmdline = readline(prompt);
    countargs=Spacecounting(cmdline);
    //printf("PMan: %s\n",cmdline);
	if(cmdline!=NULL){
    int i=0;//split char into array
    option[i] = strtok(cmdline," ");
		while(option[i]!=NULL){
			option[++i] = strtok(NULL," ");
		}
	}
    //printf("split:%s, %s \n",option[0],option[1]);
    checknum = determincmd(cmdline, option, countargs);
	if(checknum<0){
		
		exit(-1);
	}
    //printf("temp: %s111\n",temp_cmd);
    *prompt = '\0';
    //free(prompt);


}
   // input = readline(prompt);

    //printf("%s\n", input);
    return 1;
}

int determincmd(char* Command, char* option[], int countargs){
    //printf("argscount:%d.\n",countargs);
	//if(countargs==0)
		//exit(-1);
 //printf("cmdline: %s.\n",Command);
    while(countargs!=0){
	if (strncmp(Command, "bg", strlen(Command)) == 0) {
		
   // printf("args123count:%d.\n",countargs);
            if (countargs > 1){
            //printf("hahah");
            bg(option);
            } else{
                printf("PMan: bg must followed by an argument\n");
            }
        }else if (strncmp(Command, "bglist", strlen(Command)) == 0) {
            bglist();
        }else if (strncmp(Command, "bgkill", strlen(Command)) == 0) {
            if (countargs > 1){
                int temp=0;
                char *ptr;
                temp=strtol(option[1], &ptr, 10);
                if (ptr == option[1]){    //if no characters were converted these pointers are equal
                    printf("PMan  >bgkill: required a pid(int)\n");
                //bgkill();
                }else {
                bgkill(temp);
                //printf("is a number : %d \n", temp);
                }
            }else{
                printf("PMan: bgkill: Must follow a pid\n");
            }
        }else if (strncmp(Command, "bgstop", strlen(Command)) == 0) {
            if (countargs > 1){
                int temp=0;
                char *ptr;
                temp=strtol(option[1], &ptr, 10);
                if (ptr == option[1]){    //if no characters were converted these pointers are equal
                    printf("PMan  >bgstop: required a pid(int)\n");
                //bgkill();
                }else {
                bgstop(temp);
                //printf("is a number : %d \n", temp);
                }
            }else{
                printf("PMan > bgstop: Must followed by a pid\n");
            }
        }    else if (strncmp(Command, "bgstart", strlen(Command)) == 0) {
            if (countargs > 1){
                int temp=0;
                char *ptr;
                temp=strtol(option[1], &ptr, 10);
                if (ptr == option[1]){    //if no characters were converted these pointers are equal
                    printf("PMan >bgstart: required a pid(int)\n");
                //bgkill();
                }else {
                bgstart(temp);
                //printf("is a number : %d \n", temp);
                }
            }else{
                printf("PMan > bgstart: Must followed by a pid\n");
                }
            }else if (strncmp(Command, "bstat", strlen(Command)) == 0) {
            if (countargs > 1){
                int temp=0;
                char *ptr;
                temp=strtol(option[1], &ptr, 10);
                if (ptr == option[1]){    //if no characters were converted these pointers are equal
                    printf("PMan >bstat: required a pid(int)\n");
                //bgkill();
                }else {
                bstat(temp);
                //printf("is a number : %d \n", temp);
                }
            }else{
                printf("PMan > bstat: Must followed by a pid\n");
                }
            }else{
            printf("PMan >%s: command not found\n",Command);
            }
			break;
	}
}



int bstat(pid_t pid){
// FILE *fp = popen("/proc/[pid]/stat(status)", "r") ;

}
int bgstart(pid_t pid)
{
    int errCheck;
    if (pid <= 0) {
        printf("RSI: bgstart: %d argument must be a pid\n", pid);
        return 1;
    }
    errCheck = kill(pid, SIGCONT);
    if (errCheck != 0) {
        printf("bgstart: %s\n", strerror(errno));
        return 1;
    }
    return 1;
}

int bgstop(pid_t pid)
{
    int errCheck;
    if (pid <= 0) {
        printf("RSI: bgstop: %d argument must be a pid\n", pid);
        return 1;
    }
    errCheck = kill(pid, SIGSTOP);
    if (errCheck != 0) {
        printf("bgstop: %s\n", strerror(errno));
        return 1;
    }
    return 1;
}
int bgkill(pid_t pid)
{
    //printf("pid is : %d\n", pid);
    int errCheck;
    if (pid < 0) {
        printf("PMan: bgkill: argument must be a pid\n");
        return 1;
    }
    errCheck = kill(pid, SIGTERM);
    removenode(pid);
    if (errCheck != 0) {
        printf("bgkill: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

int bg(char* options[]) {
    char* NodeCommand = (char*)malloc(sizeof(char)*LINESIZE);
    strcpy(NodeCommand, "");

    char** optionsPTR = options;
    *optionsPTR++;
    //char* path = (char*)malloc(sizeof(char)*LINESIZE);

    //strcpy(path, "/bin/");
    //strcat(path, optionsPTR[0]);

    int i;
    for (i = 0;optionsPTR[i] != NULL;i++) {
        strcat(NodeCommand, optionsPTR[i]);
        strcat(NodeCommand, " ");
    }

    int status, err;
    pid_t ChildPID, WaitErr;

    ChildPID = fork();
    //printf("Childpid: %d\n", ChildPID);
    if (ChildPID < 0) {
        /* There is an error */
        printf("Error forking the process.\n");
        exit(1);
    }
    if (ChildPID >= 0) {
        if (ChildPID == 0) {
            err = execvp(optionsPTR[0], optionsPTR);
            printf("PMan: %s\n", strerror(errno));
            exit(1);
        }
        else {
            addnode(NodeCommand, ChildPID,"Running", Head);
            free(NodeCommand);
            WaitErr = waitpid(ChildPID, &status, WNOHANG);
        }
    }
    //free(path);
    return 0;
}
int removenode(pid_t pid)
{
    Processlist* Current = Head;

    if (Current == NULL)
        return -1;    /* Error */

    if (Head->pid == pid) {
        /* the head node is the one to remove */
        if (Head->Next != NULL){
            Head = Head->Next;
        }else{                        /* Head is the only node */
            Head = NULL;
        }
    }else {
        for (;;) {
            if (Current->Next != NULL) {
                if (Current->Next->pid == pid) {
                    /* This is the node to remove */
                    Current->Next = Current->Next->Next;
                    return 0;
                }
            }
            else{
                break;
            }
        }
    }
    return 0;
}

int bglist()
{
    Processlist* Current = Head;
    int i;
    for (i = 0;Current != NULL;Current = Current->Next) {
        printf("%d:  %s, %s \n",  Current->pid,Current->status,getcwd(NULL, 0));
        i++;
    }
    printf("Total background job(s):   %d\n", i);
    return 0;
}

 
int addnode(char* Command, pid_t pid,char* Status, Processlist *Current){
    /* Allocate and initialize */
    Processlist* newNode = (Processlist*)malloc(sizeof(Processlist));
    newNode->cmd = (char*)malloc(sizeof(char)*LINESIZE);
	newNode->status = (char*)malloc(sizeof(char)*LINESIZE);
	
    strcpy(newNode->cmd, Command);
	strcpy(newNode->status, Status);
    newNode->pid = pid;
    newNode->Next = NULL;

    if (Current == NULL) {
        Head = newNode;
        return 1;
    }
    int i;
    for (i = 2;;i++) {
        if (Current->Next != NULL)
            Current = Current->Next;
        else
            break;
    }
    Current->Next = newNode; /* Add the new node on the end */

    /* on success, return # of nodes */
    return (i);
}


int Spacecounting(char* cmdline){
 int counted = 0; // result
//printf("PMan1: %s\n",cmdline);
    // state:
    const char* it = cmdline;
    int inword = 0;

    do switch(*it) {
        case '\0':
        case ' ': case '\t': case '\n': case '\r': // TODO others?
            if (inword) { inword = 0; counted++; }
            break;
        default: inword = 1;
    } while(*it++);
//printf("check: %d.\n", counted);
    return counted;
}
