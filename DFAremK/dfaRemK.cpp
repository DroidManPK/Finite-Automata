//
// Created by pavan on 18/12/17.
//
#include <iostream>

using namespace std;

void generate(int st[][2],int k){

    //DFA generator
    for(int i=0;i<k;i++)
    {

        st[i][0]=(2*i)%k;//2p%k
        st[i][1]=(2*i+1)%k;//2p+1%k

    }

}

void printdfa(int st[][2],int k){
    FILE *pipe,*fp;
    system("rm -rf dfa.dot");//remove previous file
    fp = fopen ("dfa.dot", "w");//write new file

    fprintf(fp,"digraph dfa{ \n");//init dotty file
    for(int i=0;i<k;i++)//labels init
        fprintf(fp,"%d [label = \"%d\" ,color=black];\n",i,i);

    for(int i=0;i<k;i++)//write the states to dotty
    {
        fprintf(fp,"%d -> %d [label = \"%d\" ,color=black];\n",i,st[i][0],0);
        fprintf(fp,"%d -> %d [label = \"%d\" ,color=black];\n",i,st[i][1],1);
    }

    fprintf(fp,"}\n");

    fclose (fp);
//run dotty
    pipe=popen("dot -Tps dfa.dot -o dfa.ps","w");
    pclose(pipe);
    pipe=popen("evince dfa.ps","r");
    pclose(pipe);
}

void eval(string str,int st[][2]){
    int cur=0;
    for(char &a : str){//for each digit in the binary stream of characters
        switch (a){
            case '0'://get current state
                cout<<"\n\nCurrent state : q"<<cur;
                cur=st[cur][0];//get next state from current state
                cout<<"\nNew State : q"<<cur;
                break;
            case '1'://get current state
                cout<<"\n\nCurrent state : q"<<cur;
                cur=st[cur][1];//get next state from current state
                cout<<"\nNew State : q"<<cur;
                break;
            default://ignore wrong characters in string
                cout<<"\nWrong Character in the string\n";

        }
    }

    cout<<"\n\nThe Remainder is : "<<cur<<"\n\n";//print the remainder = current state
}


int main(){

    int ch,k;
    string str;
    int st[k][2];
    bool f=false;
    while(true)
    {
        cout<<"\n1 - Generate DFA for k\n2 - Display DFA\n3 - Evaluate string\n\nAny other number - Exit \n\n";
        cout<<"Enter your choice : ";
        cin>>ch;
        switch(ch)
        {



            case 1:cout<<"\nEnter the value for k : ";
                cin>>k;
                f=true;
                generate(st,k);//generate dfa for given k
                cout<<"\nDFA Generated\n";
                break;

            case 2:
                if(f)//check if dfa is generated
                printdfa(st,k);//print dfa
                else
                    cout<<"\nDFA not generated\n";

                break;

            case 3:
                if(f){
                    cout<<"\nEnter the binary number string :\n";
                    cin>>str;
                    eval(str,st);//evaluate string

                }
                else
                    cout<<"\nDFA not generated\n";

                break;
            default:printf("Exit\n");
                exit(0);

        }



    }
}