/*
    Name:Ajay Subhash Jadhav
    Roll No:2018201095
*/
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
 #include <sys/types.h>
 #include <sys/stat.h>
#include <stack>
#include <vector>
#include <iostream>
#include <sstream>
#include <string.h>
#include "fileops.h"
#include "inputs.h"
#include "commands.h"
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define HOME 104
#define BACK 127
#define SWITH_TO_COMMAND 58
#define QUIT 97
#define ENTER 10
#define ESC 27
using namespace std;

int main(int argc,char **argv)
{
    int MODE=0;
    /*  structure storing indices for list of files*/
    struct INDICES IND;IND.max_index=20;

    /*storing paths*/
    struct PATH_VALUES PAT_VAL;

    /* storing file list for backward and forward by left and right arrow keys*/
    vector<string> file_names;
    vector<string> paths;
    stack<string> stored_paths,tmp;
    if(argc!=2)
    {   
        /* path not sent while executing(default path)*/
        set_paths(".",&PAT_VAL);
        change_working_directory(".");
        getdir(PAT_VAL.CUR_PATH,file_names);
    }
    else
    {
        // path sent by user
        set_paths(argv[1],&PAT_VAL);
        change_working_directory(argv[1]);
        getdir(PAT_VAL.CUR_PATH,file_names);
    }
    /*setting root path in stack for forward backward functionality*/
    stored_paths.push(get_current_working_directory());

    /* setting the indices*/
    set_indices(file_names,&IND);
    if(IND.max_index>=IND.high_index && IND.low_index>=0)
    {
        if(IND.high_index<IND.max_index)
        {
            /*print function uses vector of file names and adjusts cursor accordingly */
            print_list(file_names,IND.low_index,IND.high_index);
            set_cursor(IND.high_index);
        }
    }
    else
    {
        print_list(file_names,IND.low_index,IND.max_index);
        set_cursor(IND.max_index);
    }
    while(true)
    {
      int k1,k2,k3;
      k1=' ';k2=' ';k3=' ';
      if(MODE==0)
      {
          //terminal title
        cout<<"\033]0;File Explorer (Normal Mode)\007";
        k1=getKey();
        if(k1==ESC)  
        {
            k2=getKey();
            k3=getKey();
            //arrow key codes
            if(k1==ESC && k2==91)
            {
                /*scrolling functionality
                    if directory has less than 20 files then scrolling ins not active.
                    indexes are set for seeing upper index amd lower index on screen as well as vector<string> file_name
                    */
                if(k3==UP)
                {
                    
                    if(IND.low_index<IND.cur_index && IND.cur_index>20)
                    {
                      --IND.cur_index;
                        clear_screen();
                        cout << "\033[1;31m"<<PAT_VAL.CUR_PATH<<"\033[0m\n";
                        print_list(file_names,IND.cur_index-20,IND.cur_index);
                        arrowUp();
                    }
                    else if(IND.cur_index<=20 && IND.cur_index>IND.low_index)
                    {
                        arrowUp();
                        --IND.cur_index; 
                    }
                }
                else if(k3==DOWN)
                {
                    if(IND.high_index<=IND.max_index )
                    {
                        if(IND.cur_index<IND.high_index)
                        {
                            arrowDown();
                            ++IND.cur_index;
                        }
                    }
                    else if(IND.cur_index<20)
                    {
                        arrowDown();
                        ++IND.cur_index;
                    }
                    else if(IND.cur_index>=20 && IND.cur_index<IND.high_index)
                    {
                        ++IND.cur_index;
                        clear_screen();
                        cout << "\033[1;31m"<<PAT_VAL.CUR_PATH<<"\033[0m\n";
                        print_list(file_names,IND.cur_index-20,IND.cur_index);
                        arrowUp();
                    }
                }
                else if(k3==LEFT)
                {
                    if(!tmp.empty())
                    {
                        /*following code snipped is used for printing file list on screen
                        stack is maintained,vector<string> file_name is updated for new files
                        current path is also updated */
                        stored_paths.push(tmp.top());
                        tmp.pop();
                        string path=stored_paths.top();
                        PAT_VAL.CUR_PATH=path;
                        file_names.clear();
                        getdir(path,file_names);
                        change_working_directory(PAT_VAL.CUR_PATH);

                        set_indices(file_names,&IND);
                        if(IND.high_index<IND.max_index)
                        {
                            print_list(file_names,0,IND.high_index);
                            set_cursor(IND.high_index);
                        }
                        else
                        {
                            print_list(file_names,0,IND.max_index);
                            set_cursor(IND.max_index);
                        }
                    }
            
                }
                /* left arrrw key ,explorer goes back in directory if exists,does not work in root directory */
                else if(k3==LEFT)
                {
                    if(stored_paths.size()>1)
                    {
                        tmp.push(stored_paths.top());
                        stored_paths.pop();
                        PAT_VAL.CUR_PATH=stored_paths.top();
                        file_names.clear();
                        getdir(PAT_VAL.CUR_PATH,file_names);
                        change_working_directory(PAT_VAL.CUR_PATH);
                        set_indices(file_names,&IND);
                        if(IND.high_index<IND.max_index)
                        {
                            print_list(file_names,0,IND.high_index);
                            set_cursor(IND.high_index);
                        }
                        else
                        {
                            print_list(file_names,0,IND.max_index);
                            set_cursor(IND.max_index);
                        }
                    }
                }
            }
        }
        /* file open by pressing enter key when cursor is on that file line done by extracting file mimetype*/
        else if(k1==ENTER)
        {
            string p=PAT_VAL.CUR_PATH+"/"+file_names[IND.cur_index];
            if(!isDir(p.c_str()))
            {
                char* x = const_cast<char*>(p.c_str());
                /*getting mime type and default application.desktop*/
                string str=get_mime_type(x);
                str=str.substr(0,str.size()-1);
                if(str!="")
                {
                    string r=".desktop";
                    std::string::size_type i = str.find(r);
                    if (i != std::string::npos)
                        str.erase(i,r.length());
                    if (fork()== 0)
                    {

                        string path="/usr/bin/"+str;
                        execl(path.c_str(), "", p.c_str(), (char *)0);
                        exit(1);
                    }
                }
               /* if (fork()== 0)
                {

                    execl("/usr/bin/xdg-open", "xdg-open", p.c_str(), (char *)0);
                    exit(1);
                }*/
            }
            else
            {   
                /* enter key press on directory */
                if((file_names[IND.cur_index]!="..") || (file_names[IND.cur_index]==".." && PAT_VAL.CUR_PATH!=PAT_VAL.HOME_VAL))
                {
                    
                    string path=file_names[IND.cur_index];
                    if(path=="..")
                    {
                        /* remove ".." and set path as current path*/
                        size_t found=PAT_VAL.CUR_PATH.find_last_of("/\\");
                        PAT_VAL.CUR_PATH=PAT_VAL.CUR_PATH.substr(0,found);
                    }
                    /*handles  "." case  */
                    else if(path==".")
                    {
                       // continue;
                        PAT_VAL.CUR_PATH=PAT_VAL.CUR_PATH;
                    }
                    else
                    {
                        PAT_VAL.CUR_PATH+="/"+path;
                    }
                    /* clear vector for new files (to be converted in new method)*/
                    file_names.clear();
                    getdir(path,file_names);
                    change_working_directory(PAT_VAL.CUR_PATH);
                    stored_paths.push(get_current_working_directory());
                    set_indices(file_names,&IND);
                    if(IND.high_index<IND.max_index)
                    {
                        print_list(file_names,0,IND.high_index);
                        set_cursor(IND.high_index);
                    }
                    else
                    {
                        print_list(file_names,0,IND.max_index);
                        set_cursor(IND.max_index);
                    }
                }
            }
        }

        /* going home folder on pressing h (home is directory where proggram was launched) */

        else if(k1==HOME)
        {
                file_names.clear();
                PAT_VAL.CUR_PATH=PAT_VAL.HOME_VAL;
                getdir(PAT_VAL.HOME_VAL,file_names);
                change_working_directory(PAT_VAL.HOME_VAL);
                set_indices(file_names,&IND);
                if(IND.high_index<IND.max_index)
                {
                    print_list(file_names,0,IND.high_index);
                    set_cursor(IND.high_index);
                }
                else
                {
                    print_list(file_names,0,IND.max_index);
                    set_cursor(IND.max_index);
                }
        }
        /* Backspace key press (same as left arrow key)*/
        else if(k1==BACK)
        {
            if(stored_paths.size()>1)
                    {
                        tmp.push(stored_paths.top());
                        stored_paths.pop();
                        PAT_VAL.CUR_PATH=stored_paths.top();
                        file_names.clear();
                        getdir(PAT_VAL.CUR_PATH,file_names);
                        change_working_directory(PAT_VAL.CUR_PATH);
                        set_indices(file_names,&IND);
                        if(IND.high_index<IND.max_index)
                        {
                            print_list(file_names,0,IND.high_index);
                            set_cursor(IND.high_index);
                        }
                        else
                        {
                            print_list(file_names,0,IND.max_index);
                            set_cursor(IND.max_index);
                        }
                    }
        }
        /* Entering into command mode by pressing ':' */
        else if(k1==SWITH_TO_COMMAND)
        {
            MODE=1;
            string p=get_current_working_directory();
            change_working_directory(p);
            cout<<"\033["<<gettermlines()-IND.cur_index<<"B";
        }

        /*exit fileass by 'q' quit  */
        else if(k1==QUIT)
        {
            clear_screen();
            break;
        }
    }
    /*command mode*/ 
    else if(MODE==1)
    {
        /* title bar of terminal window */
        cout<<"\033]0;File Explorer (Command Mode)\007";
       char c;
       vector<char> buf;
       while(MODE==1)
       {
           /* The following code snipped works in non-canonical mode 
            a character vector is continuosly filled as user 
            in command in command mode until backspace or enter key is
             pressed */
           c=getKey();
           if(c!=ENTER && c!=ESC && c!=BACK)
           {
               cout<<c;
               buf.push_back(c);
           }
           else
           {
              if(c==ESC)
              {
                MODE=0;
                clear_line();
                cout<<"\033["<<gettermlines()-IND.cur_index-2<<"A";
                break;
              }
              else if(c==BACK)
              {
                  buf.clear();
                  clear_line();
              }
              else if(c==ENTER)
              {
                bool gotof=false;
                  /* After typing command and pressing enter this code snipped tokenises the character vector
                  and sends to processCommand method  */
                vector<string> tokens;
                clear_line();
                string str;
                if(!buf.empty())
                str=string(buf.begin(),buf.end());
                clear_line();
                if(str!="")
                {
                    std::istringstream iss(str);
                    for(std::string s; iss>>s;)
                    {
                        tokens.push_back(s);
                    }
                    if(tokens[0]!="goto" && tokens[0]!="search")
                    {
                        processCommand(tokens,get_current_working_directory());
                    }
                    else if(tokens[0]=="goto")
                    {
                        
                        struct stat info;
                        if( stat(tokens[1].c_str(), &info )!=0)
                        {
                            cout<<"cannot access directory";
                        }
                        else
                        {
                            gotof=true;
                            MODE=0;
                            change_working_directory(tokens[1]);
                            file_names.clear();
                            PAT_VAL.CUR_PATH=get_current_dir_name();
                            getdir(PAT_VAL.CUR_PATH,file_names);
                            change_working_directory(PAT_VAL.CUR_PATH);
                            set_indices(file_names,&IND);
                            if(IND.high_index<IND.max_index)
                            {
                                print_list(file_names,0,IND.high_index);
                                set_cursor(IND.high_index);
                            }
                            else
                            {
                                print_list(file_names,0,IND.max_index);
                                set_cursor(IND.max_index);
                            }
                        }
                    }
                    else if(tokens[0]=="search")
                    {
                        search(get_current_working_directory(),tokens[1],paths);
                        if(paths.size()>0)
                        {    
                            for(unsigned int i=0;i<paths.size();i++)
                            {
                                size_t found=paths[i].find_last_of("/\\");
                                paths[i]=paths[i].substr(0,found);                            
                            }
                            set_indices(paths,&IND);
                            if(IND.high_index<IND.max_index)
                            {
                                print_search_result(paths,0,IND.high_index);
                                set_cursor(IND.high_index);
                            }
                            else
                            {
                                print_search_result(paths,0,IND.max_index);
                                set_cursor(IND.max_index);
                            }
                            MODE=2;
                        }
                        else
                        {
                            cout<<"File not found";
                        }
                    }
                }
                buf.clear();
                str.clear();
                if(tokens[0]!="goto" && tokens[0]!="search")
                {
                    getKey();
                    clear_line();
                }
                else if(gotof==false)
                {
                    getKey();
                    clear_line();
                }
                tokens.clear();
              }
           }
       }
    }
    else if(MODE==2)
    {
       cout<<"\033]0;File Explorer (Normal Mode)\007";
       while(MODE==2)
        {
            int p1=' ',p2=' ',p3=' ';
            p1=getKey();
            if(p1==27)
            {
                p2=getKey();
                if(p1==27 && p2==91)
                {
                    p3=getKey();
                    if(p3==UP)
                    {
                        if(IND.low_index<IND.cur_index && IND.cur_index>20)
                        {
                            --IND.cur_index;
                            clear_screen();
                            print_search_result(paths,IND.cur_index-20,IND.cur_index);
                            arrowUp();
                        }
                        else if(IND.cur_index<=20 && IND.cur_index>IND.low_index)
                        {
                            arrowUp();
                            --IND.cur_index; 
                        }    
                    }
                    else if(p3==DOWN)
                    {
                        if(IND.high_index<=IND.max_index )
                        {
                            if(IND.cur_index<IND.high_index)
                            {
                                arrowDown();
                                ++IND.cur_index;
                            }
                        }
                        else if(IND.cur_index<20)
                        {
                            arrowDown();
                            ++IND.cur_index;
                        }
                        else if(IND.cur_index>=20 && IND.cur_index<IND.high_index)
                        {
                            ++IND.cur_index;
                            clear_screen();
                            print_search_result(paths,IND.cur_index-20,IND.cur_index);
                            arrowUp();
                        }
                    }
                }
            }
            else if(p1==ENTER)
            {
                file_names.clear();
                getdir(paths[IND.cur_index],file_names);
                change_working_directory(paths[IND.cur_index]);
                set_indices(file_names,&IND);
                if(IND.high_index<IND.max_index)
                {
                    print_list(file_names,0,IND.high_index);
                    set_cursor(IND.high_index);
                }
                else
                {
                    print_list(file_names,0,IND.max_index);
                    set_cursor(IND.max_index);
                }
            }
            else if(p1==BACK)
            {
                 set_indices(paths,&IND);
                if(IND.high_index<IND.max_index)
                {
                    print_search_result(paths,0,IND.high_index);
                    set_cursor(IND.high_index);
                }
                else
                {
                    print_search_result(paths,0,IND.max_index);
                    set_cursor(IND.max_index);
                }
            }
            else if(p1==HOME)
            {
                file_names.clear();
                PAT_VAL.CUR_PATH=PAT_VAL.HOME_VAL;
                getdir(PAT_VAL.HOME_VAL,file_names);
                change_working_directory(PAT_VAL.HOME_VAL);
                set_indices(file_names,&IND);
                if(IND.high_index<IND.max_index)
                {
                    print_list(file_names,0,IND.high_index);
                    set_cursor(IND.high_index);
                }
                else
                {
                    print_list(file_names,0,IND.max_index);
                    set_cursor(IND.max_index);
                }
                MODE=0;
            }
        }
    }
}
    return 0;
}
