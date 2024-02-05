#include "main.h"

bool flg;
int num, maxLen;
Anime list[MAX_NUM];

int main()
{
    // init and keeping login
    if (initList(&num, list, &maxLen))
    {
        flg = true;
    }
    else
    {
        flg = false;
    }

    // output
    if (flg)
    {
        outList(&num, list, &maxLen, 0);
    }

    // choose list
    while (flg)
    {
        chooseList(&num, list, &flg, &maxLen);
    }

    return 0;
}
