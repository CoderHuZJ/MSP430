/*
 * TLV5638.h
 *
 *  Created on: 2023Äê8ÔÂ3ÈÕ
 *      Author: 86181
 */

#ifndef TLV5638_H_
#define TLV5638_H_

#include <msp430.h>
#include <math.h>

#define CLK_INIT     P2DIR|=BIT0
#define CLK_SET      P2OUT|=BIT0
#define CLK_RESET    P2OUT&=~BIT0

#define CS_INIT      P5DIR|=BIT7
#define CS_SET       P5OUT|=BIT7
#define CS_RESET     P5OUT&=~BIT7

#define SDI_INIT     P4DIR|=BIT0
#define SDI_SET      P4OUT|=BIT0
#define SDI_RESET    P4OUT&=~BIT0


#endif /* TLV5638_H_ */
