#include "main.h"

bool flg;
int num, maxLen;
Anime list[MAX_NUM];

int main()
{
    if (initList(&num, list, &maxLen))
    {
        flg = true;
    }
    else
    {
        flg = false;
    }
    // init and keeping login

    if (flg)
    {
        outList(&num, list, &maxLen); // output
    }

    while (flg)
    {
        chooseList(&num, list, &flg, &maxLen); // choose list
    }

    return 0;
}
