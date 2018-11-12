/*
    Name:Ajay Subhash Jadhav
    Roll No:2018201095
*/
#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>
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
#include "commands.h"
#include "fileops.h"

#define MIMETYPE "/etc/mime.types"
#define DEFAULT_LIST "/usr/share/applications/defaults.list"

using namespace std;
/* recusrive delete for deleting directory*/
int rec_delete(string dir_name)
{
    DIR *dir;
    struct dirent *dirp;
    string path;
    dir=opendir(dir_name.c_str());
    if(dir==NULL)
    {
        perror("Error opeining directory");
        return 0;
    }

    while ((dirp= readdir(dir))!=NULL)
     {
        if (strcmp(dirp->d_name, ".") && strcmp(dirp->d_name, "..")) 
        {
            string file(dirp->d_name);
            path=dir_name+"/"+file;
            if(dirp->d_type==DT_DIR)
            {
                rec_delete(path);
            }
            remove(path.c_str());
        }
    }
    closedir(dir);
    remove(dir_name.c_str());
    return 1;
}
/*search works for multiple files rturns the vector of patshs where file is found */
vector<std::string> search(string x,string file_name,vector<std::string>& paths) 
{
    static string found;
    found="0";
	DIR *dp;	
	dp=opendir(x.c_str());
	if(dp==NULL)
    {
		cerr<<endl<<"Error opening directory"<<x<<"(errno:"<<errno<<")"<< endl;
	}
	
	struct dirent *dirp;
	struct stat s;
	string p;
	while((dirp=readdir(dp)))
	{

		if(!strncmp(dirp->d_name, ".", 1) || !strncmp(dirp->d_name, "..", 2))
			continue;
		if(x==".") 
		{
			p=dirp->d_name;
		}
		else	
		{
	    	p = x+"/"+dirp->d_name;
		}
		if(stat(p.c_str(),&s))
			 continue;
		if(S_ISDIR(s.st_mode)) 
		{
			search(p,file_name,paths);
			continue;
		}
		if(dirp->d_name==file_name)
		{
		   paths.push_back(p);
		}
	}
	closedir(dp);
	return paths;
}   
string get_cur()
{
    char buf[256];
    char* x = getcwd(buf,256);
    if(x) return std::string(x);
    return ""; 
}

bool is_dir(const char* path)
{
    struct stat f;
    stat(path, &f);
    return S_ISDIR(f.st_mode);
}
/* create file  */
int create_file(string file_name,string path)
{
   string filep=path+"/"+file_name;
   fstream file;
   file.open(filep,ios::out);
   if(!file)
   {
       return 0;
   }
   file.close(); 
   return 1;
}
/*create directory */
int create_directory(string directory,string path)
{
    int status;
    path=path+"/"+directory;
    status=mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    return status;
}
/* copy file when given source and destination*/
void copyf(string source,string dest)
{
    FILE *fp1, *fp2;
    int key;
    fp1=fopen(source.c_str(), "rb");
    if(fp1==NULL)
    {
        fprintf(stderr, "Can't open source file");
    }
    fp2=fopen(dest.c_str(), "wb");

    if(fp2==NULL)
    {
        fclose(fp1);
        fprintf(stderr, "Can't open dest file");
    }

    while((key=fgetc(fp1))!=EOF)
    {
        fputc(key,fp2); 
    }

    fclose(fp1);
    fclose(fp2);
}
void copydir(const char *dir, string output_dir)
{

    DIR *dirp;
    struct dirent *entry;
    string tmp_str,tmp_path,output_path;
    string input_dir=dir;

    if(is_dir(dir)==false)
    {
        cout<<"Can't open!"<< endl;
        return;
    }
    if((dirp=opendir(dir))!=NULL)
    {
        while((entry=readdir(dirp))!=NULL) 
        {
            tmp_str=entry->d_name;
            if( strcmp(entry->d_name, ".")  != 0 && strcmp(entry->d_name, "..") != 0 )
            {
                tmp_path=input_dir;
                tmp_path=tmp_path+"/"+tmp_str;
                if(is_dir(tmp_path.c_str()))
                {
                    output_path=output_dir;
                    output_path=output_path+"/"+tmp_str;
                    mkdir(output_path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                    copydir(tmp_path.c_str(), output_path);
                }
                else
                {
                    output_path=output_dir;
                    output_path=output_path+"/"+tmp_str;
                    copyFile(tmp_path.c_str(), output_path.c_str());
                }
            }
        }
        closedir(dirp);
    }
}
void copyFile(string inDir, string outDir)
{
    copyf(inDir,outDir);    
}
void copy_files(vector<string> files,string destination,string home)
{
    for(unsigned int i=1;i<=files.size()-2;i++)
    {
        string src=home+"/"+files[i]; 
        string dest=destination+"/"+files[i];
        if(!is_dir(src.c_str()))
        {
            copyf(src,dest);
        }
        else
        {
            string dest=(destination+"/"+files[i]);
            mkdir(dest.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
            copydir(src.c_str(),dest);
        }
    }
    cout<<"Task Finished";
}
 /* move file copies file to the destination folder and afterwards deletes from source folder*/
void move_files(vector<string> tokens,string home)
{
    int count=0;
    for(unsigned int i=1;i<=tokens.size()-2;i++)
    {
        string src=home+"/"+tokens[i];
        string dest=tokens[tokens.size()-1]+"/"+tokens[i];
        if(!isDir(src.c_str()))
        {
            copyf(src,dest);
            remove(src.c_str());
            count++;
        }
        else
        {
            mkdir(dest.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
            copydir(src.c_str(),dest);
            rec_delete(src);
        }
    }
    cout<<" "<<count+" files moved successfully";
}

void rename_file(string old_name,string new_name)
{
    copyf(old_name,new_name);
    remove(old_name.c_str());
    cout<<"file renamed";
}

/*Below function dumps the content of directory in dump file (in current working directory) implemented using 
iterative BFS walk among directories */
void make_snapshot(fstream& fs,queue<string>& que,string path)
{
    while(!que.empty())
    {
        DIR *dp;
        string x;
        dirent *dirp;
        fs<<path<<endl;	
        dp=opendir(que.front().c_str());
        if(dp==NULL)
        {
            cout<<"opening failed";
        }

        while((dirp=readdir(dp)))
        {

            if(!strncmp(dirp->d_name, ".", 1) || !strncmp(dirp->d_name, "..", 2))
                continue;
            x=que.front()+"/"+dirp->d_name;
            struct stat s;
            stat(x.c_str(),&s);
            if(S_ISDIR(s.st_mode)) 
            {
                fs<<x<<endl;
                que.push(x);
            }
            else
            {
                fs<<"\t\t"<<dirp->d_name<<endl;
            }
        }
        que.pop();
        closedir(dp);
    }
    cout<<"Dump file created in current directory";
    return;
}
/* command mode commands are processed in the function all the major functions are called from here */
void processCommand(vector<string> tokens,string home)
{
    
    if(tokens[0]=="copy_file")
    {
        if(tokens.size()>=3)
        {
            copy_files(tokens,tokens[tokens.size()-1],home);
        }
        else
        {
            cout<<"Invalid Parameters";
        }
    }
    else if(tokens[0]=="delete_file")
    {
        if(tokens.size()==3)
        {
            if(tokens[2]==".")
            {
                if(remove(tokens[1].c_str())!=0)
                {
                    cout<<"File deletion failed";
                }
                else
                {
                    cout<<"File deleted";
                }
            }
            else
            {
                string p=home+"/"+tokens[2]+"/"+tokens[1];
                if(remove(p.c_str())!=0)
                {
                    cout<<"File deletion failed";
                }
                else
                {
                    cout<<"File deleted";
                }
            }
        }
        else
        {
         cout<<"Invalid parameters";   
        }
    }
    else if(tokens[0]=="delete_dir")
    {
        if(tokens.size()==3)
        {
            if(tokens[2]==".")
            {
                string p=home+"/"+tokens[1];
                rec_delete(p);
            }
            else 
            {
                string p=home+"/"+tokens[2];
                if(rec_delete(p))
                    cout<<"Directory deleted successfully";
                else
                    cout<<"Directory deletion failed";
            }
        }
        else
        {
            cout<<"Invalid Parameters";
        }
    }
    else if(tokens[0]=="create_file")
    {
        if(tokens.size()==3)
        {
            if(tokens[2]==".")
            {
                if(create_file(tokens[1],home)==1)
                {
                    cout<<"File successfully created";
                }
                else
                {
                    cout<<"File creation Failed";
                }
            }
        }
        else
        {
            cout<<"Invalid properly";
        }
    }
    else if(tokens[0]=="create_directory")
    {
        if(tokens.size()==3)
        {
            if(tokens[2]==".")
            {
                if(create_directory(tokens[1],home)!=0)
                    cout<<"Directory creation failed";
                else
                    cout<<"Directory creation successful";
            }
            else
            {
                string p=home+"/"+tokens[2];
                if(create_directory(tokens[1],p)!=0)
                    cout<<"Directory creation failed";
                else
                    cout<<"Directory creation successful";
            }
        }
        else
        {
            cout<<"Invalid Parameters";
        }
    }
    else if(tokens[0]=="move_file")
    {
       if(tokens.size()>=3)
        {
           move_files(tokens,home);
        }
        else
        {
            cout<<"Invalid Parameters";
        }
    }
    else if(tokens[0]=="rename")
    {
        if(tokens.size()==3)
        { 
           rename_file(home+"/"+tokens[1],home+"/"+tokens[2]);
        }
        else
        {
            cout<<"Invalid Parameters";
        }
    }
    else if(tokens[0]=="snapshot")
    {   
        if(tokens.size()==2)
        {
            queue<string> que;
            que.push(tokens[1]);
            fstream fs;
            fs.open (get_cur()+"/"+"dump",std::fstream::in|std::fstream::out|std::fstream::app);
            fs<<get_cur()<<endl;
            make_snapshot(fs,que,que.front());
            fs.close();   
        }
        else
        {
            cout<<"Invalid Parameters";
        }
    }
    else
    {
        cout<<"Invalid command";
    }
}