/*
 * ADF4351.c
 *
 *  Created on: 2023年3月26日
 *      Author: ZHOU
 */
#include "ADF4351.h"
#include <msp430f6638.h>

#define SET_CLK     P3OUT|=BIT0
#define CLR_CLK     P3OUT&=~BIT0

#define SET_DAT     P2OUT|=BIT4
#define CLR_DAT     P2OUT&=~BIT4

#define SET_LE      P2OUT|=BIT5
#define CLR_LE      P2OUT&=~BIT5

#define ATT_SET_CLK     P2OUT |= BIT3;
#define ATT_CLR_CLK     P2OUT &=~BIT3;

#define ATT_SET_DAT     P6OUT |= BIT4;
#define ATT_CLR_DAT     P6OUT &=~BIT4;

#define ATT_SET_LE      P5OUT |= BIT6;
#define ATT_CLR_LE      P5OUT &=~BIT6;

/******************************variable****************************/

/******************************fuvntion****************************/
//  R0:0x00_32_00_00;       //N=100,frac=0
u32 Get_Reg0(u16 INT,u16 FRAC)      //INT FRAC
{
    u32 R0_INT=INT;             //16-BIT
    u32 R0_FRAC=FRAC;           //12-BIT
    u32 R0_OTHER=0x0;
    u32 R0=R0_OTHER|(R0_FRAC<<3)|(R0_INT<<15);
//  printf("R0=%x\n",R0);
    return R0;
}
//  R1:0x00_00_80_11;
u32 Get_Reg1(u8 PRESCALER,u16 MOD)  //0:4/5  1:8/9  MOD
{
    u32 R1_PA=0x0;              //phase adjust
    u32 R1_PRESCALER=PRESCALER; //
    u32 R1_PHASE=0x1;           //12-BIT
    u32 R1_MOD=MOD;             //
    u32 R1_OTHER=0x1;
    u32 R1=R1_OTHER|(R1_MOD<<3)|(R1_PHASE<<15)|(R1_PRESCALER<<27)|(R1_PA<<28);
//  printf("R1=%x\n",R1);
    return R1;
}
//  R2:0x04_02_8e_42;
u32 Get_Reg2(bool DOUBLE,bool RDIV,u16 R)//参考时钟倍频   参考时钟2分频     R计数器
{
    u32 R2_NOISE_MODE=0x0;      //2-BIT
    u32 R2_MUXOUT=0x5;          //3-BIT
    u32 R2_DOUBLE=DOUBLE;       //参考时钟倍频
    u32 R2_RDIV=RDIV;           //参考时钟2分频
    u32 R2_R=R;                 //R计数器
    u32 R2_OTHER=0xe42;
    u32 R2=R2_OTHER|(R2_R<<14)|(R2_RDIV<<24)|(R2_DOUBLE<<25)|(R2_MUXOUT<<26)|(R2_NOISE_MODE<<29);
//  printf("R2=%x\n",R2);
    return R2;
}
//  R3:0x00_00_04_b3;
u32 Get_Reg3(u16 CLK_DIV)   //CLK_DIV
{
    u32 R3_CLK_MODE=0x0;        //band select clk mode  0:PDF<125kHz  1:PDF>125kHz
    u32 R3_ABP=0x0;             //0:6ns(FRAC-N)  1:3ns(INT-N)
    u32 R3_CLK_DIV_MODE=0x0;    //2-BIT 0:clk divider off
    u32 R3_CLK_DIV=CLK_DIV;     //12-BIT
    u32 R3_OTHER=0x3;
    u32 R3=R3_OTHER|(R3_CLK_DIV<<3)|(R3_CLK_DIV_MODE<<15)|(R3_ABP<<22)|(R3_CLK_MODE<<23);
//  printf("R3=%x\n",R3);
    return R3;
}
//  R4:0x00_50_80_3c;           //32分频输出
u32 Get_Reg4(u8 FEED_BACK,u8 RF_DIV,u8 CLK_DIV) //feed back     RF_DIV  CLK_DIV
{
    u32 R4_FEED_BACK=FEED_BACK; //feed back select 0:divided  1:fundamental
    u32 R4_RF_DIV=RF_DIV;       //1/2/4/8/16/32/64 3-BIT
    u32 R4_CLK_DIV=CLK_DIV;     //8-BIT PDF>125kHz时启用
    u32 R4_OTHER=0x03c;
    u32 R4=R4_OTHER|(R4_CLK_DIV<<12)|(R4_RF_DIV<<20)|(R4_FEED_BACK<<23);
//  printf("R4=%x\n",R4);
    return R4;
}
//  R5:0x00_58_00_05;
u32 Get_Reg5(u8 LD_MODE)    //LD_MODE
{
    u32 R5_LD_MODE=LD_MODE;     //2-BIT
    u32 R5_OTHER=0x180005;
    u32 R5=R5_OTHER|(R5_LD_MODE<<22);
//  printf("R5=%x\n",R5);
    return R5;
}
void Write_Reg(u32 data)
{
//    IOWR_ALTERA_AVALON_SPI_SLAVE_SEL(ADF4351_BASE,1);                               //片选
//    IOWR_ALTERA_AVALON_SPI_CONTROL(ADF4351_BASE,ALTERA_AVALON_SPI_CONTROL_SSO_MSK); //强制拉低
//    while((IORD(ADF4351_BASE,2)&0x40)==0);                                          //等待发送数据
//    IOWR(ADF4351_BASE,1,data);                                                      //发送数据
//    while((IORD(ADF4351_BASE,2)&0x40)==0);                                          //等待发送完成
//    IOWR_ALTERA_AVALON_SPI_CONTROL(ADF4351_BASE,ALTERA_AVALON_SPI_CONTROL_SSO_OFST);//释放片选
    u8 ValueToWrite = 0;
    u8 i = 0;
    u8 j = 0;
//  ADF_Output_GPIOInit();

    __delay_cycles(100);
//    ADF4351_CLK = 0;
//    ADF4351_LE = 0;
    CLR_CLK;
    CLR_LE;
    __delay_cycles(100);


    for(i = 0; i < 4; i++)
    {
        ValueToWrite = (data & (0xFF000000 >> (i * 8))) >> (8 * (3 - i));
        for(j = 0; j < 8; j++)
        {
            if(0x80 == (ValueToWrite & 0x80))
            {
//                ADF4351_OUTPUT_DATA = 1;
                SET_DAT;
            }
            else
            {
//                ADF4351_OUTPUT_DATA = 0;
                CLR_DAT;
            }
            __delay_cycles(100);
//            ADF4351_CLK = 1;
            SET_CLK;
            __delay_cycles(100);
            ValueToWrite <<= 1;
//            ADF4351_CLK = 0;
            CLR_CLK;
        }
    }
//    ADF4351_OUTPUT_DATA = 0;
    CLR_DAT;
    __delay_cycles(100);
//    ADF4351_LE = 1;
    SET_LE;
    __delay_cycles(100);
//    ADF4351_LE = 0;
    CLR_LE;
}

void init_adf4351(int INT1,int FRAC1,int div)
{
    P3DIR |= BIT0;
    P2DIR |= BIT4 + BIT5;
    dat_31r5_sp_init();
//    u16 i;
//    u32 reg[6];       //6个32位寄存器
//    reg[0]=0x00320000;        //N=100,frac=0
//    reg[1]=0x00008011;
//    reg[2]=0x04028e42;
//    reg[3]=0x000004b3;
//    reg[4]=0x0050803c;        //32分频输出
//    reg[5]=0x00580005;
      Write_Reg(Get_Reg5(1));
      Write_Reg(Get_Reg4(1,div,8)); //RF_DIV=32
      Write_Reg(Get_Reg3(150));
      Write_Reg(Get_Reg2(true,true,2)); //D=1 T=1  R=1
      Write_Reg(Get_Reg1(1,100));    //相位调整使能      MOD=10
      Write_Reg(Get_Reg0(INT1,FRAC1));   //寄存器0  INT=100 frac==3
      //******f_out=25M*(INT+FRAC/MOD)/(R*RF_DIV)******//
      //发送数据
//    for(i=0;i<6;i++)
//    {
//        Write_Reg(reg[5-i]);
//    }

}

void set_ADF4351_fre(double fre)
{
    int INT;
    int FRAC;
    double inter;
    if(fre>275)
    {
    inter = fre * 1.6;
    INT = inter;
    FRAC = (inter - INT)*100;
    init_adf4351(INT,FRAC,3);
    }
    else if(fre > 68 && fre <= 137)
    {
    inter = fre * 6.4;
    INT = inter;
    FRAC = (inter - INT)*100;
    init_adf4351(INT,FRAC,5);
    }
    else if(fre > 137 && fre <= 275)
    {
    inter = fre * 3.2;
    INT = inter;
    FRAC = (inter - INT)*100;
    init_adf4351(INT,FRAC,4);       //2200/16 = 137
    }
    else if(fre <= 68)
    {
    inter = fre * 12.8;
    INT = inter;
    FRAC = (inter - INT)*100;
    init_adf4351(INT,FRAC,6);
    }
}

//将衰减系数(单位dB，步进0.5dB)转换为寄存器的值
inline uint8_t dat_get_reg(float att)
{
    uint8_t ret_val = (uint8_t)(att * 2.0);
    return ret_val;
}
//初始化
inline void dat_31r5_sp_init()
{

    P2DIR |= BIT3;
    P6DIR |= BIT4;
    P5DIR |= BIT6;

    ATT_CLR_LE;
    ATT_CLR_CLK;
    ATT_CLR_DAT;
}
//写寄存器
inline void dat_write_reg(uint8_t data)
{
    ATT_CLR_LE;
    ATT_CLR_CLK;
    ATT_CLR_DAT;

    int j = 0;
    for(j = 0; j < 8; j++)
    {
        if(0x80 == (data & 0x80))
        {
            ATT_SET_DAT;
        }
        else
        {
            ATT_CLR_DAT;
        }
        __delay_cycles(100);
        ATT_SET_CLK;
        __delay_cycles(100);
        data <<= 1;
        ATT_CLR_CLK;
    }
    ATT_CLR_DAT;
    __delay_cycles(100);
    ATT_SET_LE;
    __delay_cycles(100);
    ATT_CLR_LE;
}
//设置衰减系数
void set_att_4351(uint8_t att)
{
    dat_write_reg(dat_get_reg(att/2));
    return;
}

