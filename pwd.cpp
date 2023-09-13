#include "headers.h"
// #include <unistd.h>
// #include <cstring>
// #include <string>
using namespace std;
void pwd(){
    char pwd[100];
    if(getcwd(pwd,sizeof(pwd))!=NULL){
        printf("%s\n",pwd);
    }
    else{
        perror("Error getting pwd");
    }
}