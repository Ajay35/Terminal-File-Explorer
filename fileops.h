/*
    Name:Ajay Subhash Jadhav
    Roll No:2018201095
*/
#ifndef FILEOPS_H
#define FILEOPS_H
struct INDICES
    {
        int cur_index;
        int low_index;  
        int high_index;
        int max_index;
    };
struct SEARCH_INDICES
    {
        int cur_index;
        int low_index;  
        int high_index;
        int max_index;
    };
struct PATH_VALUES
    {
        std::string HOME_VAL;
        std::string CUR_PATH;
    };
void print_list(std::vector<std::string> ,int,int);
void set_indices(std::vector<std::string>,struct INDICES *);
void getdir(std::string,std::vector<std::string>&);
bool isDir(const char*);
void set_paths(std::string,struct PATH_VALUES *);
std::string get_current_working_directory(void);
void change_working_directory(std::string );
char* get_permissions(char *);
std::string get_uname(uid_t);
std::string get_grp_name(gid_t);
void print_search_result(std::vector<std::string>,int,int);
void set_search_indices(std::vector<std::string>,struct SEARCH_INDICES *);

#endif