#include"headers.h"
using namespace std;
 string prevDir;// storing prev directory
void cd(const string s){
   
    char curdirec[1024];//storing pwd
    if(getcwd(curdirec,sizeof(curdirec))==NULL){
        perror("getcwd");
        return;
    }

    if(s.empty()){
        const char* home=getenv("HOME");
        if(home){
            if(chdir(home)==0){
                prevDir=curdirec;
            }
            else{
                perror("chdir");
            }
        }
        else{
            perror("HOME cannot be retrieved:(");
        }
    }
    else if (s=="~") {
        const char* home=getenv("HOME");
        if (home) {
            if(chdir(home)==0){
                prevDir=curdirec;
            }
            else{
                perror("chdir");
            }
        } else {
           perror("HOME not set");
        }
    } 
    else if (s=="-") {
        if(!prevDir.empty()){
            if(chdir(prevDir.c_str())==0){// if chdir was successfull
               prevDir=curdirec;
            }
            else{
                perror("chdir");
            }
        }
        else {
            perror("cd: previous directory not available");
        }
    }
    else{
     if(chdir(s.c_str())==0){
        prevDir=curdirec;
     }
     else{
         perror("cd:");
     }         

    }    
}