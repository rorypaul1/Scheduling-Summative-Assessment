#include "wish.h"
#include <ctype.h>  // isspace
#include <regex.h>  // regcomp, regexec, regfree
#include <stdio.h>  // fopen, fclose, fileno, getline, feof
#include <stdlib.h> // exit
#include <sys/types.h>
#include <sys/wait.h> // waitpid
#include <fcntl.h>
#include <unistd.h>


int main(int argc, const char *argv[]) {
  // YOUR CODE HERE

    // Prompt user for input
    char *line = "";   // Pointer to store the line
    size_t len = 0;      // Size of the buffer, getline will adjust this
    ssize_t nread;       // Number of characters read, including newline
    char * direc="";
    char * iterline;
    char *found;
    char *found_path;
    char *path="";
    char* path_result;
    //strcpy(path, "");
    char * commands[2]={" ", NULL};
    const char * paths[5]= {"","","","",""};
    char * redir;
    char * output_file;
    char * par_iter;
    char * par_found;
    int parallel_count=0;
    char * par_commands[5] = {"","","","",""};
    char file_read[256];
    char * file_line="";

    if(argv[1]!=NULL){
      FILE *fptr;
      fptr=fopen(argv[1],"r");
        while (fgets(file_read, sizeof(file_read), fptr)) {
        //iterate through to find how many parallel commands
        file_read[255]='\n';
        file_line=file_read;    
        
        
        if(strcmp(direc, "") != 0)
          printf("%s/wish>",direc);
        else
          printf("wish>");
        
        file_line[strcspn(file_line, "\n")] = 0;
        
        iterline=file_line;

        par_iter=file_line;
        
         if (strstr(file_line, "&") != NULL){
          
          while( (par_found = strsep(&par_iter," & ")) != NULL ){
              par_commands[parallel_count]=par_found;
              ++parallel_count;
            }
           
           int j;
           for(j=0;j<parallel_count+1;++j){
            if(strcmp(par_commands[j],"")!=0){
              file_line = par_commands[j];
              int proc=fork();
              if(proc==0){
                
                      if(strstr(file_line, ">") != NULL){
                  found = strsep(&iterline," ");
                  //Redirection
                  int i;
                  for(i=0;i<5;++i){
                    
                    redir = strsep(&iterline," ");
                    output_file = strsep(&iterline," ");
                    char *result = malloc(1024);
                    strcpy(result, &path_result[i]);
                    strcat(result,found);
                    
                    if(access(result,X_OK)!=0){
                      commands[0]=result;
                    
                    int proc=fork();
                    if(proc==0){
                      
                      int fd = open(output_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

                      dup2(fd, 1);   // make stdout go to file
                      execv(commands[0],commands);
                      break;
                  }
                  else if(proc!=0){
                    int waiting = wait(NULL);
                  }
                  }
                  else{
                    break;
                  }
                  }
                }
                else{
                  
                  found = file_line;
                  int i;
                for(i=0;i<5;++i){
                  char *result = malloc(1024);
                  strcpy(result, &path_result[i]);
                  strcat(result,found);
                  if(access(result,X_OK)!=-1){
                    commands[0]=result;
                  int proc=fork();
                  if(proc==0){
                    
                    execv(commands[0],commands);
                    break;
                  }
                  else if(proc!=0){
                    int waiting = wait(NULL);
                  }
                  }
                  else{
                    break;
                  }
                }
                    
                }
                      
                  }
                  else if(proc!=0){
                    int waiting = wait(NULL);
                  }
            }
            
           }
            
          
         }
        else{
          if (strstr(file_line, "cd") != NULL){
          while( (found = strsep(&iterline," ")) != NULL )
           if(strcmp(found,"cd")!=0){
              direc=found;
              chdir(direc);
            }
         }
          else if (strstr(file_line, "path") != NULL){
            
              int count=0;
          while( (found_path = strsep(&iterline," ")) != NULL ){
            
            if(strcmp(found_path,"path")!=0){
              path=found_path;
              path_result = malloc(strlen(found_path) + strlen("/") + 1); // +1 for the null-terminator
              strcpy(path_result, path);
              strcat(path_result,"/");
              paths[count]=path_result;
              ++count;
            }  
          }
             
          
        }
        else{
          // Not built in commands
          
          
          if(strstr(file_line, ">") != NULL){
            found = strsep(&iterline," ");
            //Redirection
            int i;
            for(i=0;i<5;++i){
              redir = strsep(&iterline," ");
              output_file = strsep(&iterline," ");
              char *result = malloc(1024);
              strcpy(result, &path_result[i]);
              strcat(result,found);
              if(access(result,X_OK)!=-1){
                commands[0]=result;
              
              int proc=fork();
              if(proc==0){
                
                int fd = open(output_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

                dup2(fd, 1);   // make stdout go to file
                execv(commands[0],commands);
                printf("\n");
                break;
            }
            else if(proc!=0){
              int waiting = wait(NULL);
            }
            }
            else{
              break;
            }
            }
          }
          else{
            found = strsep(&iterline," ");
            int i;
          for(i=0;i<5;++i){
            char *result = malloc(1024);
            strcpy(result, &path_result[i]);
            strcat(result,found);
            if(access(result,X_OK)!=-1){
              commands[0]=result;
            int proc=fork();
            if(proc==0){
              execv(commands[0],commands);
              break;
            }
            else if(proc!=0){
              int waiting = wait(NULL);
            }
            }
            else{
                break;
            }
          }
              
          }
          
          
        }
        }
         
    }
    }
    else{
      // While loop continues as long as the input is not 0
    while (strcmp(line, "exit") != 0) {
        //iterate through to find how many parallel commands
        
        if(strcmp(direc, "") != 0)
          printf("%s/wish>",direc);
        else
          printf("wish>");
        nread = getline(&line, &len, stdin);
        line[strcspn(line, "\n")] = 0;
        iterline=line;

        par_iter=line;
         if (strstr(line, "&") != NULL){
          
          while( (par_found = strsep(&par_iter," & ")) != NULL ){
              par_commands[parallel_count]=par_found;
              ++parallel_count;
            }
           
           int j;
           for(j=0;j<parallel_count+1;++j){
            if(strcmp(par_commands[j],"")!=0){
              line = par_commands[j];
              int proc=fork();
              if(proc==0){
                
                      if(strstr(line, ">") != NULL){
                  found = strsep(&iterline," ");
                  //Redirection
                  int i;
                  for(i=0;i<5;++i){
                    
                    redir = strsep(&iterline," ");
                    output_file = strsep(&iterline," ");
                    char *result = malloc(1024);
                    strcpy(result, &path_result[i]);
                    strcat(result,found);
                    if(access(result,X_OK)!=-1){
                      commands[0]=result;
                    
                    int proc=fork();
                    if(proc==0){
                      
                      int fd = open(output_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

                      dup2(fd, 1);   // make stdout go to file
                      execv(commands[0],commands);
                      break;
                  }
                  else if(proc!=0){
                    int waiting = wait(NULL);
                  }
                  }
                  else{
                    break;
                  }
                  }
                }
                else{
                  
                  found = line;
                  int i;
                for(i=0;i<5;++i){
                  char *result = malloc(1024);
                  strcpy(result, &path_result[i]);
                  strcat(result,found);
                  if(access(result,X_OK)!=-1){
                    commands[0]=result;
                  int proc=fork();
                  if(proc==0){
                    
                    execv(commands[0],commands);
                    break;
                  }
                  else if(proc!=0){
                    int waiting = wait(NULL);
                  }
                  }
                  else{
                    break;
                  }
                }
                    
                }
                      
                  }
                  else if(proc!=0){
                    int waiting = wait(NULL);
                  }
            }
            
           }
            
          
         }
        else{
          if (strstr(line, "cd") != NULL){
          while( (found = strsep(&iterline," ")) != NULL )
           if(strcmp(found,"cd")!=0){
              direc=found;
              chdir(direc);
            }
         }
          else if (strstr(line, "path") != NULL){
              int count=0;
          while( (found_path = strsep(&iterline," ")) != NULL ){
            if(strcmp(found_path,"path")!=0){
              path=found_path;
              path_result = malloc(strlen(found_path) + strlen("/") + 1); // +1 for the null-terminator
              strcpy(path_result, path);
              strcat(path_result,"/");
              paths[count]=path_result;
              ++count;
            }  
          }
             
          
        }
        else{
          // Not built in commands
          
          
          if(strstr(line, ">") != NULL){
            found = strsep(&iterline," ");
            //Redirection
            int i;
            for(i=0;i<5;++i){
              redir = strsep(&iterline," ");
              output_file = strsep(&iterline," ");
              char *result = malloc(1024);
              strcpy(result, &path_result[i]);
              strcat(result,found);
              if(access(result,X_OK)!=-1){
                commands[0]=result;
              
              int proc=fork();
              if(proc==0){
                
                int fd = open(output_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

                dup2(fd, 1);   // make stdout go to file
                execv(commands[0],commands);
                printf("\n");
                break;
            }
            else if(proc!=0){
              int waiting = wait(NULL);
            }
            }
            else{
              break;
            }
            }
          }
          else{
            found = strsep(&iterline," ");
            int i;
            
          for(i=0;i<5;++i){
            
            char *result = malloc(1024);
            strcpy(result, &path_result[i]);
            strcat(result,found);
            if(access(result,X_OK)!=-1){
              commands[0]=result;
            int proc=fork();
            if(proc==0){
              execv(commands[0],commands);
              break;
            }
            else if(proc!=0){
              int waiting = wait(NULL);
            }
            }
            else{
                break;
            }
          }
              
          }
          
          
        }
        }
         
    }
    }

    
  return 0;
}