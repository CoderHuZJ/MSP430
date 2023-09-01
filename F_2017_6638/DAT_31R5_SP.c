/*
 * DAT_31R5_SP.c
 *
 *  Created on: 2023年3月31日
 *      Author: ZHOU
 */

#include "DAT_31R5_SP.h"
#include <msp430f6638.h>

#define SET_CLK     P4OUT|=BIT4;    P4OUT|=BIT1;
#define CLR_CLK     P4OUT&=~BIT4;   P4OUT&=~BIT1;

#define SET_DAT     P4OUT|=BIT5;    P4OUT|=BIT2;
#define CLR_DAT     P4OUT&=~BIT5;   P4OUT&=~BIT2;

#define SET_LE      P4OUT|=BIT3;    P4OUT|=BIT7;
#define CLR_LE      P4OUT&=~BIT3;   P4OUT&=~BIT7;

//将衰减系数(单位dB，步进0.5dB)转换为寄存器的值
uint8_t get_reg(float att)
{
    uint8_t ret_val = (uint8_t)(att * 2.0);
    return ret_val;
}

//初始化
void dat_31r5_sp_init()
{
    P4DIR |= BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT7;

    P3DIR |= BIT7;
    P3OUT &=~BIT7;

    CLR_LE;
    CLR_CLK;
    CLR_DAT;
}
//写寄存器
void write_reg(uint8_t data)
{
    CLR_LE;
    CLR_CLK;
    CLR_DAT;

    int j = 0;
    for(j = 0; j < 8; j++)
    {
        if(0x80 == (data & 0x80))
        {
            SET_DAT;
        }
        else
        {
            CLR_DAT;
        }
        __delay_cycles(100);
        SET_CLK;
        __delay_cycles(100);
        data <<= 1;
        CLR_CLK;
    }
    CLR_DAT;
    __delay_cycles(100);
    SET_LE;
    __delay_cycles(100);
    CLR_LE;
}
//设置衰减系数
void set_att(uint8_t att)
{
    write_reg(get_reg(att/2));
    return;
}
