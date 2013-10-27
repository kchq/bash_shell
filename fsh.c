#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LENGTH 1024
#define DELIMS " \t"

// This helper function will trim the leading and trailing whitespace of a given string
char *trim(char *str) {
  // isspace() tests whether character is a standard white-space character
  while(isspace(*str)) 
    str++; // Trim by advancing

  if(*str == '\0')  // The string was all whitespace chars
    return str;

  // Trim trailing space
  char *end = str + strlen(str) - 1;
  while(end > str && isspace(*end))
    end--;

  *(end+1) = '\0'; // Add a new null terminator
  return str;
}

int execute(char* input, int *retval) {

  //char* input;
  char *str1, *token;
  int j, i = 0;
  char* args[100];
  for (int i = 0; i < 100; i++) {
    args[i] = NULL;
  }

  i = 0;
  if (strcmp(input, "") == 0) {
    return 2; // will prompt again
  }
  for (j = 1, str1 = input; ; j++, str1 = NULL) {
    token = strtok(str1, DELIMS);
    if (token == NULL)
      break;
    printf("%d: %s\n", j, token);
    token = trim(token);
    args[i] = token;
    i++;
  }
  
  for (int k = 0; k < i; k++) {
    printf("Element %d = %s\n", k, args[k]);
  }
  
  printf("Input = %s\n", input);
  if (strcmp(args[0], "cd") == 0) {
    
    if (args[1] == NULL) chdir(getenv("HOME"));
    else chdir(args[1]);
    

  //-----------------EXIT-------------------//
  } else if (strcmp(args[0], "exit") == 0) {
    if (args[1] != NULL) {

      // Set the return value via the int param
      int a = atoi(args[1]);
      *retval = a;
      //printf("suckem%s\n", args[1]);
      //printf("retjjj %d\n", a);
    }
    //free(input);
    return -1; // -1 always means exit
    
  } else if (strcmp(args[0], ".") == 0) {
    if (args[1] != NULL) {
      FILE *fp;
      char *line = NULL;
      size_t len = 0;
      size_t read;
      
      fp = fopen(args[1],"r");
      if (fp == NULL) {
	printf ("File not found\n");
	return 2;
      }
      while ((read = getline(&line, &len, fp)) != -1) {
	line = trim(line);
	int retval = 0;
	execute(line, &retval);
	//fprintf (fp, "Hello, there.\n"); // if you want something in the file.
	
      }
      /*if (read == -1) {
	printf("Bad Read\n");
	}*/
      fclose (fp);
      return -1;
    }
  } else { //Execute
    pid_t pid = fork();
    pid_t endId;
    int status;
    
    if (pid == 0){
      int value = execvp(args[0], args);
      if (value == -1) {
	printf("%s: No such file or directory\n", args[0]);
      }
    } else {
      pid_t tpid;
      waitpid(pid, &status, 0);
      if (endId == -1) { // error
	perror("error with waitpid");
	exit(EXIT_FAILURE);
      } else if (endId == pid) { // child terminated
	if (WIFEXITED(status)) {
	  printf("Ended normally\n");
	} else if (WIFSIGNALED(status)) {
	  printf("Ended because of uncaugh signal\n");
	} else if (WIFSTOPPED(status)) {
	  printf("Child process stopped\n");
	}
	exit(EXIT_SUCCESS);
      }
      /*do {
	//printf("a");
	tpid = wait(&status);
	} while(tpid != pid);*/
    }
    printf("status %d\n", status);
    return status;

    //waitpid(pid, &status, 0);
  }
    
  free(input);
  if (errno) perror("Command failed");
  
  return 2;
}


int main(int argc, char *argv[]) {
  char* input;
  char prompt[MAX_LENGTH] = "CSE451Shell% ";
  
  while (1) {
    //snprintf(prompt, sizeof(prompt), "CSE451Shell% ");
    input = readline(prompt);
    int retval = 1;
    int ret = execute(input, &retval);
    if (ret == -1) {
      printf("retval %d\n", retval);
      return retval;
    } else {
      printf("retval updated:  %d\n", retval);
      retval = ret;
    }
  }
}



