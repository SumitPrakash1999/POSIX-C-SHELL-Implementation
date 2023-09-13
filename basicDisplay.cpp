#include"headers.h"
using namespace std;
string showBasicDisplay(char* curdirec, char* expath){
    if(strcmp(curdirec,expath)==0)//match
    {
      return "~";
    }
    else if(strncmp(curdirec,expath,strlen(expath)==0)){
        return string("~")+(curdirec+strlen(expath));// returning relative path by pointer arith curdirec and moving expath length in it as the files are in our directory
    }
    else{
        return curdirec;// returning absolute path
    }

}
