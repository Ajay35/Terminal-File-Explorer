/*
    Name:Ajay Subhash Jadhav
    Roll No:2018201095
*/
#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <pwd.h>
#include <grp.h>
#include <sstream>
#include <fstream>
#include <fcntl.h>  
#include <queue>
#include "inputs.h"
#include "fileops.h"
#include "commands.h"
/*Macros for file paths required for reading mime type of a file based on its extension */
#define MIMETYPE "/etc/mime.types"
#define DEFAULT_LIST "/usr/share/applications/defaults.list"

using namespace std;
/* set indices*/
void set_indices(vector<std::string> file_names,struct INDICES *a)
{
    a->low_index=0;
    a->high_index=file_names.size()-1;
    a->cur_index=0;
}
/*get file permissions in required format*/
char* get_permissions(const char *file)
{
    struct stat st;
    char *modeval=(char *)malloc(sizeof(char) * 9 + 1);
        stat(file, &st);
        mode_t p= st.st_mode;
        modeval[0] = (p & S_IRUSR) ? 'r' : '-';
        modeval[1] = (p & S_IWUSR) ? 'w' : '-';
        modeval[2] = (p & S_IXUSR) ? 'x' : '-';
        modeval[3] = (p & S_IRGRP) ? 'r' : '-';
        modeval[4] = (p & S_IWGRP) ? 'w' : '-';
        modeval[5] = (p & S_IXGRP) ? 'x' : '-';
        modeval[6] = (p & S_IROTH) ? 'r' : '-';
        modeval[7] = (p & S_IWOTH) ? 'w' : '-';
        modeval[8] = (p & S_IXOTH) ? 'x' : '-';
        modeval[9] = '\0';
    return modeval;        
}
/*print file list on terminal screen */
void print_list(vector<std::string> file_names,int l,int h)
{
    clear_screen();
    struct stat a;
    string tmp;
    cout << "\033[1;31m"<<get_current_working_directory()<<"\033[0m\n";
   
    for(int i=l;i<=h;i++)
     {
          if(file_names[i].size()>12)
          {
                tmp=file_names[i].substr(0,8)+"...";
          }
         else
            {   
                tmp=file_names[i];
                int spaces=12-(file_names[i].size());
                for(int i=1;i<=spaces;i++)
                     tmp+=" ";
            }
         stat(file_names[i].c_str(),&a);
         char mtime[80];
         time_t t = a.st_mtime;
         struct tm lt;
         localtime_r(&t,&lt);
         strftime(mtime, sizeof mtime, "%a, %d %b %Y %T", &lt);
         cout<<tmp<<"\t"<<a.st_size<<"B\t"<<get_permissions(file_names[i].c_str());
         cout<<"\t"<<get_uname(a.st_uid)<<"\t"<<get_grp_name(a.st_gid)<<" "<<mtime<<endl;
     }
}
/* Set paths on change of directory */
void set_paths(std::string p,struct PATH_VALUES *b)
{
    if(p==".")
    {
        std::string x=get_current_working_directory();
        b->CUR_PATH=x;
        b->HOME_VAL=x;
    }
    else
    {
        b->HOME_VAL=p;
        b->CUR_PATH=p;
    }
}
/* get all file names in vector*/
void getdir(std::string dir,std::vector<std::string>& file_names)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp =opendir(dir.c_str()))==NULL) 
    {
        cout << "Error( "<<errno<<") opening"<<dir<<endl;
    }
    while ((dirp=readdir(dp))!=NULL)
	{
        file_names.push_back(dirp->d_name);
    }   
    closedir(dp);
    std::sort(file_names.begin(),file_names.end());
}
void change_working_directory(std::string path)
{
    chdir(path.c_str());
}
bool isDir(const char* path)
{
    struct stat f;
    stat(path, &f);
    return S_ISDIR(f.st_mode);
}
/* return current directory in string format*/
std::string get_current_working_directory()
{
    char buf[256];
    char* x = getcwd(buf,256);
    if(x) return std::string(x);
    return "";  
}
/* get user name by user id*/
std::string get_uname(uid_t u)
{
    struct passwd *pw = getpwuid (u);
    return std::string(pw->pw_name);
}
/*get group name by group id */
std::string get_grp_name(gid_t g)
{
    struct group *grp=getgrgid(g);
    return std::string(grp->gr_name);
}
/* The below function gets mime type of file by extension checks the mime.type file in system folder
gets the default application from defaults.list and return the application name*/
string get_mime_type(char *name) 
{
    std::string filename(name);
	std::string::size_type idx;
	idx = filename.rfind('.');
	if(idx == std::string::npos)
	{
		string n="\n";
		return "gedit.desktop"+n;
	}
	char *ext = strrchr(name,'.');
	bool found=false;
	char *mime_type = NULL;
	string app="";
	mime_type = (char *)malloc(128*sizeof(char)) ;
	char line[128];
	ext++; 
	FILE *mime_type_file = fopen(MIMETYPE,"r");
	std::vector<string> tokens;
	if (mime_type_file!=NULL) 
	{
		while(fgets(line,sizeof line,mime_type_file) != NULL) 
		{
			string s=line;
			std::istringstream iss(s);
			for(std::string s; iss>>s;)
			{
				tokens.push_back(s);
			}
			for(unsigned int i=1;i<tokens.size();i++)
			{
				
				if(tokens[i]==ext)
				{
					strcpy(mime_type,tokens[0].c_str());
					found=true;
				}
			}
			if(found)
			{
			  string mim=mime_type;
			  FILE *default_list = fopen(DEFAULT_LIST,"r");
			  if (default_list!= NULL) 
				{
					while(fgets(line,sizeof line,default_list)!= NULL) 
					{
						string x=line;
						if(x.substr(0,mim.size())==mim)
						{
							app=x.substr(mim.size()+1,x.size()-mim.size());
							break;
						}
					}
				break;
				}
			}
			else
				tokens.clear();
		}
		fclose(mime_type_file);
	}
	else 
	{
		perror("WTF failed");
	}
	return app;
}
void print_search_result(vector<string> paths,int l,int h)
{
    clear_screen();
    for(int i=l;i<=h;i++)
    {
        cout<<paths[i]<<endl;
    }
}

