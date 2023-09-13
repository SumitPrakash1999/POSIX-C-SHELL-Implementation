#include"headers.h"
using namespace std;
bool aFlag = false;
bool lFlag = false;

void printinfo(struct stat& file){
    cout << (S_ISDIR(file.st_mode) ? "d" : "-");//checks for directory
    cout<< (file.st_mode & S_IRUSR ? "r" : "-");
    cout<< (file.st_mode & S_IWUSR ? "w" : "-");
    cout<< (file.st_mode & S_IXUSR ? "x" : "-");
    cout<< (file.st_mode & S_IRGRP ? "r" : "-");
    cout << (file.st_mode & S_IWGRP ? "w" : "-");
    cout << (file.st_mode & S_IXGRP ? "x" : "-");
    cout<< (file.st_mode & S_IROTH ? "r" : "-");
    cout<< (file.st_mode & S_IWOTH ? "w" : "-");
    cout << (file.st_mode & S_IXOTH ? "x" : "-");
}

void listFiles(string dir) {
    DIR* dp = opendir(dir.c_str());
    if (dp==NULL) {
        cerr << "ls: cannot access '" << dir << "': No such file or directory" << endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dp))!=NULL) {
        string name = entry->d_name;// to get filename from struct dirent
        if (!aFlag && name == ".") {
            continue;
        }

        if(lFlag){
            struct stat file;
            if(stat((dir+"/"+name).c_str(),&file)==0){
                printinfo(file);
                cout<<" "<<file.st_nlink;

                struct passwd*pw=getpwuid(file.st_uid);
                struct group *gr=getgrgid(file.st_gid);

                if(pw){
                    cout<<" "<<pw->pw_name;
                }
                else{
                    cout<<" "<<file.st_uid;
                }

                if(gr){
                    cout<<" "<<gr->gr_name;
                }
                else{
                    cout<<" "<<file.st_gid;
                }

                cout<<" "<<setw(10)<<file.st_size;//display file size with a width of 10
                cout<<" "<<ctime(&file.st_mtime);//prints last modification time and ctime converts to a human readable format
                cout<<" "<<name;
            }
        }
        else{
            struct stat file;
            if (stat((dir + "/" + name).c_str(), &file) == 0) {
                cout << name; // Display filename followed by contents
                // cout << " " << setw(10) << file.st_size; // Display file size with width of 10
                // cout << " " << ctime(&file.st_mtime);
            }
        }
        cout<<endl;
    }

    closedir(dp);
}


void ls(vector<string> s){
    
    int size=s.size();


    if(s.empty()||s[0]=="."){
        listFiles(".");
    }
    else if(s[0]==".."){
        listFiles("..");

    }
    else if(s[0]=="~"){
        char * home=getenv("HOME");
        if(home){
            listFiles(home);
        }
        else{
            cerr<<"Environment variable HOME not set"<<endl;
        }
        

    }
    else if(s[0] == "-la" || s[0] == "-al" ){
        aFlag=true;
        lFlag=true;
        if(size>1){
            for (int i=1;i<s.size();i++) {
                    if(s[i]=="-l"||s[i]=="-a"||s[i]=="-la"||s[i]=="-al"){
                        aFlag=true;
                        lFlag=true;
                        continue;
                    }
                    else{
                        listFiles(s[i]);
                    }    
                } 
        }
        else{
              listFiles(".");
        }
      
    } 
    else if (s[0]== "-a" ){
        aFlag = true;
        if(size>=2){
            if(s[1]=="-l"){
                lFlag=true;
                if(size>2){
                  for (int i=2;i<s.size();i++) {
                  listFiles(s[i]);
                  }
                }
            }
            else{
                for (int i=1;i<s.size();i++) {
                    if(s[i]=="-l"){
                        lFlag=true;
                        continue;
                    }
                    else if(s[i]=="-la"||s[i]=="-al"){
                        aFlag=true;
                        lFlag=true;
                        continue;
                    }
                    else if(s[i]=="-a"){
                        aFlag=true;
                        continue;
                    }
                    else{
                        listFiles(s[i]);
                    }    
                } 

            }
        }
        // if((size>=2)&&s[1]=="-l"){
        //     lFlag=true;
        // }
        else{
            listFiles(".");  
        }
             

    }
    else if(s[0]== "-l") {
        lFlag=true; 
        if(size>=2){
            if(s[1]=="-a"){
                aFlag=true;
                if(size>2){
                  for (int i=2;i<s.size();i++) {
                  listFiles(s[i]);
                  }
                }
            }
            else{
                for (int i=1;i<s.size();i++) {
                  if(s[i]=="-a"){
                        aFlag=true;
                        continue;
                    }
                    else if(s[i]=="-la"||s[i]=="-al"){
                        aFlag=true;
                        lFlag=true;
                        continue;
                    }
                    else if(s[i]=="-l"){
                        lFlag=true;
                        continue;
                    }
                    else{
                        listFiles(s[i]);
                    }    
                } 

            }
        }


        // if((size>=2)&&s[1]=="-a"){
        //     aFlag=true;
        // }
        else {
        listFiles(".");

        }    
        
    }
        
    else if (s[0] == "-") {
      cerr << "Unknown option: " << s[0] << endl;
      return;
    }
    else {// for ls dir_name
        for (string dir : s) {
           listFiles(dir);
        }   
    }
    

    


}