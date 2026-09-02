#include "com.h"
#include "../bare/bare.h"
#include "../string/string.h"
#include "../../cmd/cmds.h"

struct pCom list[10] = { { .funcType = INT, .funcData.intfunc = {"clear", clear}} };

void tryParseCommand(const char *mBuff) {
    char sBuff[256];
    size_t mSize = strlen(mBuff);
    int argv = 0;
    char argbuff[16][256];
    char *argc[16];

    int charIdx = 0;

    //parsing mBuff to get name
    size_t i;
    for(i = 0; i < mSize; i++) {
        if(mBuff[i] == ' ') {
            break;
        }
        sBuff[i] = mBuff[i];
    }
    sBuff[i] = '\0';

    //argv
    size_t j = 1;
    while (j < mSize) {
        if (mBuff[j] == ' ') {
            j++;
            continue;
        }
        while (j < mSize && mBuff[j] != ' ') {
            argbuff[argv][charIdx] = mBuff[j];
            charIdx++;
            j++;
        }
        argbuff[argv][charIdx] = '\0';
        argc[argv] = argbuff[argv];
        argv++;
        charIdx = 0;
    }

    //paring name of funcion and call it
    for(int i = 0; i < 10; i++) {
        if (list[i].funcType == INT) {
            if(strcmp(list[i].funcData.intfunc.name, sBuff) == 0) {
                list[i].funcData.intfunc.func(argv, argc);
            }
        }
    }
}
