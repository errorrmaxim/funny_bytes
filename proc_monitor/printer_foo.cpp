#include "printer_foo.h"

proc_info::proc_info():pid(0)
{
    cout << __PRETTY_FUNCTION__ << endl;
}

proc_info::proc_info(pid_t _pid):pid(_pid)
{
    sysinfo (&memInfo);
    get_proc_stats(pid, &p_inf);

     SET_PCOMMAND();
     SET_UNAME();
     SET_PR();
     SET_NI();
     SET_VIRT();
     SET_RES();
     SET_MEM();
     SET_CPU();
     SET_PACTIV();
     SET_STATE();

}

void proc_info::SET_UNAME()
{
        passwd *pw = getpwuid(static_cast<uid_t>(p_inf.euid));
        u_name = pw->pw_name;
}

void proc_info::SET_PR()
{
      p_PR = p_inf.priority;
}

void proc_info::SET_NI()
{
    p_NI = p_inf.nice;
}

void proc_info::SET_VIRT()
{
    double tmp = convert_unit(static_cast<double>(p_inf.vsize), 2);
    p_virt_m = my_round(tmp, 1);
}

void proc_info::SET_PCOMMAND()
{
    char buff[PATH_MAX];
    string path = "/proc/" + to_string(pid) + "/cmdline";

    int fd = open(path.c_str(), O_RDONLY);
    size_t sz = read(fd, buff, 128);
    p_command = buff;
}

void proc_info::SET_RES()
{
    p_res = convert_unit(static_cast<double>(p_inf.vm_rss),1);
}

void proc_info::SET_STATE()
{
    p_state = p_inf.state;
}

void proc_info::SET_PACTIV()
{
    stringstream ss;
    double tot= (p_inf.utime + p_inf.stime)/ static_cast<double>(Hertz);
    int H = tot / 60;
    double min_sec = fmod(tot, 60);

     ss << H << ':' << setw(5) << setfill('0') << fixed << setprecision(2) << min_sec;

     time_active = ss.str();

}
void proc_info::SET_CPU()
{
    time_t total = p_inf.utime + p_inf.stime;
    time_t seconds_since_boot = uptime(NULL,NULL);
    time_t seconds = seconds_since_boot - p_inf.start_time / Hertz;
    double pcpu = static_cast<double>((total * 1000ULL / Hertz) / (seconds*10.0));
    p_cpu = my_round(pcpu, 1);

}

void proc_info::SET_MEM()
{
    unsigned tot_ram = memInfo.totalram / 1024;
    double rss = my_round(static_cast<double>((p_inf.vm_rss * 100.0) / tot_ram), 1);
    p_mem = rss;
}



//------------------------------------------------------------------------



string proc_info::GET_UNAME()
{
    int str_sz = u_name.length();
    if(str_sz > 7)
    {
        u_name = u_name.substr(0, 6) + '+';
    }
       return u_name;
}

long proc_info::GET_PR()
{
        return p_PR;
}

long proc_info::GET_NI()
{
    return p_NI;
}

double proc_info::GET_VIRT() const
{
  return p_virt_m;
}

string proc_info::GET_PCOMMAND() const
{
    int str_sz = p_command.length();
string tmp;

    if(str_sz > 20)
    {
     tmp = p_command.substr(str_sz-20);
     return tmp;
    }
    else {
        return p_command;
         }

}

double proc_info::GET_RES() const
{
   return my_round(p_res, 2);
}

char proc_info::GET_STATE()
{
    return p_state;
}

string proc_info::GET_PACTIV() const
{
    return time_active;
}

double proc_info::GET_CPU() const
{
   return my_round(p_cpu, 1);
}

double proc_info::GET_MEM() const
{
   return my_round(p_mem, 2);

}

pid_t proc_info::GET_PID() const
{
    return pid;
}









string set_rows(std::vector<proc_info>&v)
{
    stringstream ss;
    for(auto it = v.begin(); it != v.end();++it)
    {
    ss << setw(5) << setfill(' ') << it->GET_PID();
    ss << setw(9) << setfill(' ') <<  it->GET_UNAME();
    ss << setw(5) << setfill(' ') << it->GET_PR() << setw(4) << setfill(' ') << it->GET_NI()\
       << setw(8) << setfill(' ') << it->GET_VIRT() << setw(7) << setfill(' ') << it->GET_RES()\
       << setw(6) << setfill(' ') << it->GET_CPU() << setw(5) << setfill(' ') << it->GET_MEM()\
       << setw(10) << setfill(' ') << it->GET_PACTIV() << setw(3) << setfill(' ') << it->GET_STATE() << ' '\
       << setw(16) << setfill(' ') << it->GET_PCOMMAND() << endl;
    }
    return ss.str();

}


string get_header()
{
    stringstream ss;

    ss << string(80, '-') << endl << setw(5) << setfill(' ') << "PID" << setw(9) << setfill(' ') << "USER  "\
       << setw(5) << setfill(' ') << "PR" << setw(4) << setfill(' ') << "NI"\
       << setw(8) << setfill(' ') << "VIRT" << setw(7) << setfill(' ') << "RES"\
       << setw(6) << setfill(' ') << "%CPU" << setw(5) << setfill(' ') << "%MEM"\
       << setw(10) << setfill(' ') << "TIME+" << setw(3) << setfill(' ') << "S"\
       << setw(16) << setfill(' ') << "COMMAND" << endl << string(80, '-') << endl;

    return ss.str();
}

inline vector<proc_info>pid_vec()
{
    DIR *dir;
    struct dirent *entry;
    vector<proc_info>my_dir;
    pid_t pid_tmp;

    dir = opendir("/proc");
    if (!dir) {
        perror("Fatal error diropen...");
        exit(1);
    };
    while (entry = readdir(dir))
    {
            if (isdigit(entry->d_name[0]))
              {
                pid_tmp = atoi (entry->d_name);
                my_dir.push_back(proc_info(pid_tmp));
              }
    }
    closedir(dir);
    return my_dir;
}

string  get_now()
{
    stringstream ss;
    time_t seconds = time(NULL);
    tm* now = localtime(&seconds);
    ss << setw(2) << setfill('0') << now->tm_hour << ':'\
       << setw(2) << setfill('0') << now->tm_min << ':'\
       << setw(2) << setfill('0') << now->tm_sec;
    return ss.str();

}

string system_info()
{
    meminfo();
    double tatal_mem = my_round((kb_main_total/1024/1024.0), 1);
    double free_mem = my_round((kb_main_free/1024/1024.0), 1);
    unsigned tasks = pid_vec().size();
    stringstream ss;
    time_t seconds_since_boot = uptime(NULL,NULL);
    int d = seconds_since_boot / (24 * 3600);
    int h = (seconds_since_boot % (3600 *24)) / 3600;
    int m = (seconds_since_boot / 60) % 60;
    int s = seconds_since_boot % 60;
    ss << "my_top - " << get_now()  << " up " << d << " day, "<< setw(2) << setfill('0') << h << ":"\
       << setw(2) << setfill('0') << m << endl << "Tasks: " << tasks << " total." << endl << "CPU: " << smp_num_cpus\
       << endl << "GiB MeM  : " << tatal_mem << endl << "Free Mem : " << free_mem << endl;
    return ss.str();
}



