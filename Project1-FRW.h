
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <string>
#include <queue>

using namespace std;


//Pointers to clause variable List
struct VarPtr {
  int RuleNo = 0;
  int ClauseNo = 0;
};

//Variable List
struct FRW_Variable {
  string Name;
  string Status;
  string Value;
};
FRW_Variable *FRW_varList;     //variable list
queue <FRW_Variable> condQue;   // condition variable Queue


string *k_b;                   //Knowledge base
string *clauseVarList;            //Clause Variable list

VarPtr CVPtr;                    //pointer to clause variable list


void initForwardKB();
bool Check_Instantiated(FRW_Variable);
void SearchClauseVariblesList(FRW_Variable var);
void GetVaribleFromFWR(string);


void initForwardKB()
{

    k_b = new string[50];
    string line;
    int index = 0;

    //Reading the knowledge base file
    ifstream f("Project1-FrWardrules.txt");
    if (f.is_open())
    {
        while (getline(f, line) && index < 50)
        {
           if(index != 0 && (index % 2) != 0)
           {
              string temp ="";
              for ( int i = 0 ; i <3 ; i++)
              {
                temp = temp +"\n\n"+line;
                getline(f, line);
              }
              k_b[index] =temp;
            }
            else
            {
              k_b[index] = line;
            }
            index++;
        }
        f.close();
    }
    else
    {
        cout<<"UNABLE TO OPEN FORWARD KNOWLEDGE BASE FILE"<<endl;
    }
	 //Reading clause variable list
   clauseVarList = new string[25] {""};
   ifstream fcvl("Project1-ClauseVaribleList.txt");
   index = 0;
   if (fcvl.is_open())
   {
    while (getline(fcvl, line) && index < 8)
        {
            clauseVarList[index] = line;
            index++;
        }
        fcvl.close();
    }
    else
    {
        cout<<"UNABLE TO OPEN CLAUSE VARIABLE FILE"<<endl;
    }

    //Reading the Variables List file
	FRW_varList = new FRW_Variable[3];
    ifstream fvl("Project1-VariableList.txt");
    index = 0;
    //Reading the variable list file
    if (fvl.is_open())
    {
		//2 is the number of variable in the variable list DISORDER TREATMENT
        while (getline(fvl, line)&& index < 2)
        {
            FRW_varList[index].Name = line;
            FRW_varList[index].Status = "NI";
            FRW_varList[index].Value = "";
            index++;
        }
        fvl.close();
    }
    else
    {
        cout<<"UNABLE TO OPEN VARIABLE LIST FILE"<<endl;
    }


}


bool Check_Instantiated(FRW_Variable V)
{
    for (int i = 0; i < 2; i++)
    {
        //check if the variable in the variable list Instantiated or not
        if ( V.Status == "NI" && V.Name == FRW_varList[i].Name)
            return false;
    }
    return true;

}

void GetVaribleFromFWR(string str)
{

    FRW_Variable var ;
    var.Name= "DISORDER";
    var.Status="NI";
    var.Value=str;

    //put varible value and Instantiate the varible
    int i = 0;
    while(i < 2)
    {
        if(var.Name==FRW_varList[i].Name)
        {
            FRW_varList[i].Value= var.Value;
            FRW_varList[i].Value= var.Value;
            //check the statutes of the varible
            if(! Check_Instantiated(var))
            {
               var.Status ="I";
            }
        }
        i++;
    }

    //set variable in the conclusion variable queue

    condQue.push(var);

    //update the clause # and rule #
    CVPtr.ClauseNo = 1 ;
    CVPtr.RuleNo =0; // initialize it to 0

    // search for rule number in clause varible list

    SearchClauseVariblesList(var);

}

void SearchClauseVariblesList(FRW_Variable var)
 {

   //search the Clause variable List for all variable in conclusion queue

   cout<<"**Searching for "<<var.Value<<" "<<var.Name<<" in our forward Knowledge_Base**"<<endl;
   cout<<endl;
    bool found = false;
    int i = 0 ;

   while(!condQue.empty())
   {
       //get the front of the queue to process is
       FRW_Variable TempVar = condQue.front();
       // 8 is number of Variable in Clause Variable List
       while(found == false && i< 8 )
       {

           if(TempVar.Name == clauseVarList[i])
           {
                CVPtr.RuleNo = 2*i;
                CVPtr.ClauseNo = 1;

                //check if rule number variable is matching
                if(k_b[CVPtr.RuleNo] == TempVar.Value)
                {
                    //set variable treatment value
                    FRW_varList[1].Value = k_b[CVPtr.RuleNo+1];
                     found = true;
                }
             }
         i++;
       }
        //remove var from conclusion variable queue
     condQue.pop();
   }

     // check if variable is found or not

     if(found == true)
     {
         cout<<endl;
         cout<<"The Treatment for "<<k_b[CVPtr.RuleNo]<<" Disorder :"<<endl;
		 cout<<endl;
         cout<<k_b[CVPtr.RuleNo+1]<<endl;
         cout<<endl;
     }
     else
     {
         cout<<endl;
         cout<<"  No Treatment is Found"<<endl;
         cout<<endl;
     }


}














