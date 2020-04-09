#include <iostream>
#include <fstream>
#include <regex>
#include <queue>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <list>
#include <queue>
#include <map>
#include <iterator>
#include <cctype>
#include <stack>
using namespace std;
std::list<string> l = {"STT","STA","STS","STCH","STX","STB",
                       "LDT","LDA","LDS","LDCH","LDX","LDB",
                       "TD","WD","RD","J","JLT","JLQ","JLG",
                       "ADD","SUB","MUL","DIV","RMO","COMP",
                       "COMPR","JSUB","RSUB","START","ADDR",
                       "SUBR","MULR","DIVR","BASE","NOBASE",
                       "WORD","RESW","RESB","BYTE","LTORG",
                       "EQU","ORG","END","TIX","TIXR","SUM","JEQ"
                      };
std::list<string> lsymbols = {"RMO",
                              "COMPR","START","ADDR",
                              "SUBR","MULR","DIVR",
                              "WORD","RESW","RESB","BYTE",
                              "END","TIXR"
                             };
struct Node
{
    char format;
    string location,label,mnomotic,operand,comment,line;
};
int pc=0;
std::map<std::string,string>  symbols;
std::map<std::string,string>  dependent;
std::map<std::string,string>  expression;
std::map<std::string,string>  label;
queue<Node> qNode;
queue<Node> qNodef;
stack<Node> sNode;
bool flag = false;
int counter = 0;
int lin = 1;
bool IsThisStringAHexNumber(std::string const &str);
void readFile(string path);
void checkError(Node node);
void getSymbol(Node node);
void writeInFile();
void writeInF();
int main()
{
    string pass;
    cout << "Please Enter The Pass : ";
    cin >> pass;
    string path = "";
    cout << "Please Enter The Name OF File : ";
    cin >> path;
    path = "C:\\SIC\\"+path+".txt";
    readFile(path);
    while(!flag)
    {
        cout << "Please Enter A Valid Name OF File : ";
        cin >> path;
        path = "C:\\SIC\\"+path+".txt";
        readFile(path);
    }
    qNodef = qNode;
    writeInFile();
    writeInF();
    return 0;
}
void writeInF()
{
    ofstream myfile("C:\\SIC\\f.txt");
    myfile << "ExPRESSION TABLE.\n";
    map<string,string>::iterator iter;
    for( iter = expression.begin(); iter != expression.end(); iter++ )
    {
        myfile << "KEY : " << iter->first << " ,VALUE : " <<iter->second << ".\n";
    }
    myfile << "\n";
    myfile << "SYMBOL TABLE.\n";
    bool flag2 = false;
    map<string,string>::iterator iterd;
    for( iterd = symbols.begin(); iterd != symbols.end(); iterd++ )
    {
        myfile << "KEY : " << iterd->first << " ,VALUE : " <<iterd->second << ".\n";
    }
    myfile << "\n";
    Node node;
    myfile << "LINE    ADDRESS    LABEL    MNENONIC    OPERAND    COMMENTS.\n";
    while(qNodef.size()!=0)
    {
        node = qNodef.front();
        if(node.line == "")
        {
            myfile<<"    ";
        }
        else
        {
            myfile<<node.line;
            for(int i=node.line.size(); i<4; i++)
            {
                myfile<<" ";
            }
        }
        myfile <<"    ";
        if(node.location== ""&&node.mnomotic==""&&node.operand=="")
        {
            myfile<<"       ";
        }
        else
        {
            myfile<<node.location;
            for(int i=node.location.length(); i<7; i++)
            {
                myfile<<" ";
            }
        }
        myfile <<"    ";
        if(node.label=="")
        {
            myfile<<"     ";
        }
        else
        {
            myfile<< node.label;
            for(int i=node.label.size(); i<5; i++)
            {
                myfile<<" ";
            }
        }
        myfile <<"    " ;
        if(node.mnomotic=="END")
        {
            flag2 = true;
        }
        if(node.mnomotic=="")
        {
            myfile<<"        ";
        }
        else
        {
            if(node.format=='+')
            {
                node.mnomotic = node.format+node.mnomotic;
            }
            myfile<<node.mnomotic;
            for(int i=node.mnomotic.size(); i<8; i++)
            {
                myfile<<" ";
            }
        }
        myfile<<"    ";
        if(node.operand=="")
        {
            myfile<<"       ";
        }
        else
        {
            myfile<< node.operand;
            for(int i=node.operand.size(); i<7; i++)
            {
                myfile<<" ";
            }
        }
        myfile<<"    ";
        if(node.comment=="")
        {
            myfile << "        ";
        }
        else
        {
            myfile << node.comment;
            for(int i=node.comment.size(); i<8; i++)
            {
                myfile<<" ";
            }
        }
        myfile<<"\n";
        qNodef.pop();
    }
    if(!flag2)
    {
        myfile << "                            END";
        myfile<<"\n";
    }
    map<string,string>::iterator iter1;
    for( iter1 = symbols.begin(); iter1 != symbols.end(); iter1++ )
    {
        if(iter1->second=="-1")
        {
            myfile << "                   ***ERROR : THESE "<<iter1->first<<" ISN'T DEFINED.\n";
        }
    }
    myfile.close();
}
void writeInFile()
{
    ofstream myfile("C:\\SIC\\write.txt");
    Node node;
    bool flag2 = false;
    myfile << "LINE    ADDRESS    LABEL    MNENONIC    OPERAND    COMMENTS.\n";
    while(qNode.size()!=0)
    {
        node = qNode.front();
        if(node.line == "")
        {
            myfile<<"    ";
        }
        else
        {
            myfile<<node.line;
            for(int i=node.line.size(); i<4; i++)
            {
                myfile<<" ";
            }
        }
        myfile <<"    ";
        if(node.location== ""&&node.mnomotic==""&&node.operand=="")
        {
            myfile<<"       ";
        }
        else
        {
            myfile<<node.location;
            for(int i=node.location.length(); i<7; i++)
            {
                myfile<<" ";
            }
        }
        myfile <<"    ";
        if(node.label=="")
        {
            myfile<<"     ";
        }
        else
        {
            myfile<< node.label;
            for(int i=node.label.size(); i<5; i++)
            {
                myfile<<" ";
            }
        }
        myfile <<"    " ;
        if(node.mnomotic=="END")
        {
            flag2 = true;
        }
        if(node.mnomotic=="")
        {
            myfile<<"        ";
        }
        else
        {
            if(node.format=='+')
            {
                node.mnomotic = node.format+node.mnomotic;
            }
            myfile<<node.mnomotic;
            for(int i=node.mnomotic.size(); i<8; i++)
            {
                myfile<<" ";
            }
        }
        myfile<<"    ";
        if(node.operand=="")
        {
            myfile<<"       ";
        }
        else
        {
            myfile<< node.operand;
            for(int i=node.operand.size(); i<7; i++)
            {
                myfile<<" ";
            }
        }
        myfile<<"    ";
        if(node.comment=="")
        {
            myfile << "        ";
        }
        else
        {
            myfile << node.comment;
            for(int i=node.comment.size(); i<8; i++)
            {
                myfile<<" ";
            }
        }
        myfile<<"\n";
        qNode.pop();
    }
    if(!flag2)
    {
        myfile<< "                   ***Error : THERE ISN'T END OF PROGRAM ";
        myfile << "\n";
    }
    map<string,string>::iterator iter1;
    for( iter1 = symbols.begin(); iter1 != symbols.end(); iter1++ )
    {
        if(iter1->second=="-1")
        {
            myfile << "                   ***ERROR : THESE "<<iter1->first<<" ISN'T DEFINED.\n";
        }
    }
    myfile << "\n";
    myfile << "SYMBOL TABLE.\n";
    map<string,string>::iterator iter;
    for( iter = symbols.begin(); iter != symbols.end(); iter++ )
    {
        myfile << "KEY : " << iter->first << " ,VALUE : " <<iter->second << ".\n";
    }
    myfile.close();
}
void readFile(string path)
{

    Node node ;
    string line;
    ifstream myfile (path);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            flag = true;
            if(line=="\n")
            {
                continue;
            }
            regex a("(\\s+)?([A-Z0-9]+)(\\s+)(\\+)?([A-Z]+)(\\s+)([^\\s.]+)(\\s+)?((\\.)([^\n]+))?");  //label&mnon&operand
            regex b("(\\s+)?(\\+)?([A-Z]+)(\\s+)([^\\s]+)(\\s+)?((\\.)([^\n]+))?");   //mnon&operand
            regex d("(\\s+)?(\\.)([^\n]+)?");
            regex c("(\\s+)?([A-Z]+)(\\s+)?((\\.)([^\n]+))?");
            smatch matches;
            if(regex_search(line, matches, a))
            {
                node.line=std::to_string(lin);
                lin++;
                node.label = matches[2].str();
                node.format = matches[3].str()[0];
                node.mnomotic = matches[5].str();
                node.operand = matches[7].str();
                node.comment = matches[11].str();
                checkError(node);
            }
            else if(regex_search(line, matches, b))
            {
                node.line=std::to_string(lin);
                lin++;
                node.label = "";
                node.format = matches[2].str()[0];
                node.mnomotic = matches[3].str();
                node.operand = matches[5].str();
                node.comment = matches[9].str();
                checkError(node);
            }
            else if(regex_search(line, matches, d))
            {
                node.line=std::to_string(lin);
                lin++;
                stringstream ss1;
                stack<Node> temp;
                while(sNode.size()!=0 && sNode.top().location=="")
                {
                    temp.push(sNode.top());
                    sNode.pop();
                }
                if(sNode.size()==0)
                {
                    ss1 << hex << pc;
                }
                else
                {
                    ss1 << hex << sNode.top().location;
                }
                node.location = ss1.str();
                while(temp.size()!=0)
                {
                    sNode.push(temp.top());
                    temp.pop();
                }
                node.label = ".THIS IS A COMMENT LINE";
                node.mnomotic = "";
                node.operand = "";
                node.format = ' ';
                qNode.push(node);
                sNode.push(node);
            }
            else if(regex_search(line, matches, c))
            {
                node.line=std::to_string(lin);
                lin++;
                stringstream ss1;
                stack<Node> temp;
                while(sNode.size()!=0 && sNode.top().location=="")
                {
                    temp.push(sNode.top());
                    sNode.pop();
                }
                if(sNode.size()==0)
                {
                    ss1 << hex << pc;
                }
                else
                {
                    ss1 << hex << sNode.top().location;
                }
                node.location = ss1.str();
                while(temp.size()!=0)
                {
                    sNode.push(temp.top());
                    temp.pop();
                }
                node.label = "";
                node.mnomotic = matches[2].str();
                node.operand = "";
                node.comment = matches[6].str();
                qNode.push(node);
                sNode.push(node);
            }
        }
        map<string,string>::iterator iter;
        for( iter = dependent.begin(); iter != dependent.end(); iter++ )
        {
            regex ex("([A-Z]+)([-]+)([A-Z]+)");
            regex ex2("([A-Z]+)");
            smatch matches;
            if(regex_search(iter->second, matches, ex))
            {
                string operand1 = matches[1].str();
                string operand2 = matches[3].str();
                if(symbols[operand2]!="-1" && symbols[operand1]!="-1")
                {
                    int m1 = 0;
                    int m2=0;
                    std::istringstream converter(symbols[operand1]);
                    converter >> std::hex >> m1;
                    std::istringstream converter2(symbols[operand2]);
                    converter2 >> std::hex >> m2;
                    int pl = m1-m2;
                    stringstream ss;
                    ss << hex << pl;
                    symbols[iter->first]= ss.str();
                }
            }else if(regex_search(iter->second, matches, ex2)){
                    int m1 = 0;
                    string operand1 = matches[1].str();
                    std::istringstream converter(symbols[operand1]);
                    converter >> std::hex >> m1;
                    stringstream ss;
                    ss << hex << m1;
                    symbols[iter->first]= ss.str();
            }
        }
        map<string,string>::iterator iter2;
        for( iter2 = expression.begin(); iter2 != expression.end(); iter2++ )
        {
            regex ex("([A-Z]+)([-]+)([A-Z]+)");
            smatch matches;
            if(regex_search(iter2->first, matches, ex))
            {
                string operand1 = matches[1].str();
                string operand2 = matches[3].str();
                if(symbols[operand2]!="-1" && symbols[operand1]!="-1")
                {
                    int m1 = 0;
                    int m2 = 0;
                    std::istringstream converter(symbols[operand1]);
                    converter >> std::hex >> m1;
                    std::istringstream converter2(symbols[operand2]);
                    converter2 >> std::hex >> m2;
                    int pl = m1-m2;
                    stringstream ss;
                    ss << hex << pl;
                    expression[iter2->first]= ss.str();
                }
            }
        }
        myfile.close();
    }
}
void checkError(Node node)
{

    stringstream ss;
    ss << hex << pc;
    node.location = ss.str();
    Node error;
    if(counter == 0 && node.mnomotic!="START")
    {
        error.line="";
        stringstream ss1;
        ss1 << hex << pc;
        error.location = ss1.str();
        error.label = "***ERROR: THERE ISN'T START ADDRESS";
        error.mnomotic = "";
        error.operand = "";
        error.format = ' ';

        counter++;
    }

    else if(counter==0 && node.mnomotic=="START" && node.operand=="")
    {
        error.line = "";
        error.location = "";
        error.label = "***ERROR:THERE ISN'T A START ADDRESS";
        error.mnomotic = "";
        error.operand = "";
        error.format = ' ';
    }
    else if(node.mnomotic=="START" && node.operand!="")
    {
        counter++;
        std::istringstream converter(node.operand);
        converter >> std::hex >> pc;
    }
    else if(!(std::find(l.begin(), l.end(), node.mnomotic) != l.end()))
    {
        error.line = "";
        error.location = "";
        error.label = "***ERROR:THERE ISN'T MNOMOTIC CALLED " + node.mnomotic;
        error.mnomotic = "";
        error.operand = "";
        error.format = ' ';
    }
    else if(node.mnomotic=="RSUB" || node.mnomotic=="END" || node.mnomotic == "BASE" || node.mnomotic == "NOBASE")
    {

    }
    else if(node.mnomotic=="EQU")
    {
        regex ex("([A-Z]+)([-]+)?([A-Z])?");
        regex ex2("([A-F0-9]+)([+---*-/]+)?([A-F0-9])?");
        smatch matches;
        if(regex_search(node.operand, matches, ex))
        {
        }
        else if(regex_search(node.operand, matches, ex2))
        {
        }
        else
        {
            error.line = "";
            error.location = "";
            error.label = "***ERROR:THESE EXPRESSION IS FALSE " + node.operand;
            error.mnomotic = "";
            error.operand = "";
            error.format = ' ';
        }
    }
    else if(node.mnomotic=="ORG")
    {
        bool found = false;
        regex ex("([A-Z][A-Z0-9]+|[A-Fa-f0-9]+)(([+-/*]+)([A-Z][A-Z0-9]+|[A-Fa-f0-9]+))?");
        smatch matches;
        int m1 = 0;
        int m2 = 0;
        if(regex_search(node.operand, matches, ex))
        {
            map<string, string>::iterator p1;
            p1 = label.find(matches[1].str());
            if(p1 != label.end())
            {
                std::istringstream converter(p1->second);
                converter >> std::hex >> m1;
                found = true;
            }
            else if(IsThisStringAHexNumber(matches[1].str()))
            {
                std::istringstream converter(matches[1].str());
                converter >> std::hex >> m1;
            }
            else
            {
                error.line = "";
                error.location = "";
                error.label = "***ERROR:THESE ";
                error.label +=node.operand;
                error.label+= " IS UNDEFINED ";
                error.mnomotic = "";
                error.operand = "";
                error.format = ' ';
            }
            map<string, string>::iterator p;
            p = label.find(matches[4].str());
            if(matches[4].str() != "")
            {
                if(p != label.end())
                {
                    std::istringstream converter(p->second);
                    converter >> std::hex >> m2;
                    found = true;
                }
                else if(IsThisStringAHexNumber(matches[4].str()))
                {
                    std::istringstream converter(matches[4].str());
                    converter >> std::hex >> m2;
                }
                else
                {
                    error.line = "";
                    error.location = "";
                    error.label = "***ERROR:THESE ";
                    error.label +=node.operand;
                    error.label+= " IS UNDEFINED ";
                    error.mnomotic = "";
                    error.operand = "";
                    error.format = ' ';
                }
                if(found)
                {
                    if(matches[3].str()=="+")
                    {
                        pc = m1+m2;
                    }
                    else if(matches[3].str()=="-")
                    {
                        pc = m1-m2;
                    }
                    else if(matches[3].str()=="*")
                    {
                        pc = m1*m2;
                    }
                    else if(matches[3].str()=="/")
                    {
                        pc = m1/m2;
                    }
                }
            }
            else
            {
                pc = m1;
            }
        }
    }
    else
    {
        if(node.mnomotic=="RESW" || node.mnomotic=="WORD" || node.mnomotic=="RESB")
        {
            regex r("[0-9]+");
            smatch matches;
            if(!(regex_search(node.operand, matches, r)))
            {
                error.line = "";
                error.location = "";
                error.label = "***ERROR:IT'S NOT A INT NUMBER";
                error.mnomotic = "";
                error.operand = "";
                error.format = ' ';
            }
            else if (node.mnomotic=="RESW")
            {
                int x = 0;
                std::istringstream iss (node.operand);
                iss >> x;
                pc = pc + x*3;
            }
            else
            {
                int x = 0;
                std::istringstream iss (node.operand);
                iss >> x;
                pc = pc + x;
            }
        }
        else if(node.mnomotic=="BYTE")
        {
            regex c("(C)(\\')(.*)(\\')");
            regex x("(X)(\\')([0-9A-Fa-f]+)(\\')");
            smatch matches;
            if(!(regex_search(node.operand, matches, c)) && !(regex_search(node.operand, matches, x)))
            {
                error.line = "";
                error.location = "";
                error.label = "***ERROR:SYNTEX IS WRONG";
                error.mnomotic = "";
                error.operand = "";
                error.format = ' ';
            }
            else
            {
                pc = pc + matches[3].str().length();
            }
        }
        else if(node.format=='+')
        {
            pc = pc+4;
        }
        else if(node.mnomotic[node.mnomotic.length()-1] =='R' || node.mnomotic=="RMO")
        {
            regex s1("([XASTB])(\\,)([XASTB])");
            regex s2("[XASTB]");
            smatch matches;
            if(node.operand.length() != 3 && node.operand.length() != 1)
            {
                error.line = "";
                error.location = "";
                error.label = "***ERROR:SYNTEX IS WRONG";
                error.mnomotic = "";
                error.operand = "";
                error.format = ' ';
            }
            else if((node.mnomotic =="TIXR") &&(regex_search(node.operand, matches, s2))&&node.operand.length() == 1)
            {
                pc = pc + 1;
            }
            else if((node.mnomotic =="SUBR"||node.mnomotic =="COMPR"||node.mnomotic =="DIVR"||node.mnomotic =="MULR"||node.mnomotic=="RMO"||node.mnomotic =="ADDR") && (regex_search(node.operand, matches, s1))&& node.operand.length() == 3)
            {
                pc = pc + 2;
            }
            else
            {
                error.line = "";
                error.location = "";
                error.label = "***ERROR:SYNTEX IS WRONG";
                error.mnomotic = "";
                error.operand = "";
                error.format = ' ';
            }
        }
        else if(node.operand[0] == '=')
        {
            regex c("(\\=)(C)(\\')(.*)(\\')");
            regex x("(\\=)(X)(\\')([0-9A-Fa-f]+)(\\')");
            smatch matches;
            if(!(regex_search(node.operand, matches, c)) && !(regex_search(node.operand, matches, x)))
            {
                error.line = "";
                error.location = "";
                error.label = "***ERROR:SYNTEX IS WRONG";
                error.mnomotic = "";
                error.operand = "";
                error.format = ' ';
            }
            else
            {
                pc = pc + 3;
            }
        }
        else
        {
            pc = pc+3;
        }
        counter++;
    }

    qNode.push(node);
    sNode.push(node);
    getSymbol(node);
    label.insert(pair<string,string>(node.label,node.location));
    if(error.label!="")
    {
        qNode.push(error);
        sNode.push(error);
    }
}

void getSymbol(Node node)
{
    Node error;
    if(node.mnomotic!="BYTE" && node.mnomotic!="WORD" && node.mnomotic!="RESW" && node.mnomotic!="RESB"&&node.mnomotic!="ADDR"&&node.mnomotic!="SUBR"
            && node.mnomotic!="TIXR" && node.mnomotic!="DIVR" && node.mnomotic!="MULR" && node.mnomotic!="RMO" && node.mnomotic!="COMPR" && node.mnomotic!="START"
            && node.mnomotic!="END" && node.operand[node.operand.size()-2]!=','&& node.operand[0]!='=')
    {
        if(node.mnomotic=="EQU")
        {
            if(node.label=="")
            {
                error.line = "";
                error.location = "";
                error.label = "***ERROR:THERE ISN'T A LABEL.";
                error.mnomotic = "";
                error.operand = "";
                qNode.push(error);
                sNode.push(error);
                return;
            }
            regex ex("([A-Z]+)([-]+)([A-Z]+)");
            regex ex2("([A-F0-9]+)([+---*-/]+)([A-F0-9]+)");
            regex ex3("([A-Z]+)");
            regex ex4("([A-F0-9]+)");
            smatch matches;
            int m1=0;
            bool found1 = false;
            bool found2 = false;
            int m2=0;
            if(regex_search(node.operand, matches, ex))
            {
                map<string, string>::iterator p1;
                p1 = label.find(matches[1].str());
                if(p1 != label.end())
                {
                    std::istringstream converter(p1->second);
                    converter >> std::hex >> m1;
                    found1 = true;
                }
                map<string, string>::iterator p;
                p = label.find(matches[1].str());
                if(p != label.end())
                {
                    std::istringstream converter(p->second);
                    converter >> std::hex >> m2;
                    found2 = true;
                    if(found1)
                    {
                        std::istringstream converter(p->second);
                        int pl = m1-m2;
                        converter >> std::hex >> (pl);
                        symbols[node.label]= (converter).str();
                        expression[node.operand] = (converter).str();
                    }
                }
                if(!found1 || !found2)
                {
                    symbols[node.label]= "-1";
                    dependent[node.label] = node.operand;
                    expression[node.operand] = "-1";
                }
                return;
            }
            else if(regex_search(node.operand, matches, ex2))
            {
                std::istringstream converter(matches[1].str());
                converter >> std::hex >> m1;
                std::istringstream converter2(matches[3].str());
                converter2 >> std::hex >> m2;
                int pl=0;
                if(matches[2].str()=="+")
                {
                    pl = m1+m2;
                }
                else if(matches[2].str()=="-")
                {
                    pl = m1-m2;
                }
                else if(matches[2].str()=="*")
                {
                    pl = m1*m2;
                }
                else if(matches[2].str()=="/")
                {
                    pl = m1/m2;
                }
                stringstream ss;
                ss << hex << pl;
                symbols[node.label]= ss.str();
                expression[node.operand]=ss.str();
                return;
            }
            else if(regex_search(node.operand, matches, ex3))
            {
                map<string, string>::iterator p1;
                p1 = label.find(matches[1].str());
                if(p1 != label.end())
                {
                    std::istringstream converter(p1->second);
                    converter >> std::hex >> m1;
                    found1 = true;
                }
                if(!found1)
                {
                    symbols[node.label]= "-1";
                    dependent[node.label] = node.operand;
                }
                return;
            }else if(regex_search(node.operand, matches, ex4)){
                std::istringstream converter(matches[1].str());
                converter >> std::hex >> m1;
                stringstream ss;
                ss << hex << m1;
                symbols[node.label]= ss.str();
                return;
            }
        }
        else if(node.mnomotic=="ORG")
        {
        }
        else if(node.operand[0]!='#'&& isalpha(node.operand[0]) && node.operand[0]!='@')
        {
            if(symbols[node.operand]=="")
            {
                symbols[node.operand] = "-1";
            }
        }
    }
    if(node.label!="" && node.label!="BGN" && node.label!="PROG")
    {
        if(symbols[node.label]!= "-1" &&symbols[node.label]!="")
        {
            error.line = "";
            error.location = "";
            error.label = "***ERROR:THE "+node.label+" HAS DEFINED BEFORE.";
            error.mnomotic = "";
            error.operand = "";
            qNode.push(error);
            sNode.push(error);
            return;
        }
        if(symbols[node.label]=="-1"||symbols[node.label]=="")
        {
            symbols[node.label]=node.location;
        }
    }
}
bool IsThisStringAHexNumber(std::string const &str)
{
    for (size_t i = 0, n = str.length(); i < n; ++i)
        if (!std::isxdigit(str[i]))
            return false;

    return true;
}
