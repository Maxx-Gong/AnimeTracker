#include "main.h"

bool initList(int *num, Anime *list, int *maxLen) // init list
{
    *num = 1;
    // read list and read code
    return listFiles(DIR, num, list, maxLen) && readList(num, list) && readPassWord();
}

bool readList(int *num, Anime *list) // read list
{
    ifstream fin;
    string tmp, name;
    int n;

    fin.open(LISTNAME, ios::in);

    if (fin.is_open() == false)
    {
        warning("Can't open list");
        return false;
    }

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

    fin.close();

    return true;
}

void writeList(int *num, Anime *list) // write list
{
    ofstream fout;

    fout.open(LISTNAME, ios::out);

    if (fout.is_open() == false)
    {
        warning("Can't open list");
        return;
    }

    for (register int i = 1; i <= *num; i++)
    {
        fout << list[i].name << '|' << list[i].times << endl;
    }

    fout.close();
}

bool readPassWord(void)
{
    string passWord;

    cout << "Hi! Your password" << endl
         << "> ";

    passWord = getPassword();
    cout << endl;

    // code comparsion
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

void outList(int *num, Anime *list, int *maxLen) // output list
{
    // if output
    // if (ifOutput == false)
    // {
    //     cout << "QUIT" << endl;
    //     return;
    // }

    // output
    for (int i = 1; i <= *num; i += 2)
    {
        drawLine(*maxLen);

        cout << '|';

        if (list[i].times > 0)
        {
            greenOut(list, i, *maxLen);
        }
        else
        {
            normalOut(list, i, *maxLen);
        }
        if (list[i + 1].times > 0)
        {
            greenOut(list, i + 1, *maxLen);
        }
        else
        {
            normalOut(list, i + 1, *maxLen);
        }

        cout << endl;
    }
    drawLine(*maxLen);
}

void chooseList(int *num, Anime *list, bool *flg, int *maxLen) // choose list
{
    int chooseNum;
    string chooseChar, chooseTimes;
    // chooseChar is the input of which anime to choose
    // chooseNum is to link number and times
    // chooseTimes is the input of what episons the anime had been seen

    cout << "Choose which anime to watch"
         << "[" << *num << "]" << endl
         << "> ";
    cin >> chooseChar;

    if (chooseChar == "q")
    {
        writeList(num, list);
        *flg = false;
        return;
    }
    else if (chooseChar == "l")
    {
        outList(num, list, maxLen);
    }
    else if (!isChar(chooseChar))
    {
        warning("Unexpected input");
    }
    else
    {
        chooseNum = atoi(chooseChar.c_str());

        // above is to choose number

        if (chooseNum > *num)
        {
            warning("Unexpected input");
        }
        else
        {
            openFiles(DIR, list, chooseNum);

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

            cout << "Updating history" << endl
                 << "> ";
            cin >> chooseTimes;
            if (chooseTimes == "q")
            {
                writeList(num, list);
                *flg = false;
                return;
            }
            else if (chooseTimes == "f")
            {
                list[chooseNum].times = MAX_EPI;
            }
            else if (!isChar(chooseTimes))
            {
                warning("Unexpected input");
            }
            else
            {
                list[chooseNum].times = atoi(chooseTimes.c_str());
            }

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
            // above is to choose episons

            // refreshList();
        }
    }
}

bool listFiles(string dir, int *num, Anime *list, int *maxLen)
{
    intptr_t handle;
    _finddata_t findData;

    dir += "*.*";

    // open file handle
    handle = _findfirst(dir.c_str(), &findData);
    if (handle == -1)
    {
        warning("Failed to find first file");
        return false;
    }

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

    *num -= 1; // correct number

    _findclose(handle);

    return true;
}

void openFiles(string dir, Anime *list, int i)
{
    intptr_t handle;
    _finddata_t findData;

    int epi = 0;
    string command, diskDir;
    string playlist[MAX_EPI];

    diskDir = dir + list[i].name + "/*.*";

    // open file handle
    handle = _findfirst(diskDir.c_str(), &findData);
    if (handle == -1)
    {
        warning("Failed to find first file");
        return;
    }

    do
    {
        if (!(findData.attrib & _A_SUBDIR) && isMovie(findData.name))
        {
            epi++;
            playlist[epi] = findData.name;

        }
    } while (_findnext(handle, &findData) == 0);

    if(list[i].times == 0)
    {
        epi = 1;
    }
    else
    {
        epi = list[i].times;
    }

    command = "mpv --no-terminal \"" + dir + list[i].name + '/' + playlist[epi] + "\"";

    system(command.c_str());
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
        // cout << "debug:" << ret << endl;
        ch = _getch();
    }

    return ret;
}

inline bool comparePassWord(string *passWord) // code comparision
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

inline bool isChar(string str) // distinguish number and char
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
