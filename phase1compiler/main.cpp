#include <iostream>
#include <string>
#include <iterator>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#define Max 1000
#define Maxtemp 100
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

vector<string> Keywords;
vector<char> Punctuations;
map<string,string> Regular_expressions;
map<string,string> Regular_definitions;
int table[Max][Max];
string input [Max];
map <int,string> State;
int Numstate=1;
int Numinput=0;
vector<string> rangs;

string nfatable[Maxtemp][Maxtemp];/////initialize it
int dfatable[Maxtemp][Maxtemp];/////initialize it
bool dfaoutput[Maxtemp];////////////////////

string tempinput[Maxtemp];/////initialize it
int tempstate=0;/////initialize it
int tempnuminput=0;/////initialize it
int ashin=0;
int ashout=0;
int size_of_nfa=0;


int get_index_inputtemp(string s)
{
    for(int i=0; i<tempnuminput; i++)
    {
        if(s.compare(tempinput[i])==0)
        {
            return i;
        }

    }
    return 100;
}

string tostring(int i)
{
    ostringstream str1;
    str1 << i;
    string geek = str1.str();
    return geek;
}
int tointeger(string s)
{
    stringstream geek(s);
    int x = 0;
    geek >> x;
    return x;
}

struct Node
{
    Node() : input(),inputstate(0),outputstate(0) {}
    Node(string newinput,int newinputstate,int newoutputstate)
        : input(newinput),inputstate(newinputstate),outputstate(newoutputstate) {}

    string input;
    int inputstate;
    int outputstate;

};



string deletespace(string s)
{
    string str="";
    for(int i=0; i<s.length(); i++)
    {
        if(s[i]!=' ')
            str.push_back(s[i]);
    }
    return str;
}

void addtoinput(string s)
{
    for(int i=0; i<Numinput; i++)
    {
        if(s.compare(input[i])==0)
        {
            return;
        }

    }
    if(s.size()==3)
    {
        rangs.push_back(s);
    }
    input[Numinput]=s;
    Numinput++;
}

void addtoinputtemp(string s)
{
    for(int i=0; i<tempnuminput; i++)
    {
        if(s.compare(tempinput[i])==0)
        {
            return;
        }

    }
    tempinput[tempnuminput]=s;
    tempnuminput++;
}

int get_index_input(string s)
{
    for(int i=0; i<Numinput; i++)
    {
        if(s.compare(input[i])==0)
        {
            return i;
        }

    }
    return 1000;
}

void initialize_table()
{
    for(int i=0; i<Max; i++)
    {
        for(int j=0; j<Max; j++)
        {
            table[i][j]=-1;
        }
    }

}

void Parsefile()
{
    string line = "";
    ifstream infile("input.txt");

    while (getline(infile, line))  //read line by line
    {
        if(line[0]=='{')
        {

            size_t pos = 0;
            string s=line.substr(1,line.length()-2);
            s+=" ";
            string token;
            while ((pos = s.find(" ")) != string::npos)
            {
                token = s.substr(0, pos);
                Keywords.push_back(deletespace(token));
                for(int p=0; p<token.size(); p++)
                {
                    string w(1,token[p]);
                    addtoinput(w);
                }
                s.erase(0, pos + 1);
            }

        }
        else if(line[0]=='[')
        {
            for (int i = 1; i < line.length()-1; i++)
            {
                if(line[i]!=' '&&line[i]!='\\')
                {
                    Punctuations.push_back(line[i]);
                    string w(1,line[i]);
                    addtoinput(w);
                }
            }

        }
        else
        {
            for (int i = 0; i < line.length(); i++)
            {
                if(line[i]=='=')
                {
                    string str1=line.substr(0,i);
                    string str2=line.substr(i+1,line.length());
                    Regular_definitions.insert(pair<string, string>(deletespace(str1), deletespace(str2)));
                    break;
                }
                else if(line[i]==':')
                {
                    string str1=line.substr(0,i);
                    string str2=line.substr(i+1,line.length());
                    Regular_expressions.insert(pair<string, string>(deletespace(str1), str2));
                    break;
                }
            }
        }


    }

    //printing data
    map<string, string>:: iterator itr;
    for (itr = Regular_expressions.begin(); itr != Regular_expressions.end(); ++itr)
    {
        cout << '\t' << itr->first
             << '\t' << itr->second << '\n';
    }
    cout << endl;
    for (itr = Regular_definitions.begin(); itr != Regular_definitions.end(); ++itr)
    {
        cout << '\t' << itr->first
             << '\t' << itr->second << '\n';
    }
    cout << endl;

    for(int i=0; i<Punctuations.size(); i++)
    {
        cout<<Punctuations.at(i)<<'\n';
    }

    for(int i=0; i<Keywords.size(); i++)
    {
        cout<<Keywords.at(i)<<'\n';
    }

}


struct Node closure(Node node)
{
    struct Node retnode;
    if(node.outputstate==-1&&node.inputstate==-1)
    {
        int index=get_index_inputtemp(node.input);
        nfatable[tempstate][index].append(tostring(tempstate));
        nfatable[tempstate][index].append(",");
        retnode=Node(node.input,tempstate,tempstate);
        tempstate++;
    }
    else if(node.outputstate!=node.inputstate)
    {
        int index=get_index_inputtemp("\\L");
        nfatable[node.outputstate][index].append(tostring(node.inputstate));
        nfatable[node.outputstate][index].append(",");
        nfatable[node.inputstate][index].append(tostring(node.outputstate));
        nfatable[node.inputstate][index].append(",");
        retnode=Node(node.input,node.inputstate,node.outputstate);

    }
    else
    {
        retnode=Node(node.input,node.inputstate,node.outputstate);
    }
    return retnode;
}

struct Node positive_closure(Node node)
{

    struct Node retnode;

    if(node.outputstate==-1&&node.inputstate==-1)
    {
        int index=get_index_inputtemp(node.input);
        int x=tempstate;
        tempstate++;
        nfatable[x][index].append(tostring(tempstate));
        nfatable[x][index].append(",");
        nfatable[tempstate][index].append(tostring(tempstate));
        nfatable[tempstate][index].append(",");
        retnode=Node(node.input,x,tempstate);
        tempstate++;
    }
    else
    {
        int index=get_index_inputtemp("\\L");
        nfatable[node.outputstate][index].append(tostring(node.inputstate));
        nfatable[node.outputstate][index].append(",");
        retnode=Node(node.input,node.inputstate,node.outputstate);
    }
    return retnode;
}
struct Node disjunction(Node node1,Node node2)
{
    struct Node retnode;
    string s=node1.input;
    s.append("|");
    s.append(node2.input);
    int index=get_index_inputtemp("\\L");
    if(node1.inputstate==-1&&node1.outputstate==-1&&node2.inputstate==-1&&node2.outputstate==-1)
    {
        int index1=get_index_inputtemp(node1.input);
        int index2=get_index_inputtemp(node2.input);
        int x1=tempstate;
        tempstate++;
        nfatable[x1][index1].append(tostring(tempstate));
        nfatable[x1][index1].append(",");
        nfatable[x1][index2].append(tostring(tempstate));
        nfatable[x1][index2].append(",");

        retnode=Node(s,x1,tempstate);
        tempstate++;
    }
    else if(node1.inputstate==-1&&node1.outputstate==-1)
    {
        int index1=get_index_inputtemp(node1.input);
        nfatable[node2.inputstate][index1].append(tostring(node2.outputstate));
        nfatable[node2.inputstate][index1].append(",");
        retnode=Node(s,node2.inputstate,node2.outputstate);
    }
    else if(node2.inputstate==-1&&node2.outputstate==-1)
    {
        int index2=get_index_inputtemp(node2.input);
        nfatable[node1.inputstate][index2].append(tostring(node1.outputstate));
        nfatable[node1.inputstate][index2].append(",");

        retnode=Node(s,node1.inputstate,node2.outputstate);
    }
    else
    {
        int in,out;
        if(node1.inputstate!=node2.inputstate)
        {
            nfatable[tempstate][index].append(tostring(node1.inputstate));
            nfatable[tempstate][index].append(",");
            nfatable[tempstate][index].append(tostring(node2.inputstate));
            nfatable[tempstate][index].append(",");
            in=tempstate;
            tempstate++;
        }
        else
        {
            in=node1.inputstate;
        }
        if(node1.outputstate!=node2.outputstate)
        {
            nfatable[node1.outputstate][index].append(tostring(tempstate));
            nfatable[node1.outputstate][index].append(",");
            nfatable[node2.outputstate][index].append(tostring(tempstate));
            nfatable[node2.outputstate][index].append(",");
            out=tempstate;
            tempstate++;
        }
        else
        {
            out=node1.outputstate;
        }
        retnode=Node(s,in,out);
    }
    return retnode;

}

struct Node concatenation(Node node1,Node node2)
{
    struct Node retnode;

    string s=node1.input;
    s.append(node2.input);

    int index=get_index_inputtemp("\\L");
    if(node1.inputstate==-1&&node1.outputstate==-1&&node2.inputstate==-1&&node2.outputstate==-1)
    {
        int index1=get_index_inputtemp(node1.input);
        int index2=get_index_inputtemp(node2.input);
        int x1=tempstate;
        tempstate++;
        nfatable[x1][index1].append(tostring(tempstate));
        nfatable[x1][index1].append(",");
        int x=tempstate;
        tempstate++;
        nfatable[x][index2].append(tostring(tempstate));
        nfatable[x][index2].append(",");

        retnode=Node(s,x1,tempstate);
        tempstate++;
    }
    else if(node1.inputstate==-1&&node1.outputstate==-1)
    {
        int index1=get_index_inputtemp(node1.input);
        nfatable[tempstate][index1].append(tostring(node2.inputstate));
        nfatable[tempstate][index1].append(",");
        retnode=Node(s,tempstate,node2.outputstate);
        tempstate++;
    }
    else if(node2.inputstate==-1&&node2.outputstate==-1)
    {
        int index2=get_index_inputtemp(node2.input);
        nfatable[node1.outputstate][index2].append(tostring(tempstate));
        nfatable[node1.outputstate][index2].append(",");
        retnode=Node(s,node1.inputstate,tempstate);
        tempstate++;
    }
    else
    {
        if(node1.outputstate!=node2.inputstate)
        {
            nfatable[node1.outputstate][index].append(tostring(node2.inputstate));
            nfatable[node1.outputstate][index].append(",");

        }

        retnode=Node(s,node1.inputstate,node2.outputstate);
    }
    return retnode;

}



vector<string> spliter(string reg_exp)
{
    vector<string> split;
    string sec="";
    for(int i=0; i<reg_exp.size(); i++)
    {
        if(reg_exp[i]=='|'||reg_exp[i]=='('||reg_exp[i]==')'||reg_exp[i]=='+'||reg_exp[i]=='*')
        {
            if(i-1>=0)
            {
                if(reg_exp[i-1]=='\\')
                {
                    sec+=reg_exp[i];
                    split.push_back(sec);
                    string w(1,reg_exp[i]);
                    addtoinput(w);
                    addtoinputtemp(w);
                    sec="";
                    continue;
                }
            }
            if(sec.size()!=0)
            {
                split.push_back(sec);
                addtoinput(sec);
                addtoinputtemp(sec);
                sec="";
            }
            sec+=reg_exp[i];
            split.push_back(sec);
            sec="";
        }
        else if(reg_exp[i]==' '||reg_exp[i]=='\t')
        {
            if(sec.size()!=0)
            {
                split.push_back(sec);
                addtoinput(sec);
                addtoinputtemp(sec);
                sec="";
            }
        }
        else if(reg_exp[i]=='=')
        {
            if(i-1>=0)
                if(reg_exp[i-1]=='\\')
                {
                    string f=sec.substr(0,sec.size()-1);
                    if(f.size()!=0)
                    {
                        split.push_back(f);
                        addtoinput(f);
                        addtoinputtemp(f);
                    }
                    sec="";
                }
            sec+=reg_exp[i];
        }
        else
        {
            sec+=reg_exp[i];
        }


    }
    if(sec.size()!=0)
    {
        split.push_back(sec);
        addtoinput(sec);
        addtoinputtemp(sec);
        sec="";
    }
    // add concatenation
    bool prev=false;
    vector<string> splitcon;
    for(int i=0; i<split.size(); i++)
    {
        if(split.at(i).compare("|")==0)
        {
            prev=false;
        }
        else if(split.at(i).compare("+")==0||split.at(i).compare("*")==0||split.at(i).compare(")")==0)
        {
            prev=true;
        }
        else if(split.at(i).compare("(")==0)
        {
            if(prev)
            {
                splitcon.push_back("con");
            }
            prev=false;
        }
        else
        {
            if(prev)
            {
                splitcon.push_back("con");
            }
            prev=true;
        }
        splitcon.push_back(split[i]);
    }

    return splitcon;
}



int get_priority(string s)
{
    int ret=0;
    if(s.compare("|")==0)
    {
        return 1;
    }
    if(s.compare("+")==0)
    {
        return 3;
    }
    if(s.compare("*")==0)
    {
        return 3;
    }
    if(s.compare("con")==0)
    {
        return 2;
    }

    return ret;
}

vector<string> Expression_evaluater(string reg_exp)
{
    vector<string> split=spliter(reg_exp);
    stack<string> mystack;
    bool prev_bool=false;
    vector<string> expre_eval;
    for(int i=0; i<split.size(); i++)
    {
        if(split.at(i).compare("|")==0||split.at(i).compare("+")==0||split.at(i).compare("*")==0||split.at(i).compare("(")==0||split.at(i).compare(")")==0||split.at(i).compare("con")==0)
        {
            if(split.at(i).compare("(")==0)
            {
                mystack.push(split[i]);
            }
            else if(split.at(i).compare(")")==0)
            {
                while(mystack.top().compare("(")!=0)
                {
                    expre_eval.push_back(mystack.top());
                    mystack.pop();
                }
                mystack.pop();
            }
            else
            {
                while(!mystack.empty())
                {
                    if(get_priority(mystack.top())>=get_priority(split[i]))
                    {
                        expre_eval.push_back(mystack.top());
                        mystack.pop();
                    }
                    else
                    {
                        break;
                    }
                }
                mystack.push(split[i]);
            }

        }
        else
        {
            expre_eval.push_back(split[i]);
        }

    }
    while(!mystack.empty())
    {
        expre_eval.push_back(mystack.top());
        mystack.pop();
    }

    addtoinputtemp("\\L");
    int index=get_index_inputtemp("\\L");
    if(index!=(tempnuminput-1))
    {
        string t=tempinput[tempnuminput-1];
        tempinput[index]=t;
        tempinput[tempnuminput-1]="\\L";
    }

    return expre_eval;

}


void check()
{
    map<string, string>:: iterator itr;
    string deffirst[50];
    string defsecond[50];
    string expfirst[50];
    string expsecond[50];
    int index1=0,index2=0;
    for (itr = Regular_definitions.begin(); itr != Regular_definitions.end(); ++itr)
    {
        deffirst[index1]=itr->first;
        defsecond[index1]=itr->second;
        index1++;
    }
    for (itr = Regular_expressions.begin(); itr != Regular_expressions.end(); ++itr)
    {
        expfirst[index2]=itr->first;
        expsecond[index2]=itr->second;
        index2++;
    }

    // for regular def
    for (int m=0; m<index1; m++)
    {
        string str=defsecond[m];
        string strcutt="";
        int fir=0;
        for(int i=0; i<str.length(); i++)
        {
            if((str[i]<='z'&&str[i]>='a')||(str[i]<='Z'&&str[i]>='A'))
            {
                strcutt.push_back(str[i]);
            }
            else
            {
                if(strcutt.length()!=0&&strcutt.length()!=1)
                {
                    for(int j=0; j<index1; j++)
                    {
                        if(strcutt.compare(deffirst[j])==0)
                        {
                            string strx="";
                            strx+="(";
                            strx+=defsecond[j];
                            strx+=")";
                            str.replace(fir,strcutt.length(),strx);
                            i=fir+strx.length();
                        }
                    }
                }
                fir=i+1;
                strcutt="";
            }
        }
        defsecond[m]=str;
    }

    //for regular expression

    for (int m=0; m<index2; m++)
    {
        string str=expsecond[m];
        string strcutt="";
        int fir=0;
        for(int i=0; i<str.length(); i++)
        {
            if((str[i]<='z'&&str[i]>='a')||(str[i]<='Z'&&str[i]>='A'))
            {
                strcutt.push_back(str[i]);
            }
            else
            {
                if(strcutt.length()!=0&&strcutt.length()!=1)
                {
                    for(int j=0; j<index1; j++)
                    {
                        if(strcutt.compare(deffirst[j])==0)
                        {
                            string strx="";
                            strx+="(";
                            strx+=defsecond[j];
                            strx+=")";
                            str.replace(fir,strcutt.length(),strx);
                            i=fir+strx.length();
                        }
                    }
                }
                strcutt="";
                fir=i+1;
            }
        }
        expsecond[m]= str;
    }

    Regular_expressions.clear();
    Regular_definitions.clear();

    for(int i=0; i<index1; i++)
    {
        Regular_definitions.insert(pair<string, string>(deffirst[i],defsecond[i]));
    }
    for(int i=0; i<index2; i++)
    {
        Regular_expressions.insert(pair<string, string>(expfirst[i],expsecond[i]));
    }

}

void createNFA()
{

    Parsefile();
    check();
/// punctuation State
    initialize_table();
    for(int i=0; i<Punctuations.size(); i++)
    {
        string w(1,Punctuations[i]);
        table[0][get_index_input(w)]=Numstate;
        State[Numstate]=w;
        Numstate++;
    }
    /// keyword state
    for(int i=0; i<Keywords.size(); i++)
    {
        int tempstate=0;
        for(int j=0; j<Keywords[i].size(); j++)
        {
            string w(1,Keywords[i][j]);
            int index=get_index_input(w);
            if(j==0)
            {
                if(table[0][index]==-1)
                {
                    table[0][index]=Numstate;
                }
                else
                {
                    tempstate=table[0][index];
                }
            }
            else
            {
                if(tempstate!=0)
                {
                    if(table[tempstate][index]==-1)
                    {
                        table[tempstate][index]=Numstate;
                        tempstate=0;
                    }
                    else
                    {
                        tempstate=table[tempstate][index];
                    }
                }
                else
                {
                    table[Numstate][index]=++Numstate;
                }

            }
        }

        State[Numstate]=Keywords[i];
        Numstate++;
    }

    /*  for(int i=0;i<Numinput;i++){
           cout<<input[i]<<" ";
       }
       cout<<"\n";
    for(int i=0;i<Numstate;i++){
       for(int j=0;j<Numinput;j++){
           cout<<table[i][j]<<" ";
       }
       cout<<"\n";
    }

    */
    map<string, string>:: iterator itr;
    for (itr = Regular_expressions.begin(); itr != Regular_expressions.end(); ++itr)
    {
        cout << '\t' << itr->first
             << '\t' << itr->second << '\n';
    }
    cout << endl;
    for (itr = Regular_definitions.begin(); itr != Regular_definitions.end(); ++itr)
    {
        cout << '\t' << itr->first
             << '\t' << itr->second << '\n';
    }
    cout << endl;



}

vector <int> get_integers(string state)
{
    string sec="";
    vector<int> closer;
    for(int i=0; i<state.size(); i++)
    {
        if(state[i]==',')
        {
            if(sec.size()!=0)
            {
                closer.push_back(tointeger(sec));
                sec="";
            }
        }
        else
        {
            sec+=state[i];
        }
    }

    return closer;
}

bool it_contain(vector<int> s,int w)
{
    for(int i=0; i<s.size(); i++)
    {
        if(s[i]==w)
        {
            return true;
        }
    }
    return false;
}
vector<int> getclosure(string state)
{
    int index=get_index_inputtemp("\\L");
    vector<int> closer=get_integers(state);
    for(int i=0; i<closer.size(); i++)
    {
        if(nfatable[closer[i]][index].size()!=0)
        {
            string s=nfatable[closer[i]][index];
            vector<int> closertemp=get_integers(s);
            for(int j=0; j<closertemp.size(); j++)
            {
                if(!it_contain(closer,closertemp[j]))
                    closer.push_back(closertemp[j]);
            }
        }
    }
    return closer;
}
vector< vector<int> > evaluate;////////////////////initialize it
vector<string> stringstate;///////////////////
string evaluate_without_dublicate(string s)
{
    bool flag=false;
    int index=0;
    vector<int> input=get_integers(s);

    sort( input.begin(), input.end() );
    input.erase( unique( input.begin(), input.end() ), input.end() );

    for(int i=0; i<evaluate.size(); i++)
    {
        bool flag1=false;
        if(evaluate[i].size()==input.size())
        {
            for(int j=0; j<evaluate[i].size(); j++)
            {
                if(evaluate[i][j]!=input[j])
                {
                    flag1=true;
                    break;
                }
            }
            if(flag1==false)
            {
                index=i;
                flag=true;
                break;
            }
        }
    }
    if(flag)
    {
        return stringstate[index];
    }
    else
    {
        string t="";
        for(int i=0; i<input.size(); i++)
        {
            t.append(tostring(input[i]));
            t.append(",");
        }
        stringstate.push_back(t);
        evaluate.push_back(input);
        return t;
    }
}

void minimize()
{



}

bool contain_output(vector<int> lis,int x)
{
    for(int i=0; i<lis.size(); i++)
    {
        if(x==lis[i])
        {
            return true;
        }
    }
    return false;
}


void nfa_to_dfa()
{

    map<string,int> statemap;
    vector< vector<int> > listt;
    int state=1;
    string c=tostring(ashin);
    c.append(",");
    statemap[c]=0;
    listt.push_back(getclosure(c));

    for(int j=0; j<listt.size(); j++)
    {
        for(int k=0; k<tempnuminput-1; k++)
        {
            string s="";
            for(int i=0; i<listt[j].size(); i++) /////
            {
                s.append(nfatable[listt[j][i]][k]);//////////
            }
            //search map it contain s
            string f=evaluate_without_dublicate(s);
            if(statemap.count(f))
            {
                dfatable[j][k]=statemap.at(f);/////////
            }
            else
            {
                statemap[f]=state;
                dfatable[j][k]=state;/////////
                listt.push_back(getclosure(f));
                state++;
            }
            //if contain output
            int x=ashout;
            if(contain_output(listt[j],x))
            {
                dfaoutput[j]=true;
            }
            else
            {
                dfaoutput[j]=false;
            }

        }
    }
    size_of_nfa=state;
}

void Dfa(vector<string> expre_eval)
{
    stack<Node> stack2;

    for(int i=0; i<expre_eval.size(); i++)
    {
        if(expre_eval.at(i).compare("|")==0)
        {
            struct Node node1=stack2.top();
            stack2.pop();
            struct Node node2=stack2.top();
            stack2.pop();
            struct Node node3=disjunction(node1,node2);
            stack2.push(node3);
        }
        else if(expre_eval.at(i).compare("+")==0)
        {
            struct Node node1=stack2.top();
            stack2.pop();
            struct Node node3=positive_closure(node1);
            stack2.push(node3);
        }
        else if(expre_eval.at(i).compare("*")==0)
        {
            struct Node node1=stack2.top();
            stack2.pop();
            struct Node node3=closure(node1);
            stack2.push(node3);
        }
        else if(expre_eval.at(i).compare("con")==0)
        {
            struct Node node1=stack2.top();
            stack2.pop();
            struct Node node2=stack2.top();
            stack2.pop();
            struct Node node3=concatenation(node2,node1);
            stack2.push(node3);
        }
        else
        {
            struct Node mynode=Node(expre_eval.at(i),-1,-1);
            stack2.push(mynode);
        }

    }
    struct Node my=stack2.top();
    ashin=my.inputstate;
    ashout=my.outputstate;

    nfa_to_dfa();
    cout<<tempstate;
    cout<<"\n";
    for(int i=0; i<tempstate; i++)
    {
        for(int j=0; j<4; j++)
        {
            cout<<nfatable[i][j]<<"    ";
        }
        cout<<"\n";
    }
    cout<<size_of_nfa<<"\n";
    for(int i=0; i<size_of_nfa; i++)
    {
        for(int j=0; j<tempnuminput; j++)
        {
            cout <<" "<<dfatable[i][j];
        }
        cout<<"\n";
        cout <<dfaoutput[i];
        cout<<"\n";
    }



}

vector< vector<int> >realmap;
vector< vector<int> > fakemap;

void initialize_tabletemp()
{
    for(int i=0; i<Maxtemp; i++)
    {
        dfaoutput[i]=false;
        tempinput[i].clear();
        for(int j=0; j<Maxtemp; j++)
        {
            dfatable[i][j]=-1;
            nfatable[i][j].clear();
        }
    }

}

void initializesmalltable()
{
    fakemap.clear();
    realmap.clear();
    initialize_tabletemp();
    evaluate.clear();////////////////////initialize it
    stringstate.clear();///////////////////
    tempstate=0;/////initialize it
    tempnuminput=0;/////initialize it
    ashin=0;
    ashout=0;
    size_of_nfa=0;

}

void concatenate_to_table(string exp_output)
{
    map<int,int> to_col;
    int index=get_index_input(tempinput[0]);
    if(table[0][index]==-1)
    {
        table[0][index]=Numstate;
        vector<int> real1;
        real1.push_back(dfatable[0][0]);
        real1.push_back(Numstate);
        realmap.push_back(real1);
        to_col[dfatable[0][0]]=Numstate;
        Numstate++;
    }
    else
    {
        vector<int> real1;
        real1.push_back(dfatable[0][0]);
        real1.push_back(table[0][index]);
        realmap.push_back(real1);
        to_col[dfatable[0][0]]=table[0][index];
    }

    for(int i=0; i<realmap.size(); i++)
    {
        int x1=0;
        int x2=0;
        if(i==0)
        {
            x1=0;
            x2=0;
        }
        else
        {
            x1=realmap[i][0];
            x2=realmap[i][1];
        }
        if(dfaoutput[x1]==true)
        {
            if(State.count(x2)==0)
            {
                State[x2-1]=exp_output;
            }
        }
        for(int j=0; j<(tempnuminput-1); j++)
        {
            int index=get_index_input(tempinput[j]);
            if(table[x2][index]==-1)
            {
                if(to_col.count(dfatable[x1][j])!=0)
                {
                    table[x2][index]=to_col.at(dfatable[x1][j]);
                }
                else
                {
                    table[x2][index]=Numstate;
                    to_col[dfatable[x1][j]]=Numstate;
                    vector<int> real1;
                    real1.push_back(dfatable[x1][j]);
                    real1.push_back(Numstate);
                    realmap.push_back(real1);
                    Numstate++;
                }
            }
            else
            {
                /// fake state
                if(to_col.count(dfatable[x1][j])!=0)
                {
                    if(to_col.at(dfatable[x1][j])!=table[x2][index])
                    {
                        vector<int> fak11;
                        fak11.push_back(dfatable[x1][j]);
                        fak11.push_back(table[x2][index]);
                        fakemap.push_back(fak11);
                    }
                }
                else
                {
                    to_col[dfatable[x1][j]]=table[x2][index];
                    vector<int> real1;
                    real1.push_back(dfatable[x1][j]);
                    real1.push_back(table[x2][index]);
                    realmap.push_back(real1);
                }

            }

        }

    }

}

void concatenate_to_all()
{
    vector<string> expre_eval;
  //  Dfa(Expression_evaluater(Regular_expressions.at("relop")));
    //concatenate_to_table("relop");

    map<string, string>:: iterator itr;
        for (itr = Regular_expressions.begin(); itr != Regular_expressions.end(); ++itr){
            cout << '\t' << itr->first
                 << '\t' << itr->second << '\n';
                    initializesmalltable();
                 expre_eval=Expression_evaluater(itr->second);
                 if(expre_eval.size()>1){
                        Dfa(expre_eval);
                        concatenate_to_table(itr->first);
                 }else{
                int index=get_index_input(expre_eval[0]);
                table[0][index]=Numstate;
                State[Numstate]=itr->first;
                Numstate++;
                 }
        }
        cout << endl;


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


void readcode()
{
     map<int, string>:: iterator itr;
    for (itr = State.begin(); itr != State.end(); ++itr)
    {
        cout << '\t' << itr->first
             << '\t' << itr->second << '\n';
    }
    cout << endl;

    string line = "";
    cout<<"\n";
    ifstream infile("input1.txt");

    while (getline(infile, line))  //read line by line
    {
        string token="";
        line.append(" ");
        for(int j=0; j<line.size(); j++)
        {
            if(line[j]!='\t'&&line[j]!=' '&&line[j]!='\n'&&line[j]!='\r')
            {
                token+=line[j];
            }
            else
            {
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
                                cout<<st.output;
                                cout<<"\n";
								statenum=0;
                                     while(!stack1.empty()){
                                    stack1.pop();
                                }
                                i=st.pos;//////////////
                                continue;
                            }
                        }


                        statenum=table[statenum][index];
                        if(State.count(statenum)!=0)
                        {
                            struct Statepos st1=Statepos(State.at(statenum),i);
                            stack1.push(st1);
                        }

                        if(i==token.size()-1)
                        {
                            if(State.count(statenum)!=0)
                            {
                                cout<<State.at(statenum);
                                cout<<"\n";
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
                                    cout<<st.output;
                                    cout<<"\n";
									statenum=0;
                                     while(!stack1.empty()){
                                    stack1.pop();
                                }
                                    i=st.pos;//////////////
                                }

                            }
                        }
                    }
                    token="";
                }
            }
        }

    }

}

int main()
{

    createNFA();
    concatenate_to_all();
    for(int i=0; i<Numinput; i++)
    {
        cout<<input[i]<<" ";
    }
    cout<<"\n";
    for(int i=0; i<Numstate; i++)
    {
        for(int j=0; j<Numinput; j++)
        {
            cout<<table[i][j]<<" ";
        }
        cout<<"\n";
    }
    readcode();
    cout << "Hello world!" << endl;
    return 0;
}


