#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <io.h>
#include <iomanip>
#include <conio.h>
#include <cmath>

#define MAX_NUM     101
#define MAX_EPI     10001
#define PASS_WORD   "123456"
#define LISTNAME    "List"
#define DIR         "/Movies/"

using namespace std;

struct Anime
{
    string name;
    int times;
};

struct tmpAnime
{
    string name;
    int times, number;
};

bool initList(int *num, Anime *list, int *maxLen);
void outList(int *num, Anime *list, int *maxLen, int mode);
bool listFiles(string dir, int *num, Anime *list, int *maxLen);
bool readList(int *num, Anime *list);
void chooseList(int *num, Anime *list, bool *flg, int *maxLen);
void openFiles(string dir, Anime *list, int i);
void writeList(int *num, Anime *list);

inline int findItem(string item, int *num, Anime *list);

inline bool readPassWord(void);
inline string getPassword(void);
inline bool comparePassWord(string *passWord);

inline bool isChar(string str);
inline bool isMovie(char *str_c);

inline void warning(string str);
inline void warning(int num);

inline void drawLine(int len);

inline void greenOut(tmpAnime *List, int i, int len);
inline void normalOut(tmpAnime *List, int i, int len);
