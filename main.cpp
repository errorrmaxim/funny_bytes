#include "printer_foo.h"


int main(int argc, char *argv[])
{
    /*Third arg is delay value*/
    int dly;
    if(argc == 3)
    {
        int check = strcmp(argv[1], "-d");
        dly = atoi(argv[2]);

    }
    else
    {
        dly = 1;
    }

        vector<proc_info>process_vec = pid_vec();
        vector<proc_info>process_vec_tmp;

    initscr();


    sort_fctr s;
    noecho();
    nodelay(stdscr, true);


        int c, fg;
    while (1) {
        clear();

        mvprintw (0, 0, "%s", system_info().c_str());
        mvprintw(5, 0, "%s", get_header().c_str());
        mvprintw(8, 0, "%s", set_rows(process_vec).c_str());
        c = getch();

        switch (c) {
        case 'P': s.set_flag(PID); sort(process_vec.begin(), process_vec.end(), s);break;
        case 'V':s.set_flag(VIRT); sort(process_vec.begin(), process_vec.end(), s);break;
        case 'p': s.set_flag(CPU); sort(process_vec.begin(), process_vec.end(), s);break;
        case 'M': s.set_flag(MEM); sort(process_vec.begin(), process_vec.end(), s);break;
        case 't': s.set_flag(TIME);sort(process_vec.begin(), process_vec.end(), s); break;
        case 'R': s.set_flag(RES); sort(process_vec.begin(), process_vec.end(), s);break;
        case 'C': s.set_flag(CMD); sort(process_vec.begin(), process_vec.end(), s);break;
        case 'q': exit(0); break;
        default:
            break;
        }
        if(fg == 10)
         {
           process_vec_tmp = pid_vec();
         }
        if(fg == 19)
         {
            process_vec = process_vec_tmp;
            fg = 0;
         }
            fg++;
           refresh ();
           sleep (dly);
    }

   endwin();
    return 0;
}
