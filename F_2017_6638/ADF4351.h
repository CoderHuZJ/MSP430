/*
 * ADF4351.h
 *
 *  Created on: 2023��3��26��
 *      Author: ZHOU
 */

#ifndef ADF4351_H_
#define ADF4351_H_

#include <stdint.h>
#include <stdbool.h>

#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

/******************************variable****************************/
//u32 reg[6];
/******************************fucntion****************************/
uint32_t Get_Reg0(uint16_t INT,uint16_t FRAC);
uint32_t Get_Reg1(uint8_t PRESCALER,uint16_t MOD);
uint32_t Get_Reg2(bool DOUBLE,bool RDIV,uint16_t R);
uint32_t Get_Reg3(uint16_t CLK_DIV);
uint32_t Get_Reg4(uint8_t FEED_BACK,uint8_t RF_DIV,uint8_t CLK_DIV);
uint32_t Get_Reg5(uint8_t LD_MOD);
void Write_Reg(uint32_t data);
void init_adf4351(int INT1,int FRAC1,int div);
void sao_adf4351(int INT, int FACE);
void set_ADF4351_fre(double fre);
void test2();


//��˥��ϵ��(��λdB������0.5dB)ת��Ϊ�Ĵ�����ֵ
inline uint8_t dat_get_reg(float att);
//��ʼ��
inline void dat_31r5_sp_init();
//д�Ĵ���
inline void dat_write_reg(uint8_t data);
//����˥��ϵ��
void set_att_4351(uint8_t att);



#endif /* ADF4351_H_ */
