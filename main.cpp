#include "app_init.hpp"

int main(int argc, char *argv[])
{
    if (app_run(argc, argv))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}