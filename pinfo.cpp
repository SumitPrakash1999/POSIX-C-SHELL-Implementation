#include"headers.h"
using namespace std;
void print(struct rusage &us){
   string status="S";//default status is made sleep
   if(us.ru_utime.tv_sec+us.ru_stime.tv_sec>0){//cpu time + user time
    status="R";
   }
   
   if(us.ru_nvcsw+us.ru_nivcsw>0)//sum of voluntary and involuntary context switches
    status+="+";//foreground process

   if(us.ru_nswap>0){//no of times process was swapped out
    status="Z";//zombie process
   } 

   if (us.ru_majflt > 0) {// no of major page faults but it may not be correct always
        status = 'T';  // Stopped (on a signal)
    }
   cout<<"Process Status -- {"<<status<<"}"<<endl;
   //Interesting fact:- pinfo will show S+ as currently it is in foreground waiting to execute your command, but when we give the command it changes to R as it is executing

}
void pinfo(string pid){
    pid_t processid=stoi(pid);
    struct stat buf;
    string procPath="/proc/"+pid;
    if(stat(procPath.c_str(),&buf)==-1){// checking if process directory exists
        perror("pinfo:Error in stat for checking process directory");
        return;
    }
    cout<<"pid --"<<pid<<endl;

    struct rusage use;//moving in the proc directory to retrieve status
    if(getrusage(RUSAGE_SELF,&use)==0){//RUSAGE_SELF for calling process
        print(use);
        cout<<"memory --"<<use.ru_maxrss*getpagesize()<< "{Virtual Memory}"<<endl;
        //although here we are using RSS(resident set size) which tells how much a memory a process consumes in physical RAM(given in page segments), but it does not include memory consumed by libraries that were swapped out

    }
    else{
        perror("pinfo messed up with getrusage");
    }

    //for executable path
    char exebuf[PATH_MAX];//path_max has a size of 4096 in posix as max size for path
    ssize_t path=readlink((procPath+"/exe").c_str(),exebuf,sizeof(exebuf));
    if(path!=-1){
        exebuf[path]='\0';
        cout<<"Executable Path --"<<exebuf<<endl;
    }
    else{
        perror("pinfo encountered problems in exe path");
    }
}




// void print(struct rusage &us) {
//     char status;
//     if (us.ru_utime.tv_sec + us.ru_stime.tv_sec > 0) {
//         status = 'R';
//     } else {
//         status = 'S';
//     }
//     if (us.ru_nvcsw + us.ru_nivcsw > 0) {
//         status = '+';
//     }
//

// void pinfo(string pid) {
//     pid_t processid = stoi(pid);
//     struct stat buf;

//     // Construct the path to the process directory in /proc
//     string procPath = "/proc/" + pid;

//     if (stat(procPath.c_str(), &buf) == -1) {
//         perror(("pinfo: Error: Process directory does not exist for PID: " + pid).c_str());
//         return;
//     }
//     cout << "pid -- " << pid << endl;


