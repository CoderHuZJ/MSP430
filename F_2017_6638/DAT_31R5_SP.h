/*
 * DAT_31R5_SP.h
 *
 *  Created on: 2023年3月31日
 *      Author: ZHOU
 */

#ifndef DAT_31R5_SP_H_
#define DAT_31R5_SP_H_
#include <stdint.h>

//将衰减系数(单位dB，步进0.5dB)转换为寄存器的值
uint8_t get_reg(float att);
//初始化
void dat_31r5_sp_init();
//写寄存器
void write_reg(uint8_t data);
//设置衰减系数
void set_att(uint8_t att);


#endif /* DAT_31R5_SP_H_ */
