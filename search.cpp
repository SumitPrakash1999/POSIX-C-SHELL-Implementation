#include"headers.h"
using namespace std;
// bool searchRecursive(const string& target, const string& curdirec){
//       DIR* dir=opendir(curdirec.c_str());//then open the currect directory
//     if(dir==NULL){
//         perror("opendir");
//         return false;
//     }
//     bool flag=false;
//     struct dirent* entry;
//     while((entry=readdir(dir))!=NULL){
//         if(entry->d_type==DT_DIR){//if it is a directory
//             if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0){
//                 continue;//ignore these two directories
//             }

//             string subdir=curdirec;
//             subdir+="/";
//             subdir+=entry->d_name;//making path of subdirectory

//             if(searchRecursive(target,subdir)){// recursive call
//                 flag=true;
//                 break;
//             }
//         }
//         else if(entry->d_type==DT_REG){// it is a regular file
//                 if(strcmp(entry->d_name,target.c_str())==0){
//                     flag=true;
//                     break;
//                 }
//         }

        
        
//     }
//     closedir(dir);
//     return flag;

// }

bool ssearch(const string& target, const string& curdirec){
    // char curdirec[1024]; // first we get current directory
    // if(getcwd(curdirec,sizeof(curdirec))==NULL){
    //     perror("getcwd");
    //     return false;
    // }
    DIR* dir=opendir(curdirec.c_str());//then open the currect directory
    if(dir==NULL){
        perror("opendir");
        return false;
    }
    bool flag=false;
    struct dirent* entry;
    while((entry=readdir(dir))!=NULL){
        if(entry->d_type==DT_DIR){//if it is a directory
            if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0){
                continue;//ignore these two directories
            }

            string subdir=curdirec;
            subdir+="/";
            subdir+=entry->d_name;//making path of subdirectory

            if(ssearch(target,subdir)){// recursive call
                flag=true;
                break;
            }
        }
        else if(entry->d_type==DT_REG){// it is a regular file
                if(strcmp(entry->d_name,target.c_str())==0){
                    flag=true;
                    break;
                }
        }

        
        
    }
    closedir(dir);
    return flag;
}