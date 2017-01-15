#include <iostream>
#include <cstring>
#include <cassert>


using namespace std;


enum count_val { FIRST = 0, SECOND = 8, THIRD = 16, FOURTH = 24};

void show_bin(int num)
{
    unsigned mask = 1 << 31;
    int j = 0;
        while (mask)
        {
            if(j == 8 || j == 16 || j == 24)
            {
                cout << '|';

            }
            if(mask & num)
                cout << 1;
            else
                cout << 0;

            j++;
            mask >>= 1;
        }
}
int init_bin_val(int first, int second, int third, int fourth)
{
    unsigned int mask1 = 1;
    unsigned int mask2 = 1;
    int i = 0, answ = 0;
    while (mask2) {

        if(i == 8 || i == 16 || i == 24)
        {
            mask1 = 1;

        }
        if(i < 8)
        {
            if(first & mask1)
            {
                answ ^= mask2;
            }
            mask1 <<= 1;

        }

        if(i < 16 && i >= 8)
        {

            if(second & mask1)
            {
                answ ^= mask2;
            }
            mask1 <<= 1;
        }

        if(i < 24 && i >= 16)
        {

            if(third & mask1)
            {
                answ ^= mask2;
            }
            mask1 <<= 1;
        }

        if(i < 32 && i >= 24)
        {

            if(fourth & mask1)
            {
                answ ^= mask2;
            }
            mask1 <<= 1;
        }


        mask2 <<= 1;
        i++;
    }
    return answ;
}
int get_val(int num, count_val val)
{
    unsigned int mask1 = 1;
    int answ = 0;
    num >>= val;
    int k = val;
    while (k < val + 8)
    {
        if(num & mask1)
        {
            answ += mask1;
        }
        mask1 <<= 1;
        k++;
    }
    return answ;

}


int main(int argc, char *argv[])
{

    int answ = 0;
    int answ1 = 0;

       answ = init_bin_val(255,25,35,155);
       answ1 = init_bin_val(2,4,8,16);
       show_bin(answ);
       cout << endl;
       assert(get_val(answ, FIRST) == 255);
       assert(get_val(answ, SECOND) == 25);
       assert(get_val(answ, THIRD) == 35);
       assert(get_val(answ, FOURTH) == 155);


        cout << "Assert has been successful!" << endl;
       cout << get_val(answ, FIRST) << " => FIRST" << endl;
       cout << get_val(answ, SECOND) << " => SECOND" << endl;
       cout << get_val(answ, THIRD) << " => THIRD" << endl;
       cout << get_val(answ, FOURTH) << " => FOURTH" << endl;





    return 0;
}
