#include"headers.h"
using namespace std;
const int History_Max = 20;
const string History_file="cmd_history.txt";
vector<string> cmdhistory;//to temporarily store history
int hpos=0;
pid_t foregroundpid=0;

void loadHistory(){// for reading history from file using ifstream
    ifstream historyfile(History_file);
    if(!historyfile){
        cerr<<"History file not found. Creating a new one"<<endl;
        ofstream newHistoryFile(History_file);
        newHistoryFile.close();
        return;
    }
    string temp;
    while(getline(historyfile,temp)){
        cmdhistory.push_back(temp);
    }
    historyfile.close();
}

void addHistory(string & cmd){//adding history to file
    if(cmdhistory.size()>=History_Max){
        cmdhistory.erase(cmdhistory.begin());//if its full then delete first element
    }
    cmdhistory.push_back(cmd);

    ofstream historyfile(History_file,std::ios_base::app);
    if(historyfile){
        historyfile<<cmd<<endl;
        historyfile.close();
    }
}

void displayHistory(int n){
    int start=max(0,static_cast<int>(cmdhistory.size())-n);
    for (int i = start; i < cmdhistory.size(); ++i) {
        cout << cmdhistory[i] << endl;
    }
}



void executeCommand(vector<string>& cmdtokens, bool isBackground) {//for fg and bg
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        
        char ** args=new char*[cmdtokens.size()+1];
        for (size_t i = 0; i < cmdtokens.size(); ++i) {// we need to convert it as execvp cant run on string
            args[i] = const_cast<char*>(cmdtokens[i].c_str());// const_cast is used to remove const as execvp may modify it
        }
        args[cmdtokens.size()]=nullptr;//c-style string formed


        
        // handleRedirection(cmdtokens);


        if(execvp(args[0], args)==-1){
        cerr << "Command not found: " << args[0] << endl;
        delete[] args;//deallocating memory
        exit(1);//exit_failure=1
        }        
        
    } else if (pid > 0) {
        // Parent process
        
        if (!isBackground) {
            foregroundpid=pid;//updating foregroundpid in parent process
            int status;
            waitpid(pid, &status, WUNTRACED);// wait for child to finish or stop
        } else {
            cout << "Background process created with PID: " << pid << endl;
        }
    } else {
        cerr << "Fork failed" << endl;
    }
}

void handleCtrlZ(int signum){
    // cout<<"ctrlZ"<<endl;//debug
    if(foregroundpid!=0&& signum==SIGTSTP){
        cout<<"Process "<<foregroundpid<<" stopped"<<endl;
        kill(foregroundpid,SIGTSTP);//here kill sends SIGTSTP signal to foregroundpid  
        foregroundpid=0;
    }

}
void handleCtrlC(int signum){
    // cout<<"ctrlC"<<endl;//debug
    if(foregroundpid!=0){
        kill(foregroundpid,SIGINT);
        cout<<"Process "<<foregroundpid<<" interrupted"<<endl;
        foregroundpid=0;
    }

}


int main() {

        signal(SIGTSTP,handleCtrlZ);
        signal(SIGINT,handleCtrlC);

        

        loadHistory();

        char exepath[1024];
        ssize_t p=readlink("/proc/self/exe",exepath,sizeof(exepath));//to store current path in a buffer
        if(p==-1){
            perror("Error getting current path");
        }
        char* expath=dirname(exepath);//to remove last part i.e. filename

        char syname[30];
        gethostname(syname,sizeof(syname));

       
        while (true) {

        char curdirec[1024];
        getcwd(curdirec,sizeof(curdirec));    
        string prompt=showBasicDisplay(curdirec,expath);   
        cout<<"<"<<getenv("USER")<<"@"<<syname<<":"<<prompt<<">";       
       
                

        string input;
        if(!getline(cin,input)){//as spaces may be present
            cout<<"Exiting shell....Bye..."<<endl;//for ctrlD
            break;
        }  

        hpos=0;//for up arrow
        addHistory(input);//for history
        vector<vector<string>> res=tokenize(input);

        int saved_stdin = dup(0);  // Save the original standard input
        int saved_stdout = dup(1); // Save the original standard output

        for(vector<string>& cmdtokens:res){
        if(!cmdtokens.empty()){


            string ifilename,ofilename;
            bool ipredirect=false;
            bool opredirect=false;
            //checking redirection symbols
            for(size_t i=0;i<cmdtokens.size();i++){
                if(cmdtokens[i]=="<"){
                    ipredirect=true;
                    ifilename=cmdtokens[i+1];
                    cmdtokens.erase(cmdtokens.begin()+i,cmdtokens.begin()+i+2);
                    i=i-2;
                }
                else if(cmdtokens[i]==">"){
                    opredirect=true;
                    ofilename=cmdtokens[i+1];
                    cmdtokens.erase(cmdtokens.begin()+i,cmdtokens.begin()+i+2);
                    i=i-2;
                }
                else if(cmdtokens[i]==">>"){
                    opredirect=true;
                    ofilename=cmdtokens[i+1];
                    cmdtokens.erase(cmdtokens.begin()+i,cmdtokens.begin()+i+2);
                    i=i-2;
                }
            }

            

            if(ipredirect){
            int fd=open(ifilename.c_str(),O_RDONLY) ;
            if(fd<0){
                cerr<<"Error!Input file not found"<<endl;
                continue;
            }
            dup2(fd,0);
            close(fd);
            }

            if(opredirect){
                int fd;
                if(cmdtokens.size()>0&&cmdtokens[0]==">>"){
                    fd=open(ofilename.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);

                }else{
                    fd=open(ofilename.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0644);
                }
            
            if(fd<0){
                cerr<<"Error!Output file opening or creation encountered problems!!"<<endl;
                continue;
            }
            dup2(fd,1);
            close(fd);
            }            
            //redirection checking ends here

            if(cmdtokens[0]=="pwd"){// for handling pwd
                pwd();
            }    
            else if(cmdtokens[0]=="echo"){ //for handling echo
                cmdtokens.erase(cmdtokens.begin());//remove echo word
                echo(cmdtokens);
            }
            else if(cmdtokens[0]=="cd"){//for handling cd
                if(cmdtokens.size()>2){
                    cerr<<"Invalid arguments for error handling(cd)"<<endl;
                }
                else{
                    if(cmdtokens.size()==2){
                        cd(cmdtokens[1]);
                    }
                    else{
                        cd("");
                    }
                    
                }
                
            }
            else if(cmdtokens[0]=="ls"){// for handling ls
                cmdtokens.erase(cmdtokens.begin());
                ls(cmdtokens);
            }
            else if(cmdtokens[0]=="pinfo"){
                cmdtokens.erase(cmdtokens.begin());
                if(cmdtokens.empty()){// if no pid given we'll take pid of calling process using getpid
                    pinfo(to_string(getpid()));
                }
                else{
                    pinfo(cmdtokens[0]);
                }
            }
            else if(cmdtokens[0]=="search"){
                cmdtokens.erase(cmdtokens.begin());
                if(cmdtokens.size()!=1){// if no pid given we'll take pid of calling process using getpid
                    cerr<<"Invalid Usage of search"<<endl;
                }
                else{
                    bool res=ssearch(cmdtokens[0],curdirec);
                    if(res==true){
                        cout<<"True"<<endl;
                    }
                    else{
                        cout<<"False"<<endl;
                    }
                }
            }
            else if(cmdtokens[0]=="history"){
                if(cmdtokens.size()>1){
                    int num=stoi(cmdtokens[1]);
                    displayHistory(num);
                }
                else{
                    displayHistory(10);
                }


            }
            else if(cmdtokens[0]=="exit"){
                exit(0);
            }
            else{
                bool isBack=false;
                if(!cmdtokens.empty()&&cmdtokens.back()=="&"){
                    isBack=true;
                    cmdtokens.pop_back();
                }

                executeCommand(cmdtokens,isBack);
            }
             
        }
        }
        // Restore the original standard input and output
        dup2(saved_stdin, 0);
        dup2(saved_stdout, 1);
       

        
    }
   

    return 0;
}