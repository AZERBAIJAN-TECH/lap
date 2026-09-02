#include "echo.h"
#include "../../lib/print/print.h"

int echo(int argv, char *argc[]) {
    if (argv < 1) {
        return 1;
    }
    print(argc[0]);
    return 0;
}
