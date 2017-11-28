#include <iostream>
#include "Project1-BKW.h"
#include "Project1-FRW.h"
#include <fstream>
#include <string>
using namespace std;

int main()
{
    string disorder =  initBWR_KB();
    cout<<endl;
    cout<<"Press enter to continue to the Forward System to find the treatment \n";
    fflush(stdin);
    getchar();
    initForwardKB();
    //if( disorder != "" && disorder != "No_Disorder")
    cout<<endl;
    GetVaribleFromFWR(disorder);
    cout<<"\nThank you for using our expert system \n";

    return 0;
}
