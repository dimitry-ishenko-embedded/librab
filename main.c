#include "board.h"
#include "ivt.h"
#include "reg.h"
#include "serial.h"
#include "types.h"

byte __sdcc_external_startup()
{
    board_init();
    return 0;
}

int main()
{
    sera_open(9600);
    sera_puts("Hello world!\n");

    return 0;
}
