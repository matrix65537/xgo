#include "log.h"
#include "util.h"

#ifdef __DEBUG__

extern void test_log();
extern void test_util();
int main(int argc, char** argv)
{
    test_log();
    //test_util();
    return 0;
}

#endif
