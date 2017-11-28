#include <iostream>
#include <Stack>
#include <fstream>
#include <string>
using namespace std;

struct variable
{
    string name;
    string status;
    string value;
};


struct rule
{
    string var[4];
    string value[4];
    string concl;
    string result;
};

string goal;
string *conclList;
stack <int> clauseStack;
stack <int> ruleStack;
rule *BWk_b;                   //Knowledge base
string *clsVarList;            //Clause Variable list
variable *varList;                //variable list
int RuleNO = 0 ;
int clauseNo ;
string initBWR_KB();
int SearchForGoal(string str,int = 0, string ="");// return index or -1 if not found
bool checkClauseVarible(int c, int r);
bool checkValue(int rule,int caluse, string val);
int clauseNumber(int r);

string initBWR_KB()
{
    //Reading the knowledge Base From input file
    BWk_b = new rule[28];
    int index = 1;
    string line;
    ifstream f("Project1-bk.txt");

    if (f.is_open())
    {
        while ( index <28)
        {
            getline(f,BWk_b[index].var[1]);
            getline(f,BWk_b[index].value[1]);
            getline(f,BWk_b[index].var[2]);
            getline(f,BWk_b[index].value[2]);
            getline(f,BWk_b[index].var[3]);
            getline(f,BWk_b[index].value[3]);
            getline(f,BWk_b[index].concl);
            getline(f,BWk_b[index].result);
            index++;
        }
        f.close();

    }
    else
    {
        cout<<"UNABLE TO OPEN KNOWLEDGE BASE FILE"<<endl;
    }

    /*
	index =1;
    while (index <28)
    {
         cout<<index*10<<") IF "<<BWk_b[index].var[1]<<" = "<< BWk_b[index].value[1]
         <<" AND "<< BWk_b[index].var[2]<<" = "<<BWk_b[index].value[2];
         if( BWk_b[index].var[3] != "")
            cout<<" AND "<<BWk_b[index].var[3]<<" = "<<BWk_b[index].value[3];
         cout<<" THEN "<<BWk_b[index].concl<<" = "<<BWk_b[index].result<<endl;
         cout<<endl;
        index++;
    }*/

    /*****Reading the Variables List From input file*****/
	varList = new variable[23];
    ifstream fv("Project1-varibleList2.txt");
    index = 0;
    //Reading the variable list file
    if (fv.is_open())
    {
        while (getline(fv, line)&& index <23)
        {
            varList[index].name = line;
            varList[index].status = "NI";
            varList[index].value = "";
            index++;
        }
        fv.close();
    }
    else
    {
        cout<<"---UNABLE TO OPEN VARIABLE LIST FILE---"<<endl;
    }
    /*****Reading clause variable list From input file*****/
    clsVarList = new string[81] {""};
    ifstream fc("Project1-clause_var_list2.txt");
    index = 1;
    if (fc.is_open())
    {
        while (getline(fc, line) && index < 81)
        {
            clsVarList[index] = line;
            index++;
        }
        fc.close();
    }
    else
    {
        cout<<"---UNABLE TO OPEN CLAUSE VARIABLE FILE---"<<endl;
    }

    /*****Reading conclusion list From input file*****/
    conclList = new string[28];
    ifstream fd("Project1-conclusionList2.txt");
    index = 1;
    if (fd.is_open())
    {
        while (getline(fd, line) && index < 28)
        {
            conclList[index] = line;
            index++;
        }
        fd.close();
    }
    else
    {
        cout<<"---UNABLE TO OPEN conclusion VARIABLE FILE---"<<endl;
    }
    cout<<"\n* WELCOME TO OUR EXPERT SYSTEM TO DETECT DISORDERS AND PROVIDE THE TRETMENT *"<<endl;
    cout<<" \n* OUR SYSYTEM POSSIBLE GOAL IS FINDING THE : Disorder"<<endl;
    cout<<"\n* ENTER THE GOAL"<<endl;
	cout<<endl;
	cout<<"  ";
    cin>>goal;
    int trace = 0;
    bool fo = false;
    while(trace < 28 && !fo)
    {
        RuleNO = SearchForGoal(goal,RuleNO);
        clauseNo = clauseNumber(RuleNO);
        if(RuleNO != -1)
	    {
           stack<int> temp;
           ruleStack=temp;
           clauseStack=temp;
           fo = checkClauseVarible(clauseNo,RuleNO);
           trace++;
		}
        else
           break;
    }
    if (fo)
    {
        if(checkClauseVarible(clauseNo,RuleNO) == 1)
		{
		   cout<<endl;
           cout<<"*** The Disorder is "<<BWk_b[RuleNO/10].result<<" ***"<<endl;
		   return BWk_b[RuleNO/10].result;
		}
    }
    else
    {
		cout<<"  Your fine :)"<<endl;
		return "";

	}


}

int clauseNumber(int r)
{
    return (3 * (r/10-1)+1);
}

bool checkClauseVarible(int c, int r)
{
    bool found = true;
    clauseStack.push(1);
    ruleStack.push(r);
    int clause = c;
    int cptr = 1;
    int i = 0;
    while(!clauseStack.empty() && !ruleStack.empty() && cptr <4)
    {
        cptr = clauseStack.top();
        int rule = ruleStack.top();
        found = true;
        while(i < 23 && cptr <4)
        {
            cptr = clauseStack.top();
            rule = ruleStack.top();
            if(clsVarList[clause]==varList[i].name)
            {
                if(varList[i].status=="NI" )
                {
                    cout<<"\n- Do you experience any "<<varList[i].name<<"?"<<endl;
					cout<<endl;
					cout<<"  ";
                    cin>>varList[i].value;
                    varList[i].status="I";

                }

                if(!checkValue(rule,cptr,varList[i].value))
                {
                    found = false;
                    cptr=4;
                }
                else
                {
                    found = true;
                }

                if(rule !=70 && cptr == 2)
                {
                    cptr = 3;
                }
            }
            else
            {
                found = false;
            }
            if(found && cptr == 3)
            {
                clauseStack.pop();
                ruleStack.pop();
                clause=clauseNo;
                return found;
            }
            if(found && cptr <3)
            {
                cptr++;
                clause++;
                clauseStack.pop();
                clauseStack.push(cptr);
            }
            i++;
        }
        if(!found)
        {
            if(clsVarList[clause] == "NEXT")
            {
                int rule2 = SearchForGoal(clsVarList[clause],0,BWk_b[rule/10].value[1]);
                int temp = clause;
                if(rule2 != -1)
                {
                    int clause2 = clauseNumber(rule2);
                    bool found2 = checkClauseVarible(clause2,rule2);
                    if(found2)
                    {
                        clauseStack.push(2);
                        ruleStack.push(rule);
                        clause = temp+1;
                        found = true;
                        i= 0;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        else if (found)
        {
            clauseStack.pop();
            ruleStack.pop();
        }

    }
    return found;
}


bool checkValue(int rule,int caluse, string val)
{
    if(BWk_b[rule/10].value[caluse] == val)
        return true;

    return false;
}

int SearchForGoal(string str, int in, string val)
{

    bool found = false;
    int i = 1;
    if(in != 0)
        i = in/10;

    while(!found && i < 28 )
    {

        if(conclList[i].compare(str) == 0 && i*10 > in )
        {
            if(val !="")
            {
                if(BWk_b[i].result == val)
                    found = true;
            }
            else
                found = true;
        }
        i++;
    }

    if(found == true)
        return (i-1)*10;
    else
        return -1;
}
