#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <libgen.h> 
#include<dirent.h>
#include<sys/stat.h>
#include<iomanip>
#include<grp.h>
#include<sys/resource.h>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<sstream>   
#include<csignal>

using namespace std;
string showBasicDisplay(char *a ,char* b);
vector<vector<string>> tokenize(string & i);
void pwd();
void echo(vector<string> s);
void cd(const string s);
void ls(vector<string> s);
void pinfo(string r);
bool ssearch(const string& target, const string& curdirec);
// void executeCommand(vector<string>& s,bool check);
// pid_t foregroundpid=0;
//bool searchRecursive(const string s1,const string s2);