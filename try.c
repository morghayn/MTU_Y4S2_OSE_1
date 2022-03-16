#include "types.h"
#include "stat.h"
#include "user.h"

int main()
{
    trace(1);
    printf(1, "Tracing on\n");
    trace(0);
    printf(1, "Tracing off\n");
    exit();
}
