cd x64/Debug
TestExec.exe /../TestFiles1 *.h *.cpp
cd ..
cd ..
pause#include <iostream>
#include <string>
#include <vector>         //to store parsed input
#include <algorithm>
using namespace std;

int main() {
    string line;
    vector<string> sentences;
    vector<string> filtered;
    
    //1. Parse Input
    while (getline(cin, line, '|'))       //parse inputs by '|' delimiter
    {
        sentences.push_back(line);
        filtered.push_back(line);
        //cout << line << endl;
    }
    
    //2. Filter
    for(int i=0; i<filtered.size(); i++)
    {
        //a) Remove leading white space
        while(filtered[i][0] == ' ')
            filtered[i].erase(filtered[i].begin());
        
        for(int j=0; j<filtered[i].size(); j++)
        {
            //b) make all lowercase
            if(isupper(filtered[i][j]))
            {
                filtered[i][j] = tolower(filtered[i][j]);
            }
            
            //c) remove contiguous whitespaces
            while(filtered[i][j] == ' ' && filtered[i][j+1] == ' ')
            {
                filtered[i].erase(filtered[i].begin()+j);   
                j--;                                        //accomodate erased char
            }
            
            //d) remove non-alpha numeric characters   
            if(!isalnum(filtered[i][j]) && filtered[i][j] != ' ')
            {
                filtered[i].erase(filtered[i].begin()+j);
                j--;                                        //accomodate erased char
            }
        }
        
        //e) Remove trailing white space
        while(*filtered[i].end()-1 == ' ')
            filtered[i].erase(filtered[i].end() - 1);
        
        //cout << "filtered[i]: " << filtered[i] << endl;       //debugging
    }
    cout << endl;
    
    //3. compare filtered and original
    int index = 0;                                                   //output index
    for(int i=0; i< filtered.size(); i++)
    {
        for(int j = i; j < filtered.size(); j++)
        {
            //Keep the larger passage
            if(sentences[index].size() < filtered[j].size())
                index = j;
                
            //a) if two passages are considered equal after filtering above, keep shortest (of the original?)
            else if(sentences[index] == filtered[j])
            {
                if(sentences[index].size() < sentences[j].size())    //current original larger?
                {
                    index = j;
                }
            }
           
        }
    }
    
    //3. Display output
    cout << sentences[index];
    
    
    /* Issue with Test 2 ?? Expected output 2 contains delimited character '|'??? */
}
