#include "com.h"
#include "../bare/bare.h"
#include "../string/string.h"

int clear(int argv, char *argc[]) {
    if (argv < 1) {
        return 0;
    }
    char *video = (char *) 0xB8000;
    int row = atoi(argc[0]);
    int color = atoi(argc[1]);

    vga_clearrow(row, color, video);
}

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
    for(size_t i = 0; i < mSize; i++) {
        if(mBuff[i] == ' ') {
            i++;
            while(i < mSize && mBuff[i] != ' ') {
                argbuff[argv][charIdx] = mBuff[i];
                i++;
                charIdx++;
            }
            argbuff[argv][charIdx] = '\0';
            argc[argv] = argbuff[argv];
            argv++;
            charIdx = 0;
        }
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
