#include "main.h"

// init list
bool initList(int *num, Anime *list, int *maxLen)
{
    *num = 1;
    // read folder, list and password
    return listFiles(DIR, num, list, maxLen) && readList(num, list) && readPassWord();
}

// read list
bool readList(int *num, Anime *list)
{
    ifstream fin;
    string tmp, name;
    int n;

    // open list
    fin.open(LISTNAME, ios::in);

    if (fin.is_open() == false)
    {
        warning("Can't open list");
        return false;
    }

    // compare to reading folder
    while (getline(fin, tmp))
    {
        name.clear();
        for (register int i = 0; i < tmp.size(); i++)
        {
            if (tmp[i] == '|')
            {
                n = findItem(name, num, list);
                list[n].times = atoi(tmp.substr(i + 1).c_str());
                break;
            }
            else
            {
                name += tmp[i];
            }
        }
    }

    // close list
    fin.close();

    return true;
}

// write list
void writeList(int *num, Anime *list)
{
    ofstream fout;

    // open list
    fout.open(LISTNAME, ios::out);

    if (fout.is_open() == false)
    {
        warning("Can't open list");
        return;
    }

    // write list
    for (register int i = 1; i <= *num; i++)
    {
        fout << list[i].name << '|' << list[i].times << endl;
    }

    // close list
    fout.close();
}

bool readPassWord(void)
{
    string passWord;

    // get password
    cout << "Hi! Your password" << endl
         << "> ";

    passWord = getPassword();
    cout << endl;

    // password comparsion
    if (comparePassWord(&passWord))
    {
        return true;
    }
    else
    {
        warning("Wrong password");
        return false;
    }
}

// out list
void outList(int *num, Anime *list, int *maxLen, int mode)
{
    // if output
    // if (ifOutput == false)
    // {
    //     cout << "QUIT" << endl;
    //     return;
    // }

    // out lsit
    drawLine(*maxLen);
    switch (mode)
    {
    case 0:
        for (int i = 1; i <= *num; i++)
        {
            bool isprint = false;

            cout << '|';

            // first row
            while (isprint == false)
            {
                if (i > *num)
                {
                    break;
                }
                else if (list[i].times < MAX_EPI)
                {
                    if (list[i].times > 0)
                    {
                        greenOut(list, i, *maxLen);
                    }
                    else
                    {
                        normalOut(list, i, *maxLen);
                    }
                    isprint = true;
                }
                else
                {
                    i += 1;
                }
            }
            isprint = false;
            i++;
            // second row
            while (isprint == false)
            {
                if (i > *num)
                {
                    break;
                }
                else if (list[i].times < MAX_EPI)
                {
                    if (list[i].times > 0)
                    {
                        greenOut(list, i, *maxLen);
                    }
                    else
                    {
                        normalOut(list, i, *maxLen);
                    }
                    isprint = true;
                }
                else
                {
                    i += 1;
                }
            }
            cout << endl;
            if (i >= *num)
            {
                break;
            }
        }
        break;
    case 1:
        for (int i = 1; i <= *num; i++)
        {
            cout << '|';

            // first row
            if (list[i].times > 0)
            {
                greenOut(list, i, *maxLen);
            }
            else
            {
                normalOut(list, i, *maxLen);
            }
            i++;
            // second row
            if (list[i].times > 0)
            {
                greenOut(list, i, *maxLen);
            }
            else
            {
                normalOut(list, i, *maxLen);
            }

            cout << endl;
        }
        break;
    case 2:
        for (int i = 1; i <= *num; i++)
        {
            bool isprint = false;

            cout << '|';

            // first row
            while (isprint == false)
            {
                if (i > *num)
                {
                    break;
                }
                else if (list[i].times == MAX_EPI)
                {
                    greenOut(list, i, *maxLen);
                    isprint = true;
                }
                else
                {
                    i++;
                }
            }
            isprint = false;
            i++;
            // second row
            while (isprint == false)
            {
                if (i > *num)
                {
                    break;
                }
                else if (list[i].times == MAX_EPI)
                {
                    greenOut(list, i, *maxLen);
                    isprint = true;
                }
                else
                {
                    i++;
                }
            }
            cout << endl;
        }
        break;
    }
    drawLine(*maxLen);
}

// choose list
void chooseList(int *num, Anime *list, bool *flg, int *maxLen)
{
    int chooseNum;
    string chooseChar, chooseTimes;
    // chooseChar is the input of which anime to choose
    // chooseNum is to link number and times
    // chooseTimes is the input of what episons the anime had been seen

    // get anime number
    cout << "Choose which anime to watch"
         << "[" << *num << "]" << endl
         << "> ";
    cin >> chooseChar;

    // input
    if (chooseChar == "q")
    {
        // quit
        writeList(num, list);
        *flg = false;
        return;
    }
    else if (chooseChar == "l")
    {
        // list
        outList(num, list, maxLen, 0);
    }
    else if (chooseChar == "la")
    {
        outList(num, list, maxLen, 1);
    }
    else if (chooseChar == "lf")
    {
        outList(num, list, maxLen, 2);
    }
    else if (!isChar(chooseChar))
    {
        // error
        warning("Unexpected input");
    }
    else
    {
        // switch to number
        chooseNum = atoi(chooseChar.c_str());

        if (chooseNum > *num)
        {
            // error
            warning("Unexpected input");
        }
        else
        {
            // open the crrent epison
            if (list[chooseNum].times == 0)
            {
                list[chooseNum].times = 1;
            }
            openFiles(DIR, list, chooseNum);

            // print crrent epison
            if (list[chooseNum].times >= MAX_EPI)
            {
                cout << "\033[32;1m" << '[' << chooseNum << ']' << list[chooseNum].name << " "
                     << "fin"
                     << "\033[0m" << endl;
            }
            else
            {
                cout << "\033[32;1m" << '[' << chooseNum << ']' << list[chooseNum].name << " " << list[chooseNum].times << "\033[0m" << endl;
            }

            // get update epison
            cout << "Updating history" << endl
                 << "> ";
            cin >> chooseTimes;

            if (chooseTimes == "q")
            {
                // quit
                writeList(num, list);
                *flg = false;
                return;
            }
            else if (chooseTimes == "f")
            {
                // finish
                list[chooseNum].times = MAX_EPI;
            }
            else if (!isChar(chooseTimes))
            {
                // error
                warning("Unexpected input");
            }
            else
            {
                // record
                list[chooseNum].times = atoi(chooseTimes.c_str());
            }

            // print update epison
            if (list[chooseNum].times >= MAX_EPI)
            {
                cout << "\033[102;1m" << '[' << chooseNum << ']' << list[chooseNum].name << " "
                     << "fin"
                     << "\033[0m" << endl;
            }
            else
            {
                cout << "\033[102;1m" << '[' << chooseNum << ']' << list[chooseNum].name << " " << list[chooseNum].times << "\033[0m" << endl;
            }
            // above is to choose and open episons

            // refreshList();
        }
    }
}

// read folder
bool listFiles(string dir, int *num, Anime *list, int *maxLen)
{
    intptr_t handle;
    _finddata_t findData;

    dir += "*.*";

    // open file handle
    handle = _findfirst(dir.c_str(), &findData);
    if (handle == -1)
    {
        warning("Failed to find first movie folders");
        cout << dir << ' ' << "*";
        return false;
    }

    // read folder
    do
    {
        if (findData.attrib & _A_SUBDIR &&
            strcmp(findData.name, ".") != 0 &&
            strcmp(findData.name, "..") != 0)
        {
            // cout << findData.name << ' ' << findItem(findData.name, num, list) << endl;
            list[*num].name = findData.name;
            list[*num].times = 0;
            if (*maxLen < list[*num].name.size())
            {
                *maxLen = list[*num].name.size();
            }
            *num += 1;
        }
    } while (_findnext(handle, &findData) == 0);

    // correct number
    *num -= 1;

    // close handle
    _findclose(handle);

    return true;
}

// open epison
void openFiles(string dir, Anime *list, int i)
{
    intptr_t handle;
    _finddata_t findData;

    int epi = 0;
    string command, diskDir;
    string playlist[MAX_EPI];

    // anime dir
    diskDir = dir + list[i].name + "/*.*";

    // open file handle
    handle = _findfirst(diskDir.c_str(), &findData);
    if (handle == -1)
    {
        warning("Failed to find first movie files");
        return;
    }

    // read epison
    do
    {
        if (!(findData.attrib & _A_SUBDIR) && isMovie(findData.name))
        {
            epi++;
            playlist[epi] = findData.name;
        }
    } while (_findnext(handle, &findData) == 0);

    if (list[i].times == 0)
    {
        epi = 1;
    }
    else
    {
        epi = list[i].times;
    }

    command = "mpv --no-terminal \"" + dir + list[i].name + '/' + playlist[epi] + "\"";

    // execute command
    system(command.c_str());

    // close handle
    _findclose(handle);
}

inline int findItem(string item, int *num, Anime *list)
{
    for (register int i = 1; i <= *num; i++)
    {
        if (item == list[i].name)
        {
            return i;
        }
    }
    return 0;
}

inline string getPassword(void)
{

    string ret;
    char ch;
    ch = _getch();
    while (ch != '\n' && ch != '\r')
    {
        ret += ch;
        ch = _getch();
    }

    return ret;
}

inline bool comparePassWord(string *passWord)
{
    if (*passWord == PASS_WORD)
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool isChar(string str)
{
    for (register int i = 0; i < str.size(); i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            return false;
        }
    }

    return true;
}

inline bool isMovie(char *str_c)
{
    string str = str_c;

    string extend[5] = {"mp4", "mkv", "mov", "flv", "avi"};

    for (register int i = 0; i < 5; i++)
    {
        if (str.find(extend[i]) != string::npos)
            return true;
    }

    return false;
}

inline void warning(string str) // warning
{
    cout << "\033[31;1m"
         << "[Warning]" << str
         << "\033[0m" << endl;
}

inline void warning(int num) // warning
{
    cout << "\033[31;1m"
         << "[Warning]" << num
         << "\033[0m" << endl;
}

inline void drawLine(int len)
{
    cout << '+';
    cout << setfill('-')
         << setw(4 + len + 4 + 1)
         << '-';
    cout << '+';
    cout << setfill('-')
         << setw(4 + len + 4 + 1)
         << '-';
    cout << '+';
    cout << endl;
}

inline void greenOut(Anime *list, int i, int len)
{
    cout << '[' << setfill('0') << setw(2) << i << ']';
    cout << setfill(' ')
         << left
         << setw(len)
         << list[i].name;
    if (list[i].times >= MAX_EPI)
    {
        cout << right
             << "\033[32;1m" << setw(4) << "fin"
             << "\033[0m" << ' ' << '|';
    }
    else
    {
        cout << right
             << "\033[32;1m" << setw(4) << list[i].times << "\033[0m" << ' ' << '|';
    }
}

inline void normalOut(Anime *list, int i, int len)
{
    cout << '[' << setfill('0') << setw(2) << i << ']';
    cout << setfill(' ')
         << left
         << setw(len)
         << list[i].name;
    if (list[i].times >= MAX_EPI)
    {
        cout << right
             << setw(4)
             << "fin" << ' ' << '|';
    }
    else
    {
        cout << right
             << setw(4)
             << list[i].times << ' ' << '|';
    }
}
