/*
 * AD9910.c
 *
 *  Created on: 2023年4月29日
 *      Author: LiJingFang
 */
#include "AD9910.h"

void AD9910_GPIO_INIT()
{
    SCLK_INIT;
    SCLK_RESET;

    CS_INIT;
    CS_SET;

    IOUP_INIT;
    IOUP_RESET;

    SDIO_INIT;
    SDIO_RESET;

    IORESET_INIT;
    IORESET_SET;
}

void AD9910_GPIO_INIT_F6638()
{
    OSK_INIT;
    OSK_SET;

    TEN_INIT;
    TEN_SET;

    PF0_INIT;
    PF0_RESET;

    PF1_INIT;
    PF1_RESET;

    PF2_INIT;
    PF2_RESET;

    F1_INIT;
    F1_RESET;

    DPH_INIT;
    DPH_RESET;

    DRC_INIT;
    DRC_RESET;

    DRO_INIT;
    DRO_RESET;

    PWR_INIT;
    PWR_RESET;

    SCLK_INIT;
    SCLK_RESET;

    CS_INIT;
    CS_SET;

    IOUP_INIT;
    IOUP_RESET;

    SDIO_INIT;
    SDIO_RESET;

    IORESET_INIT;
    IORESET_RESET;
}

void AD9910_SendBytes(u8 sendBytes)
{
    u8 i=0;
    u8 bitSel = 0x80;
    u8 sendbit = 0x00;
//    CS_RESET;
//    SCLK_RESET;

    for(i=0;i<8;i++)
    {
        sendbit = sendBytes & bitSel;
        bitSel  = bitSel >> 1;
        if(sendbit)
        {
            SDIO_SET;
        }
        else{
            SDIO_RESET;
        }
//        _delay_cycles(DELAY_NUM_US);
        SCLK_SET;
        _delay_cycles(DELAY_NUM_US);
        SCLK_RESET;
    }

//    CS_SET;
}

void AD9910_Send32Bit(u8 addr,u32 sendDat)
{
    u8 i=0;
    u8 byte2send=0x00;
    u32 byteSel = 0xFF000000;

    IOUP_RESET;
    CS_RESET;
    SCLK_RESET;

//    IORESET_RESET;
    _delay_cycles(4*DELAY_NUM_US);

    AD9910_SendBytes(addr);
    _delay_cycles(2*DELAY_NUM_US);
    for(i=0;i<4;i++)
    {
        byte2send = (sendDat&byteSel) >> (24-8*i);
        byteSel = byteSel >> 8;
        AD9910_SendBytes(byte2send);
    }

    IOUP_SET;
    _delay_cycles(4*DELAY_NUM_US);
    IOUP_RESET;

    CS_SET;
//    IORESET_SET;
    SCLK_RESET;
    SDIO_RESET;
}

void AD9910_Send64Bit(u8 addr,u64 sendDat)
{
    u8 i=0;
    u8 byte2send=0x00;
    u64 byteSel = 0xff00000000000000;

    IOUP_RESET;
    CS_RESET;
    SCLK_RESET;

//    IORESET_RESET;
    _delay_cycles(4*DELAY_NUM_US);

    AD9910_SendBytes(addr);
    _delay_cycles(DELAY_NUM_US);

    for(i=0;i<8;i++)
    {
        byte2send = (sendDat&byteSel) >> (56-8*i);
        byteSel = byteSel >> 8;
        AD9910_SendBytes(byte2send);
    }

    _delay_cycles(10*DELAY_NUM_US);
    IOUP_SET;
    _delay_cycles(4*DELAY_NUM_US);
    IOUP_RESET;

    CS_SET;
//    IORESET_SET;
    SCLK_RESET;
    SDIO_RESET;
}

void AD9910_Init()
{
    AD9910_GPIO_INIT();

    IORESET_SET;
    __delay_cycles(50);
    IORESET_RESET;

    AD9910_Send32Bit(CFR1_W_ADDR,CFR1_COMMAND);
    AD9910_Send32Bit(CFR2_W_ADDR,CFR2_COMMAND);
    AD9910_Send32Bit(CFR3_W_ADDR,CFR3_COMMAND);
    AD9910_Send32Bit(DAC_CTL_W_ADDR,DAC_CTL_COMMAND);
}



void AD9910_SetFreAndAmp(double fre, double amp)
{
    u32 FTW = 0;
    u16 AM = 0;
    u64 RAM_64Bit = 0x00;
//    FTW = (u32)(fre*4.294967296);     //标准系统时钟1GHz

    FTW = (u32)(fre*4.294803402);       //修正 系统时钟 1000038161Hz
//    FTW = (u32)(fre*(pow(2,32)/1000038000));   //修正系统时钟2  1GHz+38kHz

    if(amp<0)
    {
        amp = 0;
    }else if(amp>=1)
    {
        amp = 0.9999;
    }
    AM = (u16)round(amp * 0x4000);

    RAM_64Bit = ((u64)(AM&0x3FFF)<<48) | FTW;

    AD9910_Send32Bit(CFR1_W_ADDR,CFR1_COMMAND);
    _delay_cycles(100*DELAY_NUM_US);

    AD9910_Send32Bit(CFR2_W_ADDR,CFR2_COMMAND);
    _delay_cycles(100*DELAY_NUM_US);

    AD9910_Send32Bit(CFR3_W_ADDR,CFR3_COMMAND);
    _delay_cycles(100*DELAY_NUM_US);

    AD9910_Send32Bit(DAC_CTL_W_ADDR,DAC_CTL_COMMAND);
    _delay_cycles(100*DELAY_NUM_US);


    AD9910_Send64Bit(RAM_PF0_W_ADDR,RAM_64Bit);
    _delay_cycles(100*DELAY_NUM_US);

}

void AD9910_SetFreAndAmpOnly(double fre, double amp)
{
    u32 FTW = 0;
    u16 AM = 0;
    u64 RAM_64Bit = 0x00;
    FTW = (u32)(fre*4.294803402);       //修正 系统时钟 1000038161Hz
//    FTW = (u32)(fre*(pow(2,32)/1000038000));   //修正系统时钟2  1GHz+38kHz

    if(amp<0)
    {
        amp = 0;
    }else if(amp>=1)
    {
        amp = 0.9999;
    }
    AM = (u16)round(amp * 0x4000);

    RAM_64Bit = ((u64)(AM&0x3FFF)<<48) | FTW;

    AD9910_Send64Bit(RAM_PF0_W_ADDR,RAM_64Bit);
}

void AD9910_Init_F6638()
{
    AD9910_GPIO_INIT_F6638();

    IORESET_SET;
    __delay_cycles(50);
    IORESET_RESET;

    AD9910_Send32Bit(CFR1_W_ADDR,CFR1_COMMAND);
    AD9910_Send32Bit(CFR2_W_ADDR,CFR2_COMMAND);
    AD9910_Send32Bit(CFR3_W_ADDR,CFR3_COMMAND);
    AD9910_Send32Bit(DAC_CTL_W_ADDR,DAC_CTL_COMMAND);
}
