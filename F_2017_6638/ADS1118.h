/*
 * ADS1118.h
 *
 *  Created on: 2023Äê8ÔÂ2ÈÕ
 *      Author: 86181
 */

#ifndef ADS1118_H_
#define ADS1118_H_

#include <msp430.h>
#include <math.h>

extern long int data_read;

#define CLK_INIT           P6DIR|=BIT7
#define CLK_SET            P6OUT|=BIT7
#define CLK_RESET          P6OUT&=~BIT7

#define CS_INIT            P5DIR|=BIT0
#define CS_SET             P5OUT|=BIT0
#define CS_RESET           P5OUT&=~BIT0

#define SDI_INIT           P6DIR|=BIT6
#define SDI_SET            P6OUT|=BIT6
#define SDI_RESET          P6OUT&=~BIT6

#define SDO_INIT           P6DIR&=~BIT5
#define SDO_REG            P6REN|=BIT5
#define SDO_READ           P6OUT|=BIT5




#endif /* ADS1118_H_ */
