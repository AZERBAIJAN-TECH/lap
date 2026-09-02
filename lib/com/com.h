#ifndef COM_H
#define COM_H

typedef int (*func) (int argv, char *argc[]);

struct com {
    const char *name;
    func function;
};

void tryParseCommand(const char *mBuff);
#endif
