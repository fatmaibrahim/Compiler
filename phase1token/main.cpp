#include <iostream>
#include <fstream>
#include <sstream>
#include<string>
#include<map>
#include<vector>
#include <bits/stdc++.h>
#include<stack>
#define MAX 1000
using namespace std;

string production[MAX][MAX];
vector<string> tokens;

int row=44;
int col=31;
int table [44][31];
string input[31];
map<int, string>state;
vector<string> rangs;

int tointeger(string s){
 stringstream geek(s);
  int x = 0;
    geek >> x;
    return x;
}


void initializer_output(){
    string line = "";
    ifstream infile("stateoutput.txt");
    while (getline(infile, line))  //read line by line
    {
        string sec="";
        int Numstate=0;
        bool flag=true;
       for(int i=1;i<line.size();i++){
        if(line[i]=='}'&&flag){
            Numstate=tointeger(sec);
            sec="";
            flag=false;
            i++;
        }else{
        sec+=line[i];
        }
       }
      state[Numstate]=sec;
    }

}
void initializer_input(){
 string line = "";
    ifstream infile("stateinput.txt");
    int indexinput=0;
    while (getline(infile, line))  //read line by line
    {
        string sec="";
     for(int i=0;i<line.size();i++){
        if(line[i]=='\t'){
            if(sec.size()!=0){
                input[indexinput++]=sec;
                if(sec.size()>1){
                    rangs.push_back(sec);
                }
                sec="";
            }
        }else{
        sec+=line[i];
        }
     }
     if(sec.size()!=0){
                input[indexinput++]=sec;
                if(sec.size()>1){
                    rangs.push_back(sec);
                }
                sec="";
            }
    }
}
void initializer_table(){
     string line = "";
    ifstream infile("statetable.txt");
    int indexrow=0;
    int indexcol=0;
    while (getline(infile, line))  //read line by line
    {
        string sec="";
        for(int i=0;i<line.length();i++){
            if(line[i]=='-'){
                table[indexrow][indexcol++]=-1;
            }else{
            if(line[i]=='\t'){
                    if(sec.size()!=0){
                      table[indexrow][indexcol++]=tointeger(sec);
                      sec="";
                    }
            }else{
             sec+=line[i];
            }
        }
    }
           if(sec.size()!=0){
                      table[indexrow][indexcol++]=tointeger(sec);
                      sec="";
                    }
 indexrow++;
 indexcol=0;
}
}
void initializerall(){
initializer_input();
initializer_output();
initializer_table();
}

int get_index_input(string s)
{
    for(int i=0; i<col; i++)
    {
        if(s.compare(input[i])==0)
        {
            return i;
        }

    }
    return 1000;
}


struct Statepos
{
    Statepos() : output(),pos(0) {}
    Statepos(string newoutput,int newpos)
        : output(newoutput),pos(newpos) {}

    string output;
    int pos;

};

bool inrange(string s,char c)
{
    int x1=s[0];
    int x2=s[2];
    int x3=c;
    if(x3>=x1&&x3<=x2)
    {
        return true;
    }
    return false;
}

int getrange(char c)
{

    for(int i=0; i<rangs.size(); i++)
    {
        if(inrange(rangs[i],c))
        {
            return get_index_input(rangs[i]);
        }
    }
    return -1;
}

vector<string>parse_token(string token){
    vector<string> retu;
   if(token.size()!=0)
                {
                    int statenum=0;
                    stack<Statepos> stack1;
                    for(int i=0; i<token.size(); i++)
                    {
                        int index=get_index_input(token.substr(i,1));
                        if(index==1000)
                        {
                            index=getrange(token[i]);
                            if(index==-1)
                            {
                            if(!stack1.empty())
                            {
                                struct Statepos st=stack1.top();
                                stack1.pop();
                                retu.push_back(st.output);
                                while(!stack1.empty()){
                                    stack1.pop();
                                }
                            }
                                cout<<"error this character not defined in language";
                                cout<<"\n";
                                break;
                            }
                        }
                        ///////to final

                        if(table[statenum][index]==-1)
                        {
                            if(stack1.empty())
                            {
                                cout<<"error in lexical";
                                cout<<"\n";
                                break;
                            }
                            else
                            {
                                struct Statepos st=stack1.top();
                                stack1.pop();
                                retu.push_back(st.output);
                                i=st.pos;//////////////
                                statenum=0;
                                while(!stack1.empty()){
                                    stack1.pop();
                                }
                                continue;
                            }
                        }


                        statenum=table[statenum][index];
                        if(state.count(statenum)!=0)
                        {
                            struct Statepos st1=Statepos(state.at(statenum),i);
                            stack1.push(st1);
                        }

                        if(i==token.size()-1)
                        {
                            if(state.count(statenum)!=0)
                            {
                                retu.push_back(state.at(statenum));
                                break;
                            }
                            else
                            {
                                if(stack1.empty())
                                {
                                    cout<<"error in lexical";
                                    cout<<"\n";
                                    break;
                                }
                                else
                                {
                                    struct Statepos st=stack1.top();
                                    stack1.pop();
                                    retu.push_back(st.output);
                                     statenum=0;
                                     while(!stack1.empty()){
                                    stack1.pop();
                                }
                                    i=st.pos;//////////////
                                }

                            }
                        }
                    }
                }

                return retu;

}









void print_to_file(stack<string> stack1,vector<string> vector1,string errors,bool u){
  stack<string> stack3=stack1;
  if(!u){
 ofstream outfile;
  outfile.open("output_file.txt",ios_base::app);
  string str1="";
      while(!stack3.empty()){
        str1+=stack3.top();
        str1+=" ";
        stack3.pop();
      }
      string str2="";
      for(int i=0;i<vector1.size();i++){
        str2+=vector1[i];
        str2+=" ";
      }
      outfile<<str1<<"\t"<<str2<<"\t"<<errors<<"\n";

  }else{
       ofstream out_file("output_file.txt");
    out_file <<"Stack \t\t\t\t input \t\t\t\t\t\t output \n";
  }
}


int getindexinput(string str){
for(int i=1;i<7;i++){
    if(str.compare(production[0][i])==0){
        return i;
    }
}
return 0;
}
int getindexterminal(string str){
 for(int i=1;i<3;i++){
    if(str.compare(production[i][0])==0){
        return i;
    }
}
return 0;
}
string illegal_synch(string str){
string errors="Error:(illegal ";
errors+=str;
errors+=")";
return errors;
}

string missing_errors(string str){
string errors="Error: missing ";
str.erase(str.length()-1,1);
str.erase(0,1);
errors+=str;
errors+=", inserted";
return errors;
}
string illegal_errors(string terminl,string str){
string errors="Error:(illegal ";
errors+=terminl;
errors+=") – discard ";

str.erase(str.length()-1,1);
str.erase(0,1);
errors+=str;

return errors;
}


void output_tokens(){

//////initialize
tokens.push_back("$");
stack<string> mystack;
mystack.push("$");
mystack.push(production[1][0]);
print_to_file(mystack,tokens,"",true);
//
while(!mystack.empty()){
        string popstack="";
        string productions="";
        string currtoken="";
        if(tokens[0]!="$"){
        currtoken="'";
        currtoken+=tokens[0];
        currtoken+="'";
        }else{
        currtoken=tokens[0];
        }
        if(tokens.size()==1&&mystack.size()==1){
           print_to_file(mystack,tokens,"accept",false);
           break;
        }else if(tokens.size()>1&&mystack.size()==1){
              print_to_file(mystack,tokens,"error inputs is too much",false);
              break;
       }else{
         popstack=mystack.top();
           ///if terminal
           if(popstack[0]=='\''){
               if(popstack.compare(currtoken)==0){
                    string match="match ";
                           match+=popstack;
                      print_to_file(mystack,tokens,match,false);
                      tokens.erase(tokens.begin());
                      mystack.pop();
               }else{
                      print_to_file(mystack,tokens,missing_errors(popstack),false);       //error unmatched
                      mystack.pop();
               }
           }else{
           productions=production[getindexterminal(popstack)][getindexinput(currtoken)];
           cout<<getindexterminal(popstack);
           cout<<getindexinput(currtoken);
              if(productions.compare("\\L")==0){
                          string str1=popstack;
                          str1+="=>";
                          str1+=productions;
              print_to_file(mystack,tokens,str1,false);
                 mystack.pop();
              }else if(productions.compare("sync")==0){                    //error
                print_to_file(mystack,tokens,illegal_synch(popstack),false);
                  mystack.pop();
              }else if(productions.size()==0){                                    //error
                print_to_file(mystack,tokens,illegal_errors(popstack,currtoken),false);
                 tokens.erase(tokens.begin());
              }else{
                  string str2=popstack;
                          str2+="=>";
                          str2+=productions;
                print_to_file(mystack,tokens,str2,false);
                mystack.pop();
                  string sec="";
                  vector<string> temp;
              for(int j=0;j<productions.length();j++){
                if(productions[j]==' '){
                   temp.push_back(sec);
                    sec="";
                }else{
                sec+=productions[j];
                }

              }
              temp.push_back(sec);
              for(int j=temp.size()-1;j>=0;j--){
                    if(temp[j].size()!=0){
                       mystack.push(temp[j]);
                    }
              }

              }

              }


       }


    }


}










int main()
{
//initializerall();

//vector<string> gettalken=parse_token("sum=");   //insert token here


//for(int i=0;i<gettalken.size();i++){
  //  cout<<gettalken[i]<<"\n";
//}

tokens.push_back("c");
tokens.push_back("e");
tokens.push_back("a");
tokens.push_back("d");

tokens.push_back("b");


 production[0][1]="'a'";
 production[0][2]="'b'";
 production[0][3]="'c'";
 production[0][4]="'d'";
 production[0][5]="'e'";
 production[0][6]="$";

  production[1][0]="S";
   production[2][0]="A";

   production[1][1]="A 'b' S";
   production[1][3]="A 'b' S";
   production[1][5]="'e'";
   production[1][6]="\\L";

 production[2][1]="'a'";
   production[2][2]="sync";
   production[2][3]="'c' A 'd'";
      production[2][4]="sync";

output_tokens();


return 0;
}
