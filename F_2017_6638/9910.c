///*
// * 9910.c
// *
// *  Created on: 2023年6月24日
// *      Author: LENOVO
// */
//
//
///*
// * AD9910.c
// *
// *  Created on: 2023年3月31日
// *      Author: ZHOU
// */
//
//
///**********************************************************
//                       康威电子
//
//功能：stm32f103rct6控制
//            显示：12864
//接口：按键接口请参照key.h
//时间：2015/11/3
//版本：1.0
//作者：康威电子
//其他：
//
//更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
//https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ
//
//**********************************************************/
//#include "9910.h"
//
//const uchar cfr1[]={0x00,0x40,0x00,0x00};       //cfr1控制字
//const uchar cfr2[]={0x01,0x00,0x00,0x00};       //cfr2控制字
////const uchar cfr3[]={0x05,0x0F,0x41,0x32};       //cfr3控制字  40M输入  25倍频  VC0=101   ICP=001;
//const uchar cfr3[]={0x05,0x0F,0x41,0xA0};       //cfr3控制字  40M输入  25倍频  VC0=101   ICP=001;
//uchar profile11[]={0x3f,0xff,0x00,0x00,0x25,0x09,0x7b,0x42};       //profile1控制字 0x25,0x09,0x7b,0x42
//                  //01振幅控制 23相位控制 4567频率调谐字
//void AD9110_IOInit(void)
//{
//    SCLK_INIT;
//    SCLK_RESET;
//
//    CSB_INIT;
//    CSB_RESET;
//
//    SDIO_INIT;
//    SDIO_RESET;
//
//    IOUP_INIT;
//    IOUP_RESET;
//
//    RST_INIT;
//    RST_RESET;
//
//    PWR_INIT;
//    PWR_RESET;
//
//}
//
////=====================================================================
//
//
////======================发送8位数据程序===================================
//void txd_8bit(uchar txdat)
//{
//    uchar i, sbt;
//    sbt=0x80;
///
//
//
//    for (i=0;i<8;i++)
//    {
//        sendbit = sbt & bitSel;
//        bitSel  = bitSel >> 1;
//        if (sendbit) DIO_SET;; else SDIO_RESET;
////        SCLK=1;
//
//        SCLK_SET;
//        _delay_cycles(100);
//        SCLK_RESET;
//
//    }
//}
//
//
///************************************************************
//** 函数名称 ：void Init_ad9910(void))
//** 函数功能 ：初始化AD9910的管脚和最简单的内部寄存器的配置，
//** 入口参数 ：无
//** 出口参数 ：无
//** 函数说明 ：无
//**************************************************************/
//void Init_ad9910(void)
//{
//    uchar k,m;
//
//    AD9110_IOInit();//IO初始化
////    AD9910_PWR = 0;//软件拉低
//    RST_SET  ;
//       __delay_cycles(50);
//    RST_RESET  ;
//
//
//    IOUP_RESET;
//    CSB_RESET;
//    SCLK_RESET;
//
//    _delay_cycles(10);
//    txd_8bit(0x00); //发送CFR1控制字地址
//    for (m=0;m<4;m++)
//    {
//
//        txd_8bit(cfr1[m]);
//
//    }
//
//    IOUP_SET;
//     _delay_cycles(4*10);
//    IOUP_RESET;
//
//     CSB_SET;
//    //    IORESET_SET;
//     SCLK_RESET;
//     SDIO_RESET;
//     SET_CS;
//    for (k=0;k<10;k++);
//
////    CS=0;
//    CLR_CS;
//    txd_8bit(0x01);    //发送CFR2控制字地址
//    for (m=0;m<4;m++)
//        txd_8bit(cfr2[m]);
////    CS=1;
//
//    SET_CS;
//    for (k=0;k<10;k++);
//
////    CS=0;
//
//    CLR_CS;
//    txd_8bit(0x02);    //发送CFR3控制字地址
//    for (m=0;m<4;m++)
//        txd_8bit(cfr3[m]);
////    CS=1;
//
//    SET_CS;
//    for (k=0;k<10;k++);
//
////    UP_DAT=1;
//    SET_IOUP;
//    for(k=0;k<10;k++);
////    UP_DAT=0;
//
//    CLR_IOUP;
//    __delay_cycles(12);
//
//}
///************************************************************
//** 函数名称 ：void Txfrc(void))
//** 函数功能 ：向AD9910芯片发送频率，幅度等相关控制数据
//** 入口参数 ：无
//** 出口参数 ：无
//** 函数说明 ：无
//**************************************************************/
//void Txfrc(void)
//{
//    uchar m;
//
////    CS=0;
//    CLR_CS;
//    txd_8bit(0x0e);    //发送profile1控制字地址
//    for (m=0;m<8;m++)
//    txd_8bit(profile11[m]);
////    CS=1;
//    SET_CS;
//
////    UP_DAT=1;
//    SET_IOUP;
////    UP_DAT=0;
//    CLR_IOUP;
//}
///************************************************************
//** 函数名称 ：void Freq_convert(void))
//** 函数功能 ：将需要的频率转换为对应的控制数据，保存进profile11并发送到芯片
//** 入口参数 ：目标频率，单位Hz
//** 出口参数 ：无
//** 函数说明 ：无
//**************************************************************/
//void Freq_convert(ulong Freq)
//{
//    ulong Temp;
//    Temp=(ulong)Freq*4.294967296;    //将输入频率因子分为四个字节  主频1GHz，32位相位累加器，故每Hz在的控制字增量 delta =  4.294967296 = (2^32)/1000000000
//    profile11[7]=(uchar)Temp;
//    profile11[6]=(uchar)(Temp>>8);
//    profile11[5]=(uchar)(Temp>>16);
//    profile11[4]=(uchar)(Temp>>24);
//    Txfrc();
//}
//
///************************************************************
//** 函数名称 ：void AD9910_AmpWrite(void))
//** 函数功能 ：将幅度控制数据保存到profile11并写入芯片
//** 入口参数 ：幅度控制字，范围0~16383
//** 出口参数 ：无
//** 函数说明 ：14位幅度控制字，控制数据0~16383对应输出幅度0~800mV左右
//**************************************************************/
//void AD9910_AmpWrite(uint16_t Amp)
//{
//    profile11[0] = (Amp%16384)>>8;
//    profile11[1] = (Amp%16384)&0xff;
//    Txfrc();
//}
//void AD9910_write(ulong Freq,uint16_t Amp)
//{
//    Freq_convert(Freq);
//    AD9910_AmpWrite(Amp);
//}
//
//
//
//
