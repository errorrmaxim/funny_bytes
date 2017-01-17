#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <unistd.h>

using namespace std;

string crypto(string str, int key = 13)
{

    string crypto_out;

    for(auto i = 0; i < str.length(); i++)
    {
       crypto_out.push_back(str[i] ^ key);

    }
    return crypto_out;
}
template<typename T>
ostream& operator<< (ostream& os, const vector<T>& rhs)
{

    for (const T& elem : rhs)
    {
        os << elem << endl;
    }
    return os;
}





int main(int argc, char *argv[])
{

    char buff[512];

    string path;
    string name_file;

    vector<string> buffer;
    vector<string> cryp_buff;
    ifstream fot;
    cout << "Enter path to file: ";
    cin >> path;
    cout << endl;

    fot.open(path);
        if(!fot)
        {
            cout << path << " : No such file or directory" << endl;
            return 1;
        }


          while (fot)
            {
            fot.getline(buff, 512);
            buffer.push_back(buff);
            }

    fot.close();


//home/pasquda/Downloads/lab.txt


        for(auto i = 0; i < buffer.size(); i++)
        {
            cryp_buff.push_back(crypto(buffer[i]) + '\n');

        }

        cout << "File: " << path << "\nHas been encoded succsessful!" << endl << endl << "=============================" << endl << endl;
        cout << "Enter path and name of TXT file: ";
        cin >> name_file;
    ofstream inp(name_file);
    if(!inp)
    {
        cout << name_file << " : No such file or directory" << endl;
        return 1;
    }
        for(auto it = cryp_buff.begin(); it != cryp_buff.end(); it++)
        {
            inp << *it;
        }

    system("reset");
    cout << "Succsessful!" << endl;
    inp.close();

    return 0;
}

