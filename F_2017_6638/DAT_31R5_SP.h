/*
 * DAT_31R5_SP.h
 *
 *  Created on: 2023��3��31��
 *      Author: ZHOU
 */

#ifndef DAT_31R5_SP_H_
#define DAT_31R5_SP_H_
#include <stdint.h>

//��˥��ϵ��(��λdB������0.5dB)ת��Ϊ�Ĵ�����ֵ
uint8_t get_reg(float att);
//��ʼ��
void dat_31r5_sp_init();
//д�Ĵ���
void write_reg(uint8_t data);
//����˥��ϵ��
void set_att(uint8_t att);


#endif /* DAT_31R5_SP_H_ */
