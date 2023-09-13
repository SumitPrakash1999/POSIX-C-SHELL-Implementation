#include "headers.h"
using namespace std;
void echo(vector<string> s){
    for(string s1:s){
        if((s1[0]=='\''||s1[0]=='"')&&(s1[s1.length()-1]=='\''||s1[s1.length()-1]=='"')){
          cout<<s1.substr(1,s1.length()-2)<<" ";
        }
        else{
            cout<<s1<<" ";
        }          
    }
    cout<<endl;
}