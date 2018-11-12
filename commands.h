/* 
    Name:Ajay Subhash Jadhav
    Roll No:2018201095
*/
#ifndef COMMANDS_H
#define COMMANDS_H

void copyf(std::string,std::string);
void processCommand(std::vector<std::string>,std::string);
int create_file(std::string,std::string);
int create_directory(std::string,std::string);
void rename_file(std::string,std::string);
void copy_files(std::vector<std::string>,std::string,std::string);
std::string get_mime_type(char *); 
std::vector<std::string> search(std::string ,std::string,std::vector<std::string>&);
void processCommand(std::vector<std::string>,std::string);
std::string get_cur(void);
bool is_dir(const char *);
void copyFile(std::string,std::string);
void copydir(const char *, std::string);
int rec_delete(std::string);


#endif