/*
 * TLV5638.c
 *
 *  Created on: 2023Äê8ÔÂ3ÈÕ
 *      Author: Huzhijie
 */

#include "TLV5638.h"

int control_num_5638[16] = {1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1};
int vol_data[16] = {0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0};

//int control_num_5638[16] = {1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1};
//int vol_data[16] = {0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0};

void TLV5638_Init()
{
    CLK_INIT;
    CS_INIT;
    SDI_INIT;
    CLK_RESET;
    CS_SET;
    SDI_RESET;
}

void TLV5638_write()
{
    CS_RESET;
    _delay_cycles(1000);

    int i = 0;
    for(i=15;i>=0;i--)
    {
        if(control_num_5638[i])
            SDI_SET;
        else
            SDI_RESET;

        _delay_cycles(1800);

        CLK_SET;

        _delay_cycles(1200);

        CLK_RESET;
    }
    _delay_cycles(1000);
    CLK_SET;
    _delay_cycles(1000);
    CLK_RESET;
    _delay_cycles(1000);
    CS_SET;
    _delay_cycles(1000);

    CS_RESET;
    _delay_cycles(1000);

    for(i=15;i>=0;i--)
    {
        if(vol_data[i])
            SDI_SET;
        else
            SDI_RESET;

        _delay_cycles(1800);

        CLK_SET;

        _delay_cycles(1200);

        CLK_RESET;
    }
    _delay_cycles(1000);
    CLK_SET;
    _delay_cycles(1000);
    CLK_RESET;
    _delay_cycles(1000);
    CS_SET;
    _delay_cycles(1000);
}


