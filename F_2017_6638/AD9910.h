/*
 * AD9910.h
 *
 *  Created on: 2023年4月29日
 *      Author: LiJingFang
 */

#ifndef AD9910_H_
#define AD9910_H_

#include <msp430.h>
//#include "define.h"
#include <math.h>

typedef unsigned int u16;
typedef unsigned long u32;
typedef unsigned short u8;
typedef unsigned long long u64;

#define DELAY_NUM_US 12
/*
 * 除SCLK、CS、IOUP、SDIO、IORESET 5个接口外，AD9910的部分管脚需置高/低
 * 置高电平：OSK、TxEN
 * 置低电平：PF0 PF1 PF2  F1 F2 DPH DRC DRO PWR
 */

#define SCLK_INIT   P5DIR|=BIT2
#define SCLK_SET    P5OUT|=BIT2
#define SCLK_RESET  P5OUT&=~BIT2

#define CS_INIT     P5DIR|=BIT3
#define CS_SET      P5OUT|=BIT3
#define CS_RESET    P5OUT&=~BIT3

#define IOUP_INIT   P1DIR|=BIT3
#define IOUP_SET    P1OUT|=BIT3
#define IOUP_RESET  P1OUT&=~BIT3

#define SDIO_INIT   P5DIR|=BIT5
#define SDIO_SET    P5OUT|=BIT5
#define SDIO_RESET  P5OUT&=~BIT5

#define IORESET_INIT P1DIR|=BIT6
#define IORESET_SET P1OUT|=BIT6
#define IORESET_RESET P1OUT&=~BIT6


#define CFR1_W_ADDR 0x00
#define CFR2_W_ADDR 0x01
#define CFR3_W_ADDR 0x02
#define DAC_CTL_W_ADDR 0x03
#define RAM_PF0_W_ADDR 0x0E

#define CFR1_COMMAND 0x00400000
#define CFR2_COMMAND 0x01000000
//#define CFR3_COMMAND 0x050F4132
#define CFR3_COMMAND 0x050F4132
#define DAC_CTL_COMMAND 0x0000007F

/*--------------------------------------*/
#define OSK_INIT P7DIR|=BIT5
#define OSK_SET P7OUT|=BIT5

#define TEN_INIT P1DIR|=BIT4
#define TEN_SET P1OUT|=BIT4

#define PF0_INIT P7DIR|=BIT4
#define PF0_RESET P7OUT&=~BIT4

#define PF1_INIT P1DIR|=BIT5
#define PF1_RESET P1OUT&=~BIT5

#define PF2_INIT P1DIR|=BIT2
#define PF2_RESET P1OUT&=~BIT2

#define F1_INIT P3DIR|=BIT0
#define F1_RESET P3OUT&=~BIT0

//这里F0接到了COM引脚，不用自己给低电平

#define DPH_INIT P5DIR|=BIT4
#define DPH_RESET P5OUT&=~BIT4

#define DRC_INIT P1DIR|=BIT1
#define DRC_RESET P1OUT&=~BIT1

#define DRO_INIT P1DIR|=BIT0
#define DRO_RESET P1OUT&=~BIT0

#define PWR_INIT P1DIR|=BIT7
#define PWR_RESET P1OUT&=~BIT7

/*--------------------------------------*/

void AD9910_Init();
void AD9910_Init_F6638();
void AD9910_SetFreAndAmp(double fre, double amp);
void AD9910_SetFreAndAmpOnly(double fre, double amp);

#endif /* AD9910_H_ */
