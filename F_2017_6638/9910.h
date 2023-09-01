    /*
 * AD9910.h
 *
 *  Created on: 2023年3月31日
 *      Author: ZHOU
 */

#ifndef AD9910_H_
#define AD9910_H_


 /*-----------------------------------------------
  名称：AD9854并口驱动
  编写：Liu
  日期：2014.3
  修改：无
  内容：
------------------------------------------------*/
#ifndef __AD9910_H__
#define __AD9910_H__

#include <msp430f6638.h>
#include <stdint.h>

#define uchar unsigned char
#define uint  unsigned int
#define ulong  unsigned long int

//#define AD9910_PWR PCout(13)//0
////原始IO
//#define AD9910_SDIO PAout(5)
//#define DRHOLD PCout(1)
//#define DROVER PCout(2)
//#define UP_DAT PCout(3)
//#define PROFILE1 PCout(10)
//#define MAS_REST PAout(6)
//#define SCLK PAout(2)
//#define DRCTL  PAout(4)
//#define OSK PCout(8)
//#define PROFILE0 PCout(4)
//#define PROFILE2 PCout(5)
//#define CS  PBout(10)

//P2 BIT3 BIT4 BIT5
//P3 BIT0
//P5 BIT6
//P6 BIT1 BIT2 BIT3 BIT4 BIT5 BIT6
//P8 BIT5 BIT6

#define SCLK_INIT   P5IR=BIT2
#define SCLK_SET    P5OUT |=BIT2
#define SCLK_RESET  P5OUT &=~BIT2


#define CSB_INIT     P5IR=BIT3
#define CSB_SET      P5OUT |=BIT3
#define CSB_RESET    P5OUT &=~BIT3


#define SDIO_INIT     P5IR=BIT5
#define SDIO_SET      P5OUT |=BIT5
#define SDIO_RESET    P5OUT &=~BIT5


#define IOUP_INIT     P1IR=BIT3
#define IOUP_SET      P1OUT |=BIT3
#define IOUP_RESET    P1OUT &=~BIT3


#define RST_INIT     P1IR=BIT6
#define RST_SET      P1OUT |=BIT6
#define RST_RESET    P1OUT &=~BIT6

#define PWR_INIT     P1IR=BIT7
#define PWR_SET      P1OUT |=BIT7
#define PWR_RESET    P1OUT &=~BIT7

void AD9110_IOInit(void);
void Init_ad9910(void);
void Freq_convert(ulong Freq);                                      //写频率
void AD9910_AmpWrite(uint16_t Amp);
void AD9910_write(ulong Freq,uint16_t Amp);
#endif





#endif /* AD9910_H_ */
