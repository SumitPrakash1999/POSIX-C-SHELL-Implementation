#include "headers.h"
using namespace std;
vector<vector<string>> tokenize( string& input){
    vector<vector<string>> commands;
     char* cmdtoken = strtok(const_cast<char*>(input.c_str()), ";");// we need to convert input to c style string and as it is const we need to typecast it also
    while(cmdtoken!=NULL){
        vector<string> tokens;
        char* token=strtok(cmdtoken," ");
        while(token!=NULL){
            tokens.push_back(token);
            token=strtok(NULL," ");
        }
        if(!tokens.empty()){
            commands.push_back(tokens);
        }
        
        cmdtoken=strtok(NULL,";");
    } 
    return commands;
}
