#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include<conio.h>
#include<string.h>
#include<stdio.h>
#include<map>
#include <sstream>
#include <bits/stdc++.h>
#include<stack>
#define MAX 1000
using namespace std;
vector<vector<string> >cfg_table(MAX);
vector<vector<string> >first_tab;
vector<string>first_temp;
vector<vector<string> >follow_tab;
vector<string> terminals;
vector<vector<string> >refr_first;
string production[MAX][MAX];

////////////////////////////////////
int row=44;
int col=31;
int table [44][31];
string input[31];
map<int, string>state;
vector<string> rangs;
vector<string> tokens;

int tointeger(string s)
{
    stringstream geek(s);
    int x = 0;
    geek >> x;
    return x;
}


void initializer_output()
{
    string line = "";
    ifstream infile("stateoutput.txt");
    while (getline(infile, line))  //read line by line
    {
        string sec="";
        int Numstate=0;
        bool flag=true;
        for(int i=1; i<line.size(); i++)
        {
            if(line[i]=='}'&&flag)
            {
                Numstate=tointeger(sec);
                sec="";
                flag=false;
                i++;
            }
            else
            {
                sec+=line[i];
            }
        }
        state[Numstate]=sec;
    }

}
void initializer_input()
{
    string line = "";
    ifstream infile("stateinput.txt");
    int indexinput=0;
    while (getline(infile, line))  //read line by line
    {
        string sec="";
        for(int i=0; i<line.size(); i++)
        {
            if(line[i]=='\t')
            {
                if(sec.size()!=0)
                {
                    input[indexinput++]=sec;
                    if(sec.size()>1)
                    {
                        rangs.push_back(sec);
                    }
                    sec="";
                }
            }
            else
            {
                sec+=line[i];
            }
        }
        if(sec.size()!=0)
        {
            input[indexinput++]=sec;
            if(sec.size()>1)
            {
                rangs.push_back(sec);
            }
            sec="";
        }
    }
}
void initializer_table()
{
    string line = "";
    ifstream infile("statetable.txt");
    int indexrow=0;
    int indexcol=0;
    while (getline(infile, line))  //read line by line
    {
        string sec="";
        for(int i=0; i<line.length(); i++)
        {
            if(line[i]=='-')
            {
                table[indexrow][indexcol++]=-1;
            }
            else
            {
                if(line[i]=='\t')
                {
                    if(sec.size()!=0)
                    {
                        table[indexrow][indexcol++]=tointeger(sec);
                        sec="";
                    }
                }
                else
                {
                    sec+=line[i];
                }
            }
        }
        if(sec.size()!=0)
        {
            table[indexrow][indexcol++]=tointeger(sec);
            sec="";
        }
        indexrow++;
        indexcol=0;
    }
}
void initializerall()
{
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


void readcode()
{
    string line = "";
    ifstream infile("test_1.txt");

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
                                if(!stack1.empty())
                                {
                                    struct Statepos st=stack1.top();
                                    stack1.pop();
                                    tokens.push_back(st.output);
                                    while(!stack1.empty())
                                    {
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
                                tokens.push_back(st.output);
                                i=st.pos;//////////////
                                statenum=0;
                                while(!stack1.empty())
                                {
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
                                tokens.push_back(state.at(statenum));
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
                                    state.at(statenum);
                                    statenum=0;
                                    while(!stack1.empty())
                                    {
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


//////////////////////////////////

vector<string>concatenate(string nontermenal,string target,vector<string> terminal)
{
    vector<string> result;
    if(nontermenal.size()!=0)
    {
        result.push_back(nontermenal);
    }
    for(int i=0; i<terminal.size(); i++)
    {
        string s=terminal[i];
        s.append(" ");
        s.append(target);
        result.push_back(s);
    }
    return result;
}


vector<string> assign_non_terminals(vector<vector<string> >lexecaltemp,int x)
{
    vector<string>result;
    vector<string> target=cfg_table[x];
    result.push_back(target[0]);
    for(int i=1; i<target.size(); i++)
    {
        size_t found=target[i].find(" ");
        string s="";
        string reminder="";
        if (found == string::npos)
        {
            s=target[i];
        }
        else
        {
            s=target[i].substr(0,found);
            reminder= target[i].substr(found+1,(target[i].length()-found));
        }
        bool flag_found=false;
        for(int l=0; l<lexecaltemp.size(); l++)
        {
            if(s.compare(lexecaltemp[l][0])==0)
            {
                vector<string> concate;
                if(reminder.size()==0)
                {
                    concate=lexecaltemp[l];
                }
                else
                {
                    concate=concatenate("",reminder,lexecaltemp[l]);
                }
                for(int j=1; j<concate.size(); j++)
                {
                    result.push_back(concate[j]);
                }
                flag_found=true;
                break;
            }
        }
        if(!flag_found)
        {
            result.push_back(target[i]);
        }
    }
    return result;
}
void remove_left_recurgion()
{
    vector<vector<string> >lexecaltemp;
    for(int i=0; i<cfg_table.size(); i++)
    {
        vector<string> line;
        if(i>0)
        {
            line=assign_non_terminals(lexecaltemp,i);
        }
        else
        {
            line=cfg_table[i];
        }
        string nonterm=line[0];
        vector<string> beta_term;
        vector<string> alpha_term;
        bool betabool=false;
        for(int j=1; j<line.size(); j++)
        {
            size_t found = line[j].find(" ");
            string sec="";
            if (found == string::npos)
            {
                found=-1;
                sec=line[j];
            }
            else
            {
                sec=line[j].substr(0,found);
            }

            if(nonterm.compare(sec)==0)
            {
                if(found!=-1)
                {
                    alpha_term.push_back(line[j].substr(found+1,(line[j].length()-found)));
                }
                else
                {
                    betabool=true;
                }
            }
            else
            {
                beta_term.push_back(line[j]);
            }
        }
        if(betabool)
        {
            string nonterm1=nonterm;
            nonterm1+='`';
            //add beta term
            lexecaltemp.push_back(concatenate(nonterm,nonterm1,beta_term));
            //add alpha term
            vector<string> alpha;
            alpha.push_back(nonterm1);
            alpha.push_back("\\L");
            lexecaltemp.push_back(alpha);

        }
        else if(alpha_term.size()==0)
        {
            lexecaltemp.push_back(line);
        }
        else
        {
            string nonterm1=nonterm;
            nonterm1+='`';
            //add beta term
            lexecaltemp.push_back(concatenate(nonterm,nonterm1,beta_term));
            //add alpha term
            vector<string> alpha=concatenate(nonterm1,nonterm1,alpha_term);
            alpha.push_back("\\L");
            lexecaltemp.push_back(alpha);
        }
    }
    cfg_table.clear();
    cfg_table=lexecaltemp;

}


string concaten(string str1,string str2)
{
    string result="";
    int x=0;
    if(str1[0]=='\''&&str2[0]=='\'')
    {
        x=1;

    }
    else
    {
        x=0;
    }

    int size1=0;
    if(str1.size()<str2.size())
    {
        size1=str1.size();
    }
    else
    {
        size1=str2.size();
    }
    int i=0;
    for(i=x; i<size1; i++)
    {
        if(str1[i]==str2[i])
        {
            result+=str1[i];
        }
        else
        {
            break;
        }
    }
    if(!(str1[i]==' '||str2[i]==' '))
    {
        return "";
    }

    if(result[result.size()-1]==' ')
    {
        result=result.substr(0,result.size()-1);
    }

    return result;
}



void remove_left_factor()
{
    vector<vector<string> > lexecaltemp;
    for(int i=0; i<cfg_table.size(); i++)
    {
        lexecaltemp.push_back(cfg_table[i]);
        int f=lexecaltemp.size()-1;
        for(int j=f; j<lexecaltemp.size(); j++)
        {
            vector<string> token=lexecaltemp[j];
            for(int k=1; k<token.size(); k++)
            {
                string factor=token[k];
                vector<string> isfactor;
                isfactor.push_back(token[k]);
                vector<string> isnotfactotr;
                for(int m=k+1; m<token.size(); m++)
                {
                    string con=concaten(factor,token[m]);
                    if(con.length()==0)
                    {
                        isnotfactotr.push_back(token[m]);
                    }
                    else
                    {
                        factor=con;
                        isfactor.push_back(token[m]);
                    }
                }
                if(isfactor.size()>1)
                {
                    vector <string> vector1;
                    vector1.push_back(lexecaltemp[j][0]);
                    /// add not factor
                    for(int h=0; h<isnotfactotr.size(); h++)
                    {
                        vector1.push_back(isnotfactotr[h]);
                    }
                    for(int h=1; h<k; h++)
                    {
                        vector1.push_back(token[h]);//////////////////////////////////////////////////////////////////////
                    }

                    ///add factor term
                    int dif=lexecaltemp.size()-j;
                    string st="";
                    while(dif--)
                    {
                        st+="`";
                    }
                    string app=factor;
                    app+=" ";
                    app+=lexecaltemp[j][0];
                    app+=st;
                    vector1.push_back(app);
                    lexecaltemp[j]=vector1;
                    // lexecaltemp[j].swap(vector1);  // editing this nonterminal
                    vector1.clear();
                    //add new non terminal
                    vector<string>vector2;
                    app="";
                    app+=lexecaltemp[j][0];
                    app+=st;
                    vector2.push_back(app);

                    for(int h=0; h<isfactor.size(); h++)
                    {
                        string s=isfactor[h].substr(factor.size(),(isfactor[h].length()-factor.size()));
                        if(s.size()==0)
                        {
                            vector2.push_back("\\L");
                        }
                        else
                        {
                            if(s[0]==' ')
                            {
                                s=s.substr(1,s.size()-1);
                            }
                            vector2.push_back(s);
                        }
                    }


                    lexecaltemp.push_back(vector2);
                    vector2.clear();

                    k=0;////////////////////////////////////////////////
                    token.clear();
                    token=lexecaltemp[j];

                }

            }
        }

    }
    /*
     for(int i=0; i<lexecaltemp.size(); i++)
        {
            for(int j=0; j<lexecaltemp[i].size(); j++)
            {
                cout<<(lexecaltemp[i][j])<<"    ";
            }
            cout<<"\n";
        }
    */

    cfg_table.clear();
    cfg_table=lexecaltemp;
}

////////////////////////////////////////////
void getTerminals(string s)
{
    string terminal="";
    for(int i=0; i<s.size(); i++)
    {
        if(s.at(i)=='\'')
        {
            terminal+=(s.at(i));
            for(int j=i+1; j<s.size(); j++)
            {
                if((s.at(j)!='\''))
                {
                    terminal+=s.at(j);
                }
                else
                {
                    terminal+=s.at(j);
                    terminals.push_back(terminal);
                    i=j+1;
                    terminal="";
                    break;
                }
            }
        }
    }
}
void eliminate_terminals()
{
    for(int i=0; i<terminals.size(); i++)
    {
        for(int j=i+1; j<terminals.size(); j++)
        {
            if(terminals[i].compare(terminals[j])==0)
            {
                terminals.erase(terminals.begin()+j);
                j--;
            }
        }
    }
}
vector<string> readFile(string file)
{
    string line;
    vector<string> table;
    ifstream input_file (file.c_str(), ifstream::in);
    if(input_file.is_open())
    {
        while (getline(input_file,line))
        {
            table.push_back(line);
            getTerminals(line);
        }
        terminals.push_back("$");
        input_file.close();
    }
    else
    {
        cout << "error";
    }
    return table;
}
void write_file(string file,vector<vector<string> >table_tem)
{
    ofstream out_file(file.c_str(), ofstream::out);
    for(int i=0; i<table_tem.size(); i++)
    {
        for(int j=0; j<table_tem[i].size(); j++)
        {
            out_file << table_tem[i][j] << "\t";
        }
        out_file << "\n";
    }
    out_file.close();
}
vector<string> merge_lines()
{
    string file_name ="cgf.txt";
    vector<string>table;
    table=readFile(file_name);

    for(int i=0; i<table.size(); i++)
    {
        if(table[i].at(0)!='#')
        {
            for(int n=0; n<table[i].size(); n++)
            {
                if(table[i].at(n)==':')
                {
                    table[i].erase(0,n+3);
                    table[i-1].append("|");
                }
            }
            table[i-1].append(table[i]);
            table.erase(table.begin()+i);
            i=i-1;
        }
        if(table[i].at(table[i].size()-1)=='|')
        {
            table[i].append(table[i+1]);
            table.erase(table.begin()+i+1);
        }
        table[i].erase(std::remove(table[i].begin(),table[i].end(),'#'),table[i].end());
    }
    for(int i=0; i<table.size(); i++)
    {
        for(int k=0; k<table[i].size(); k++)
        {
            char c=table[i].at(k);
            if((table[i].at(k)==' ')||(table[i].at(k)=='\t'))
            {
                table[i].erase(k,1);
            }
            else
            {
                break;
            }
        }
    }
    return table;
}
void get_cfg_table()
{
    vector<string>table;
    int tab_index=0;
    table=merge_lines();
    for(int i=0; i<table.size(); i++)
    {
        cout << table[i]<<"\n";
    }
    string buffer ="";
    for(int i=0; i<table.size(); i++)
    {
        string tem =table[i];
        for(int j=0; j<table[i].length(); j++)
        {
            char c=tem.at(j);
            if(((c==':')&&((tem.at(j-1)!='\'')&&(tem.at(j+1)!='\'')))||(c=='|'))
            {
                cfg_table[tab_index].push_back(buffer);
                buffer="";
                if(table[i].at(j)==':')
                {
                    j=j+2;
                }
            }
            else
            {
                buffer+=c;
            }
        }
        cfg_table[tab_index].push_back(buffer);
        buffer="";
        tab_index++;
    }
    for(int i=0; i<tab_index; i++)
    {
        for(int j=0; j<cfg_table[i].size(); j++)
        {
            cout << cfg_table[i][j] << " ";
        }
        cout << "\n";
    }
    cfg_table.resize(tab_index);
}
int check_lRec()
{
    vector<vector<string> >temp(cfg_table.size());
    temp=cfg_table;
    for(int i=0; i<temp.size(); i++)
    {
        for(int j=0; j<temp[i].size(); j++)
        {
            if(temp[i][j].at(0)==' ')
            {
                temp[i][j].erase(0,1);
            }
        }
    }
    string buffer ="";
    for(int i=0; i<temp.size(); i++)
    {
        for(int j=1; j<temp[i].size(); j++)
        {
            for(int k=0; k<temp[i][j].size(); k++)
            {
                if(temp[i][j].at(k)!=' ')
                {
                    buffer+=temp[i][j].at(k);
                }
                else
                {
                    break;
                }
            }
            if(temp[i][0].compare(buffer)==0)
            {
                return 0;
            }
            buffer="";
        }
    }
    return 1;
}
bool isTerminal(string s)
{
    if((s[0]=='\'')&&(s[s.size()-1]=='\''))
    {
        return true;
    }
    return false;
}
void remove_spaces()
{
    for(int i=0; i<cfg_table.size(); i++)
    {
        for(int j=0; j<cfg_table[i].size(); j++)
        {
            char c;
            for(int k=0; k<cfg_table[i][j].size(); k++)
            {
                if((cfg_table[i][j].at(k)==' ')||(cfg_table[i][j].at(k)=='\t'))
                {
                    cfg_table[i][j].erase(k,1);
                }
                else
                {
                    break;
                }
            }
            for(int k=cfg_table[i][j].size()-1; k>0; k--)
            {
                if((cfg_table[i][j].at(k)==' ')||(cfg_table[i][j].at(k)=='\t'))
                {
                    cfg_table[i][j].erase(k,1);
                }
                else
                {
                    break;
                }
            }
        }
    }
}
void remove_dec_spaces()
{
    for(int i=0; i<cfg_table.size(); i++)
    {
        for(int j=0; j<cfg_table[i].size(); j++)
        {
            if((cfg_table[i][j].at(0)==' ')||(cfg_table[i][j].at(0)=='\t'))
            {
                cfg_table[i][j].erase(0,1);
            }
        }
    }
    remove_spaces();
}
void nonTerminal_first(string s,int index1,int index2)
{
    for(int i=0; i<cfg_table.size(); i++)
    {
        if((cfg_table[i][0].compare(s))==0)
        {
            for(int j=1; j<cfg_table[i].size(); j++)
            {
                string buffer="";
                for(int k=0; k<cfg_table[i][j].size(); k++)
                {
                    if(cfg_table[i][j].at(k)==' ')
                    {
                        break;
                    }
                    buffer+=cfg_table[i][j].at(k);
                }
                if(isTerminal(buffer))
                {
                    refr_first[index1].push_back(cfg_table[index1][index2]);
                    first_temp.push_back(buffer);
                }
                else if((buffer.compare("\\L"))==0)
                {
                    refr_first[index1].push_back(cfg_table[index1][index2]);
                    first_temp.push_back(buffer);
                }
                else
                {
                    nonTerminal_first(buffer,index1,index2);
                }
            }
        }
    }
    return;
}

void first()
{
    first_tab.resize(cfg_table.size());
    refr_first.resize(cfg_table.size());
    for(int i=0; i<cfg_table.size(); i++)
    {
        first_tab[i].push_back(cfg_table[i][0]);
        refr_first[i].push_back(cfg_table[i][0]);
        for(int j=1; j<cfg_table[i].size(); j++)
        {
            string buffer="";
            for(int k=0; k<cfg_table[i][j].size(); k++)
            {
                if(cfg_table[i][j].at(k)==' ')
                {
                    break;
                }
                buffer+=cfg_table[i][j].at(k);
            }
            if(!buffer.empty())
            {
                if(isTerminal(buffer))
                {
                    first_tab[i].push_back(buffer);
                    refr_first[i].push_back(cfg_table[i][j]);
                }
                else if((buffer.compare("\\L"))==0)
                {
                    first_tab[i].push_back(buffer);
                    refr_first[i].push_back(cfg_table[i][j]);
                }
                else
                {
                    nonTerminal_first(buffer,i,j);
                    // refr_first[i].push_back(cfg_table[i][j]);
                    for(int d=0; d<first_temp.size(); d++)
                    {
                        if(!first_temp[d].empty())
                        {
                            first_tab[i].push_back(first_temp[d]);
                        }
                    }
                    first_temp.clear();
                }
            }
        }
    }
}
int get_index(string s)
{
    for(int i=0; i<cfg_table.size(); i++)
    {
        if(s.compare(cfg_table[i][0])==0)
        {
            return i;
        }
    }
}
void follow_rec(string rule)
{
    int index=get_index(rule);
    int flag=0;
    if(!follow_tab[index].empty())
    {
        return;
    }
    follow_tab[index].push_back(rule);
    if(index==0)
    {
        follow_tab[index].push_back("$");
    }
    for(int i=0; i<cfg_table.size(); i++)
    {
        for(int j=1; j<cfg_table[i].size(); j++)
        {
            string buffer ="";
            for(int k=0; k<cfg_table[i][j].size(); k++)
            {
                if(cfg_table[i][j].at(k)!=' ')
                {
                    buffer+=cfg_table[i][j].at(k);
                }
                else
                {
                    if((rule.compare(buffer)==0)&&(rule.compare(cfg_table[i][0])!=0))
                    {
                        string buffer1="";
                        for(int d=k+1; d<cfg_table[i][j].size(); d++)
                        {
                            if(cfg_table[i][j].at(d)!=' ')
                            {
                                buffer1+=cfg_table[i][j].at(d);
                            }
                            else
                            {
                                break;
                            }
                        }
                        if(!buffer1.empty())
                        {
                            if(isTerminal(buffer1))
                            {
                                follow_tab[index].push_back(buffer1);
                            }
                            else
                            {
                                int ind =get_index(buffer1);
                                vector<string>temp=first_tab[ind];
                                for(int v=1; v<temp.size(); v++)
                                {
                                    if(temp[v]!="\\L")
                                    {
                                        follow_tab[index].push_back(temp[v]);
                                    }
                                    else
                                    {
                                        flag=1;
                                    }
                                }
                                if(flag==1)
                                {
                                    follow_rec(cfg_table[i][0]);
                                    for(int t=1; t<follow_tab[i].size(); t++)
                                    {
                                        follow_tab[index].push_back(follow_tab[i][t]);
                                    }
                                }
                            }
                        }
                    }
                    buffer="";
                }
            }
            if((rule.compare(buffer)==0)&&(rule.compare(cfg_table[i][0])!=0))
            {
                follow_rec(cfg_table[i][0]);
                for(int t=1; t<follow_tab[i].size(); t++)
                {
                    follow_tab[index].push_back(follow_tab[i][t]);
                }
            }
        }
    }
}
void follow()
{
    follow_tab.resize(cfg_table.size());
    for(int i=0; i<cfg_table.size(); i++)
    {
        follow_rec(cfg_table[i][0]);
    }
}
void remove_duplicates()
{
    for(int i=0; i<follow_tab.size(); i++)
    {
        for(int j=1; j<follow_tab[i].size(); j++)
        {
            for(int k=j+1; k<follow_tab[i].size(); k++)
            {
                if(follow_tab[i][j].compare(follow_tab[i][k])==0)
                {
                    follow_tab[i].erase(follow_tab[i].begin()+k);
                    k--;
                }
            }
        }
    }
}
void eliminate_first()
{
    for(int i=0; i<first_tab.size(); i++)
    {
        for(int j=1; j<first_tab[i].size(); j++)
        {
            for(int k=j+1; k<first_tab[i].size(); k++)
            {
                if(first_tab[i][j].compare(first_tab[i][k])==0)
                {
                    first_tab[i].erase(first_tab[i].begin()+k);
                    k--;
                }
            }
        }
    }
}
int get_ter_index(string s)
{
    for(int i=0; i<terminals.size(); i++)
    {
        if(s.compare(terminals[i])==0)
        {
            return i;
        }
    }
}
void prepare_productions()
{
    production[cfg_table.size()+1][terminals.size()+1];
    //  production.resize(cfg_table.size()+1);
    //  production[0].push_back(" ");
    //terminals
    for(int i=0; i<terminals.size(); i++)
    {
        production[0][i+1]=terminals[i];
    }
    //non terminals
    for(int i=0; i<cfg_table.size(); i++)
    {
        production[i+1][0]=cfg_table[i][0];
    }
    //provided transitions
    int index=0;
    for(int i=0; i<first_tab.size(); i++)
    {
        for(int j=1; j<first_tab[i].size(); j++)
        {
            if(first_tab[i][j].compare("\\L")!=0)
            {
                index= get_ter_index(first_tab[i][j]);
                if(!production[i+1][index+1].empty())
                {
                    production[i+1][index+1].append(",");
                    production[i+1][index+1].append(refr_first[i][j]);
                }
                else
                {
                    production[i+1][index+1]=refr_first[i][j];
                }
            }
            else
            {
                for(int k=1; k<follow_tab[i].size(); k++)
                {
                    index=get_ter_index(follow_tab[i][k]);
                    if(!production[i+1][index+1].empty())
                    {
                        production[i+1][index+1].append(",");
                        production[i+1][index+1].append("\\L");
                    }
                    else
                    {
                        production[i+1][index+1]="\\L";
                    }
                }
            }
        }
    }
    for(int i=0; i<follow_tab.size(); i++)
    {
        for(int j=1; j<follow_tab[i].size(); j++)
        {
            int index =get_ter_index(follow_tab[i][j]);
            if(production[i+1][index+1].empty())
            {
                production[i+1][index+1]="sync";
            }
        }
    }
}
void write_parser_table()
{
    ofstream out_file("parser_table.txt");
    for(int i=0; i<=cfg_table.size(); i++)
    {
        for(int j=0; j<=terminals.size(); j++)
        {
            if( production[i][j].empty())
            {
                out_file << "empty cell\t\t";
            }
            else
            {
                if(i==0)
                {
                    out_file << production[i][j] << "\t\t\t\t";
                }
                else
                {
                    out_file << production[i][j] << "\t\t";
                }
            }
        }
        out_file << "\n\n";
    }
    out_file.close();
}
/*
void prepare(){
    for(int i=0;i<=cfg_table.size();i++){
        for(int j=0;j<=terminals.size();j++){
            for(int k=0;k<production[i][j].size();k++){
                if(production[i][j].at(k)=='\''){
                    production[i][j].erase(k,1);
                    k--;
                }
            }
        }
    }
}
*/

void print_to_file(stack<string> stack1,vector<string> vector1,string errors,bool u)
{
    stack<string> stack3=stack1;
    if(!u)
    {
        ofstream outfile;
        outfile.open("output_file.txt",ios_base::app);
        string str1="";
        while(!stack3.empty())
        {
            str1+=stack3.top();
            str1+=" ";
            stack3.pop();
        }
        string str2="";
        for(int i=0; i<vector1.size(); i++)
        {
            str2+=vector1[i];
            str2+=" ";
        }
        outfile<<str1<<"\t"<<str2<<"\t"<<errors<<"\n";

    }
    else
    {
        ofstream out_file("output_file.txt");
        out_file <<"Stack \t\t\t\t input \t\t\t\t\t\t output \n";
    }
}


int getindexinput(string str)
{
    for(int i=1; i<terminals.size()+1; i++)
    {
        if(str.compare(production[0][i])==0)
        {
            return i;
        }
    }
    return 0;
}
int getindexterminal(string str)
{
    for(int i=1; i<cfg_table.size()+1; i++)
    {
        if(str.compare(production[i][0])==0)
        {
            return i;
        }
    }
    return 0;
}
string illegal_synch(string str)
{
    string errors="Error:(illegal ";
    errors+=str;
    errors+=")";
    return errors;
}

string missing_errors(string str)
{
    string errors="Error: missing ";
    str.erase(str.length()-1,1);
    str.erase(0,1);
    errors+=str;
    errors+=", inserted";
    return errors;
}
string illegal_errors(string terminl,string str)
{
    string errors="Error:(illegal ";
    errors+=terminl;
    errors+=") – discard ";

    str.erase(str.length()-1,1);
    str.erase(0,1);
    errors+=str;

    return errors;
}


void output_tokens()
{
    cout<<"\n";
    for(int i=0; i<terminals.size(); i++)
    {
        cout<<terminals[i]<<" ";
    }

//////initialize
    tokens.push_back("$");
    stack<string> mystack;
    mystack.push("$");
    mystack.push(production[1][0]);
    print_to_file(mystack,tokens,"",true);
//
    while(!mystack.empty())
    {
        string popstack="";
        string productions="";
        string currtoken="";
        if(tokens[0]!="$")
        {
            currtoken="'";
            currtoken+=tokens[0];
            currtoken+="'";
        }
        else
        {
            currtoken=tokens[0];
        }
        if(tokens.size()==0)
        {
            break;
        }
        if(tokens.size()==1&&mystack.size()==1)
        {
            print_to_file(mystack,tokens,"accept",false);
            break;
        }
        else if(tokens.size()>1&&mystack.size()==1)
        {
            print_to_file(mystack,tokens,"error inputs is too much",false);
            break;
        }
        else
        {
            popstack=mystack.top();
            ///if terminal
            if(popstack[0]=='\'')
            {
                if(popstack.compare(currtoken)==0)
                {
                    string match="match ";
                    match+=popstack;
                    print_to_file(mystack,tokens,match,false);
                    tokens.erase(tokens.begin());
                    mystack.pop();
                }
                else
                {
                    print_to_file(mystack,tokens,missing_errors(popstack),false);       //error unmatched
                    mystack.pop();
                }
            }
            else
            {
                productions=production[getindexterminal(popstack)][getindexinput(currtoken)];
                if(productions.compare("\\L")==0)
                {
                    string str1=popstack;
                    str1+="=>";
                    str1+=productions;
                    print_to_file(mystack,tokens,str1,false);
                    mystack.pop();
                }
                else if(productions.compare("sync")==0)                      //error
                {
                    print_to_file(mystack,tokens,illegal_synch(popstack),false);
                    mystack.pop();
                }
                else if(productions.size()==0)                                      //error
                {
                    print_to_file(mystack,tokens,illegal_errors(popstack,currtoken),false);
                    tokens.erase(tokens.begin());
                }
                else
                {
                    string str2=popstack;
                    str2+="=>";
                    str2+=productions;
                    print_to_file(mystack,tokens,str2,false);
                    mystack.pop();
                    string sec="";
                    vector<string> temp;
                    for(int j=0; j<productions.length(); j++)
                    {
                        if(productions[j]==' ')
                        {
                            temp.push_back(sec);
                            sec="";
                        }
                        else
                        {
                            sec+=productions[j];
                        }

                    }
                    temp.push_back(sec);
                    for(int j=temp.size()-1; j>=0; j--)
                    {
                        if(temp[j].size()!=0)
                        {
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
    //phase 1
    initializerall();
    readcode();
    for(int i=0; i<tokens.size(); i++)
    {
        cout<<tokens[i]<<"\n";
    }
    //

    get_cfg_table();
    remove_dec_spaces();
    if(check_lRec()==0)
    {
        cout << "ERROR : The Input Grammer Isn't LL(1) !\n";
        cout<<"recovery routine is called";
        remove_left_recurgion();
        remove_left_factor();
    }
    cout << "KKKKKKKKKKKKK\n";
    write_file("cfg_parsed.txt",cfg_table);
    cout << "KKKKKKKKKKKKK\n";
    first();

    follow();
    remove_duplicates();
    write_file("follow.txt",follow_tab);
    eliminate_terminals();
    cout<<"\n Terminals : \n";
    for(int i=0; i<terminals.size(); i++)
    {
        cout<<terminals[i]<<" ";
    }
    cout << "\n";
    cout << "ref: \n";
    for(int i=0; i<refr_first.size(); i++)
    {
        for(int j=0; j<refr_first[i].size(); j++)
        {
            cout << refr_first[i][j]<<" ";
        }
        cout << "\n";
    }
    cout << "\n";
    cout << "\n";
    prepare_productions();
    eliminate_first();
    write_file("first.txt",first_tab);
    //  prepare();
    write_parser_table();
    output_tokens();
    return 0;
}


