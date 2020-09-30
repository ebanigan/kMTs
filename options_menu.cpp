#include<iostream>
//#include<cstdio>
using namespace std;

//note 170613 this does not appear to be up to date. also see main.cpp
void options_menu()
{
    cerr << "Options are: \n";
    cerr << "-s trial number and rseed\n";
    cerr << "-l load\n";
    cerr << "-f num fil per kt\n";
    cerr << "-w polym exp\n";
    cerr << "-x depolym exp\n";
    cerr << "-y rescue exp\n";
    cerr << "-z catastrophe exp\n";
    cerr << "-t time\n";
    cerr << "-k numskip\n";
    cerr << "-p ktmt spring const\n";
    cerr << "-n num kts\n";
    cerr << "-r attachment range\n";
    cerr << "-u detachment rate\n";
    cerr << "-a attachment rate\n";
    cerr << "-d initial dt\n";
    cerr << "-g polym rate\n";
    cerr << "-e depolym rate\n";
    cerr << "-b inf box\n";
    cerr << "-m negative positions allowed\n";
    cerr << "-h time delay before applying force\n";
    cerr << "-q drag\n";
}
