/*
 * ADS1118.c
 *
 *  Created on: 2023年8月2日
 *      Author: Huzhijie
 */

#include "ADS1118.h"

int control_num_1118[16] = {1,1,0,1,0,1,1,1,1,1,0,0,0,0,1,1};   //ADS1118控制字
extern int data[16]={0};
long int data_read=0;

void ADS1118_Init()
{
    CLK_INIT;
    CS_INIT;
    SDI_INIT;
    CLK_RESET;
    CS_SET;
    SDI_RESET;
}

void ADS1118_write_read()
{
//    while(P6IN&BIT6)
//    {
//
//    }

    CS_RESET;
//    CLK_RESET;

    data_read = 0;

    _delay_cycles(10);

    int i = 0;
    for(i=15;i>=0;i--)
    {
        if(control_num_1118[i])
            SDI_SET;
        else
            SDI_RESET;

        _delay_cycles(18);

        CLK_SET;

        _delay_cycles(12);

        if(P6IN&BIT5)
        {
            data[i] = 1;
            data_read = data_read * 2 + 1;
        }
        else
        {
            data[i] = 0;
            data_read = data_read * 2;
        }

        _delay_cycles(10);

        CLK_RESET;
    }

//    CLK_SET;
    CS_SET;
}


