fndef PRINTER_FOO_H
#define PRINTER_FOO_H



#include <cmath>
#include <unistd.h>
#include <iostream>
#include <proc/readproc.h>
#include <proc/sysinfo.h>
#include <proc/version.h>
#include <sys/sysinfo.h>
#include <pwd.h>
#include <ctime>
#include <fcntl.h>
#include <iomanip>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ncurses.h>
#include <pthread.h>
#include <algorithm>
#include <cstring>

using namespace std;


class proc_info
{
private:

        char p_state;                       //state of proccess                 +

        pid_t pid;                          //proc id                           +

        double p_virt_m,                    //virt virtual image                +
              p_res,                        //equals 'rss' and/or 'resident'    +
              p_mem,                        //%MEM
              p_cpu;                        //%CPU                              +


        tm *since_boot_t;                   //time since boot system            +

        proc_t p_inf;                      //pointer to structure 'proc_t'

        string p_command,                   //name of proccess                  +
               u_name,                      //user name                         +
               time_active;                 //TIME++
        long  p_PR,                         //kernel scheduling priority        +
              p_NI;                         //"NICE"                            +

        struct sysinfo memInfo;            //information about memory state

public:
    proc_info();
    proc_info(pid_t);

    void SET_PCOMMAND();
    void SET_UNAME();
    void SET_PR();
    void SET_NI();
    void SET_VIRT();
    void SET_RES();
    void SET_MEM();
    void SET_CPU();
    void SET_STATE();
    void SET_PACTIV();

    char GET_STATE();
    long GET_PR();
    long GET_NI();
    string GET_PCOMMAND() const;
    string GET_UNAME();
    double GET_VIRT() const;
    double GET_RES() const;
    double GET_MEM() const;
    double GET_CPU() const;         //+
    string GET_PACTIV() const;      //+
    pid_t GET_PID() const;          //+


};
vector<proc_info>pid_vec();
extern void printer_tab(std::vector<proc_info>&t_p);
extern string get_now();
extern string system_info();
extern string get_header();
extern string set_rows(std::vector<proc_info>&v);
extern void menu(char ch);



enum sort_flags{PID = 'P', VIRT = 'V', CPU = 'p', MEM = 'M', TIME = 't', RES = 'R', CMD = 'C'};



typedef proc_info p_inf;
auto by_pid = [](const p_inf& lhs, const p_inf& rhs) {return (lhs.GET_PID() < rhs.GET_PID());};
auto by_virt = [](const p_inf& lhs, const p_inf& rhs) {return lhs.GET_VIRT() > rhs.GET_VIRT();};
auto by_act = [](const p_inf& lhs, const p_inf& rhs) { int answ = std::strcmp(lhs.GET_PACTIV().c_str(), rhs.GET_PACTIV().c_str());
    if(answ > 0) { return true; }
                   return false;};
auto by_cpu = [](const p_inf& lhs, const p_inf& rhs) { return lhs.GET_CPU() > rhs.GET_CPU();};
auto by_mem = [](const p_inf& lhs, const p_inf& rhs) { return lhs.GET_MEM() < rhs.GET_MEM();};
auto by_res = [](const p_inf& lhs, const p_inf& rhs) { return lhs.GET_RES() > rhs.GET_RES();};
auto by_cmd = [](const p_inf& lhs, const p_inf& rhs) { int answ = std::strcmp(lhs.GET_PCOMMAND().c_str(), rhs.GET_PCOMMAND().c_str());
    if(answ > 0) {return true; }
                  return false;};


//class for
class sort_fctr
{
    sort_flags fg;
public:
    sort_fctr() {}
    sort_fctr(sort_flags _fg): fg(_fg) {}
    bool operator ()(const proc_info& lhs, const proc_info& rhs)
    {
        switch (fg) {
            case 'P':by_pid(lhs, rhs); break;
            case 't':by_act(lhs, rhs); break;
            case 'p':by_cpu(lhs, rhs); break;
            case 'M':by_mem(lhs, rhs); break;
            case 'R':by_res(lhs, rhs); break;
            case 'V':by_virt(lhs, rhs); break;
            case 'C':by_cmd(lhs, rhs); break;

        default:
            break;
        }
    }
    void set_flag(char ch)
    {
        switch (ch) {
             case 'P': fg = PID; break;
             case 'V': fg = VIRT; break;
             case 'p': fg = CPU; break;
             case 'M': fg = MEM; break;
             case 't': fg = TIME; break;
             case 'R': fg = RES; break;
             case 'C': fg = CMD; break;

        default:
            break;
        }
    }


};

template<typename T>
inline T convert_unit( T num, int n)
{

  for( int i = 0; i < n; i++)
  {
    num /= 1024;
  }
  return num;
}

template<typename T>
T my_round(T pcpu, int N)
{
    static const double powerOfTen[] = { 1.0, 10.0, 100.0, 1000.0};
    pcpu = std::floor(pcpu * powerOfTen[N]) / powerOfTen[N];
    return pcpu;
}


template<typename T>
std::string ret_member(T m)
{
    std::string ss = std::to_string(m);
    int pos = ss.length() - 5;
    ss = ss.substr(0, pos);

   return ss;
}

#endif // PRINTER_FOO_H

