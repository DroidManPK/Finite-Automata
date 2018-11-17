//
// Created by pavan on 22/12/17.
//
// Comment by anoop
#include <iostream>
#include <stack>
#include <fstream>
#include <cstring>

using namespace std;

char endm='$';
stack<char> s1,s2;
class Rule {
public : int currentState,nextState;
    char input,pushValue,popValue;
    char dir;
public:
    void getdata(bool d){
        cout<<"Current State : ";
        cin>>currentState;
        cout<<"Next State : ";
        cin>>nextState;
        cout<<"on Input Symbol : ";
        cin>>input;
        if(d)
        {
            cout<<"Enter the Direction : ";
            cin>>dir;
            cout<<"Enter the Replacement value : ";
            cin>>pushValue;
        }
        else{
            cout<<"Enter Push value for stack ";
            cin>>pushValue;
            cout<<"Enter pop value for stack : ";
            cin>>popValue;
        }

    }

};

void print(stack<char> &s)
{
    if(s.empty())
    {
        cout << endl;
        return;
    }
    char x= s.top();
    s.pop();
    print(s);
    s.push(x);
    cout << x << " ";
}

char *starr;
char * tochar(string s){
    int n = s.length();

    // declaring character array
    char char_array[n+1];

    // copying the contents of the
    // string to char array
    strcpy(char_array, s.c_str());
    starr=char_array;
    return starr;
}
void printdfa(Rule r[],int k,int m){
    FILE *pipe,*fp;
    system("rm -rf dfa.dot");//remove previous file
    fp = fopen ("dfa.dot", "w");//write new file

    fprintf(fp,"digraph dfa{ \n");//init dotty file
    for(int i=0;i<m;i++)//labels init
        fprintf(fp,"%d [label = \"%d\" ,color=black];\n",i,i);

    for(int i=0;i<k;i++)//write the states to dotty
    {
        char a[]={r[i].input,(char)'/',r[i].pushValue,(char)',',r[i].dir,'\0'};
        fprintf(fp,"%d -> %d [label = \"%s\" ,color=black];\n",r[i].currentState,r[i].nextState,a);
    }

    fprintf(fp,"}\n");

    fclose (fp);
//run dotty
    pipe=popen("dot -Tps dfa.dot -o dfa.ps","w");
    pclose(pipe);
    pipe=popen("evince dfa.ps","r");
    pclose(pipe);
}



int eval(string str,Rule r[],int n,int m,string syms,int f){
    int cur=0;

    int idx[m][syms.length()];

    for(int i=0;i<m;i++)
        for(int j=0;j<syms.length();j++)
        {
            idx[i][j]=-1;

        }

//find rules matching the input at current state
    for(int i=0;i<n;i++)
    {
        idx[r[i].currentState][syms.find(r[i].input)]=i;
    }

    cout<<"\nPrinting Rule Matching Matrix : \n";
    for(int i=0; i<m; i++)    //This loops on the rows.
    {cout<<"q"<<i<<" ";
        for(int j=0; j<syms.length(); j++) //This loops on the columns
        {
            cout << idx[i][j]  << "  ";
        }
        cout << endl;
    }
    int id=0,cou=0;

    char Z;

    while(true){//for each character in the stack s2
        cout<<"\n\nCurrent state : q"<<cur;
        //cout<<"\nat line 104 count "<<cou<<endl;
        if(s2.top()==endm)
            s2.push('B');

        id=idx[cur][syms.find(s2.top())];

        //cout<<"\nat line 106 count "<<cou<<"  "<<id<<" "<<syms.find(s2.top())<<endl;


        if(id==-1)
        {
            cout<<"\nPrinting Stacks : \n";
            print(s2);
            print(s1);
            cout<<"\nError. cannot find transition for input symbol "<<s2.top()<<" at state "<<cur;

            return -1;
        }

        cur=r[id].nextState;
        cout<<"\nNew State : q"<<cur;

        if(cur==f)
            break;

        if(r[id].dir=='R')
        {//cout<<"\ngoing right \nat line 113 count "<<cou<<"  "<<id<<endl;
            //if(r[id].pushValue!='B') dont need this
            s1.push(r[id].pushValue);

            if(s2.top()!=endm)
                s2.pop();

        } else if(r[id].dir=='L')
        {
            //cout<<"\ngoing left \nat line 121 count "<<cou<<"  "<<id<<endl;

                Z= s1.top();
                s1.pop();
                s2.pop();
                s2.push(r[id].pushValue);
                s2.push(Z);
         }

        cou++;

        cout<<"\nPrinting stacks \nS1 : ";
        print(s1);
        cout<<"\nS2 : ";
        print(s2);
        cout<<"\nCount : "<<cou;
    }

    cout<<"\n\nThe Last state is : "<<cur<<"\n\n";//print the current state
    return cur;
}

void process(Rule r[],int n,int m,int f,string syms){
    char str[100];
    cout<<"printing dfa\n";
    printdfa(r,n,m);
    char cho;
    //bool valid=true;
    while(true){
        s1=stack<char>();
        s2=stack<char>();
        s1.push(endm);
        s2.push(endm);
        cout<<"Enter a input String : ";
        cin>>str;

        for(int i=strlen(str)-1;i>=0;i--){
            cout<<str[i];
            if(str[i]==syms[syms.find(str[i])])
                s2.push(str[i]);
            else
            {
        //        valid=false;
                break;
            }
        }
        print(s2);

        cout<<endl;

        if(eval(str,r,n,m,syms,f)==f){
            cout<<"\nString Accepted\n";
        } else
        {
            cout<<"\nString not Accepted\n";
        }

        cout<<"\nAnother String? \ny or 1 to continue, any other key to exit\n";
        cin>>cho;
        if(!(cho=='y'||cho=='Y'||cho=='1'))
        exit(0);
    }

}


 int main(){


     string str,syms;
     int n,m,f;

     cout<<"File input(1) or Console input(2)?"<<endl;
     int ch;
     cin>>ch;
     if(ch==1)
     {
         cout<<"Enter input file name : ";
         string fname="input.txt";
         cin>>fname;
         fstream in(fname);
         if (!in.good ()) {
             cout << "Error: cannot open the input file\n";
             return 1;
         }
         char line[100];
         in.getline(line,100,'\n');
         m=atoi(line);

         in.getline(line,100,'\n');
         n=atoi(line);
         in.getline(line,100,'\n');
         f=atoi(line);
         in.getline(line,100,'\n');
         syms=line;

         Rule r[n];
         string tmp;
         for(int i=0;i<n;i++)
         {
             in.getline(line,100,'\n');
             char *vals;
             vals=strtok(line,",");
             r[i].currentState=atoi(vals);
             r[i].nextState=atoi(strtok(NULL,","));
             r[i].input=*(strtok(NULL,","));
             r[i].dir=*strtok(NULL,",");
             r[i].pushValue=*strtok(NULL,",");
         }

         process(r,n,m,f,syms);

     }
     else
     {
         cout<<"Enter the number of states : ";
         cin>>m;
         cout<<"Enter the number of rules : ";
         cin>>n;
         cout<<"Enter the final state : ";
         cin>>f;
         cout<<"Enter the Tape symbols (not just Input Symbols) : ";
         cin>>syms;

         Rule r[n];

         for(int i=0;i<n;i++)
         {
             cout<<"Enter Rule "<<i<<endl;
             r[i].getdata(true);
         }
         process(r,n,m,f,syms);


     }

 }
