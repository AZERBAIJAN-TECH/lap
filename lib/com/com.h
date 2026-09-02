#ifndef COM_H
#define COM_H

enum type {VOID, INT};

typedef int (*intFunc) (int argv, char *argc[]);

union com {
    struct {
        const char *name;
        void (*func)();    
    } voidfunc;
    struct {
        const char *name;
        intFunc func;
    } intfunc;
};

struct pCom {
    enum type funcType;
    union com funcData;
};

void parse_command(const char *mBuff);
#endif
