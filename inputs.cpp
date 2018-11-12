/*
    Name:Ajay Subhash Jadhav
    Roll No:2018201095
*/
#include <iostream>
#include <termios.h>
#include "inputs.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
using namespace std;

void arrowUp()
{
    cout<<"\033[1A";
}
void arrowDown()
{
    cout<<"\033[1B";
}
void arrowLeft()
{
    cout<<"\033[1C";
}
void arrowRight()
{
    cout<<"\033[1D";
}
void set_cursor(int pos)
{
    cout<<"\033["<<pos+1<<"A";
}
void clear_screen(void)
{
    cout<<"\033c";
}
int getKey()
{
    int c;
    struct termios new_set,old_set;
    tcgetattr(STDIN_FILENO, &old_set);
    new_set = old_set;
    new_set.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_set);
    c=getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_set);
    return c;
}
int gettermlines()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    return w.ws_row;
}
void clear_line(void)
{
    printf("%c[2K", 27);
    cout<<"\r";
}
