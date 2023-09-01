#include <msp430.h> 
#include <stdint.h>
#include <stdio.h>
#include "AD9910.h"
#include "9910.h"
#include "ADS1118.h"
#include "TLV5638.h"
#include "DAT_31R5_SP.h"
#include "ADF4351.h"
#include "src/setclock.h"
#include <math.h>


uint8_t str[2] = {0};
int char_num = 0;
uint8_t rx_flag = 0;


void init_uart(u16 Baudrate);

int fputc(int ch,FILE *f)
{
    UCA0TXBUF = ch&0xff;
    while(!(UCA0IFG & UCTXIFG));//等待发送完成
     return ch;
}
int fputs(const char *_ptr, register FILE *_fp)
{
  unsigned int i, len;

  len = strlen(_ptr);

  for(i=0 ; i<len ; i++){
    UCA0TXBUF = _ptr[i]&0xff;
    while(!(UCA0IFG & UCTXIFG));//等待发送完成

  }

  return len;
}


void send_end()
{
    printf("\xff\xff\xff");
}

double Fre = 10.0;  //1MHz
double Fre_dete = 0.0;
double Fre_dete_C = 0.0;
double Amp = 0.50;  //5mv
double Amp_l = 5.0;
int mode = 0;       //mode为0时，不进行检测；mode为1时，进行长度检测；mode为2时，进行负载检测
double Length = 0.0;
double Length_C = 0.0;
double R_num = 0.0;
double C_num = 0.0;

/**
 * main.c
 */
int main(void)
  {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    SetClock_MCLK12MHZ_SMCLK12MHZ_ACLK32_768K();
    init_uart(9600);

    AD9910_Init_F6638();
    ADS1118_Init();
    TLV5638_Init();

    P2DIR |= BIT7; // P2.7 设置为输出

    P2DIR &=~BIT6; //设置按键对应管脚P2.6和P3.1为输入
    P3DIR &=~BIT1;
    P2REN |= BIT6; //使能按键管脚上下拉
    P3REN |= BIT1;
    P2OUT |= BIT6; //设置管脚均为上拉
    P3OUT |= BIT1;

    P6DIR |= BIT0;
    P6OUT |= BIT0;


    while (1)
    {
//        ADS1118_write_read();
//        TLV5638_write();
//        AD9910_SetFreAndAmp(0.1 * 100000, 0.0465 * 99 / 5.0);
        if(!(P2IN&BIT6)) //P2.6检测到低电平
        {
           _delay_cycles(80000); //延时消抖
           if(!(P2IN&BIT6)) //再次检测到P2.6为低电平
           {
               printf("page page0");
               send_end();
               P2OUT ^= BIT7; //翻转P2.7对应LED状态
           }
        }

    }

	return 0;
}

void init_uart(u16 Baudrate)
{
    UCA0CTL1 |= UCSWRST; // 置位UCSWRST
    UCA0CTL1 |= UCSSEL_1; // 选择时钟源，CLK = ACLK 32768Hz

    switch(Baudrate) // 配置UART通信的波特率
    {
        case 4800:
            UCA0BR0 = 0x06; // 32768/4800=6.827，整数低八位存入UCA1BR0
            UCA0BR1 = 0x0; // 整数高八位存入UCA1BR1
            UCA0MCTL = UCBRS_7+UCBRF_0; // 小数部分：round(0.827*8) = round(6.616) = 7
            break;
        case 9600:
            UCA0BR0 = 0x03; // 32768/9600=3.413
            UCA0BR1 = 0x0;
            UCA0MCTL = UCBRS_3+UCBRF_0; //round(0.413*8) = round(3.304) = 3
            break;
        default:
            UCA0BR0 = 0x03; // 9600 Baud
            UCA0BR1 = 0x0;
            UCA0MCTL = UCBRS_3+UCBRF_0;
    }

    P2SEL |= BIT4 | BIT5; // 设置P2.4 TXD和P2.5 RXD位复用引脚
    /***
    * MSP430F6638: P8.2: TX; P8.3: RX
    * MSP430F5529: P4.4: TX; P4.5: RX
    ***/
    UCA0CTL1 &= ~UCSWRST; // 复位UCSWRST，禁用软件重置
    UCA0IE |= UCRXIE; // 打开接收中断
    __enable_interrupt(); // 全局中断使能
}

/* ======== USCI A0/B0 TX Interrupt Handler Generation ======== */
#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void){
    switch(__even_in_range(UCA0IV,4)){
    case 0:break; //vector 0 : no interrupt 无中断
    case 2: //vector 2 : RXIFG 接收中断处理
        str[char_num] = UCA0RXBUF;   //将接收缓存区读到str中
        if(UCA0RXBUF == 0x55)        //如果读取到帧头
        {
            rx_flag = 1;             //开始读取数据
        }
        else if(rx_flag == 1)
        {
            char_num++;
            if(char_num >= 2)
            {
                char_num = 0;
                rx_flag = 0;
                if(str[1] == 0x00)   //修改AD9910的频率
                {
                    if(str[0] == 0x10)//频率+10MHz
                    {
                        Fre = Fre + 100.0;
                        if(Fre >= 2000)
                            Fre = 2000.0;
                        AD9910_SetFreAndAmp(Fre * 100000, 0.0465 * Amp_l / 5.0);
                    }
                    else if(str[0] == 0x11)//频率-10MHz
                    {
                        Fre = Fre - 100.0;
                        if(Fre <= 1)
                            Fre = 1.0;
                        AD9910_SetFreAndAmp(Fre * 100000, 0.0465 * Amp_l / 5.0);
                    }
                    else if(str[0] == 0x12)//频率+1MHz
                    {
                        Fre = Fre + 10.0;
                        if(Fre >= 2000)
                            Fre = 2000.0;
                        AD9910_SetFreAndAmp(Fre * 100000, 0.0465 * Amp_l / 5.0);
                    }
                    else if(str[0] == 0x13)//频率-1MHz
                    {
                        Fre = Fre - 10.0;
                        if(Fre <= 1)
                            Fre = 1.0;
                        AD9910_SetFreAndAmp(Fre * 100000, 0.0465 * Amp_l / 5.0);
                    }
                    else if(str[0] == 0x14)//频率+0.1MHz
                    {
                        Fre = Fre + 1.0;
                        if(Fre >= 2000)
                            Fre = 2000.0;
                        AD9910_SetFreAndAmp(Fre * 100000, 0.0465 * Amp_l / 5.0);
                    }
                    else if(str[0] == 0x15)//频率-0.1MHz
                    {
                        Fre = Fre - 1.0;
                        if(Fre <= 1)
                            Fre = 1.0;
                        AD9910_SetFreAndAmp(Fre * 100000, 0.0465 * Amp_l / 5.0);
                    }
                }
                else if(str[1] == 0x01)   //修改AD9910的幅度
                {
                    if(str[0] == 0x10)
                    {
                        Amp_l = Amp_l + 10;
                        if(Amp_l >= 99)
                            Amp_l = 99.0;
                        AD9910_SetFreAndAmp(Fre * 100000, 0.0465 * Amp_l / 5.0);
                    }
                    else if(str[0] == 0x11)
                    {
                        Amp_l = Amp_l - 10;
                        if(Amp_l <= 1)
                            Amp_l = 1.0;
                        AD9910_SetFreAndAmp(Fre * 100000, 0.0465 * Amp_l / 5.0);
                    }
                    else if(str[0] == 0x12)
                    {
                        Amp_l = Amp_l + 1;
                        if(Amp_l >= 99)
                            Amp_l = 99.0;
                        AD9910_SetFreAndAmp(Fre * 100000, 0.0465 * Amp_l / 5.0);
                    }
                    else if(str[0] == 0x13)
                    {
                        Amp_l = Amp_l - 1;
                        if(Amp_l <= 1)
                            Amp_l = 1.0;
                        AD9910_SetFreAndAmp(Fre * 100000, 0.0465 * Amp_l / 5.0);
                    }
                }
                else if(str[1] == 0x03)
                {
                    if(str[0] == 0x00)        //长度检测开始
                    {
                       mode = 1;
                       Fre = 140.0;
                       Amp = 0.99;
                       Amp_l = 99;
                       int data_min = 100;
//                       double Fre_min''
                       int data_read_reg1;
                       int data_read_reg2;
                       int data_read_reg3;
                       int data_read_reg4;
                       int data_read_reg5;
                       int data_read_reg6;
                       while(true)
                       {
                           data_read_reg6 = data_read_reg5;
                           data_read_reg5 = data_read_reg4;
                           data_read_reg4 = data_read_reg3;
                           data_read_reg3 = data_read_reg2;
                           data_read_reg2 = data_read_reg1;
                           data_read_reg1 = data_read;

                           AD9910_SetFreAndAmp(Fre * 10000, 0.0465 * Amp_l / 5.0);
//                           _delay_cycles(200);
                           ADS1118_write_read();
                           _delay_cycles(20);
                           ADS1118_write_read();

                           if(Fre > 150)
                           {
                               if(data_read_reg5 < data_read_reg6 && data_read_reg4 <= data_read_reg5 && data_read_reg3 <= data_read_reg4 && data_read_reg2 >= data_read_reg3 && data_read_reg1 > data_read_reg2 && data_read >= data_read_reg1)
                               {
                                   Fre_dete = Fre - 4.0;
                                   break;
                               }

                               if(data_read_reg3 <= data_read_reg4 || data_read_reg4 <= data_read_reg5 || data_read_reg5 <= data_read_reg6)
                               {
                                   Fre = Fre + 2;
                               }
                               else if(data_read_reg1 >= data_read_reg2 || data_read_reg2 >= data_read_reg3)
                               {
                                   Fre = Fre - 1;
                               }
                           }
                           else
                           {
                               Fre = Fre + 1.0;
                           }


                           if(Fre >= 7000.0)
                           {
//                               Fre = 200.0;
                               break;
                           }
                       }

                       AD9910_SetFreAndAmp((Fre_dete - 1.0) * 10000, 0.0465 * 99 / 5.0);
                       ADS1118_write_read();
                       _delay_cycles(20);
                       ADS1118_write_read();
                       data_read_reg4 = data_read;

                       AD9910_SetFreAndAmp((Fre_dete + 1.0) * 10000, 0.0465 * 99 / 5.0);
                       ADS1118_write_read();
                       _delay_cycles(20);
                       ADS1118_write_read();
                       data_read_reg2 = data_read;

                       if(data_read_reg4 <= data_read_reg3 && data_read_reg4 <= data_read_reg2)
                           Fre_dete = Fre_dete - 1.0;
                       else if(data_read_reg4 >= data_read_reg2 && data_read_reg3 >= data_read_reg2)
                           Fre_dete = Fre_dete + 1.0;
                       else
                           Fre_dete = Fre_dete;

                       AD9910_SetFreAndAmp(Fre_dete * 10000, 0.0465 * 99 / 5.0);
                       Length = 0.69 * 3000000 / (Fre_dete * 4) * 0.94;
                       if(Length > 800 && Length < 1700)
                           Length = Length * 1.013 - 1.796 + 13;
                       if(Length >=1700 && Length < 1900)
                           Length = Length * 1.013 - 1.796 + 33;
                       else if(Length <800 && Length >300)
                           Length = Length + 10;
                       else if(Length <100)
                           Length = Length + 2.4;
                       else if(Length >1900)
                           Length = Length + 70;
                       else if(Length > 2040)
                           Length = 2101.0;
                       printf("main_page.bt0.val=0");
                       send_end();
                       printf("main_page.t3.txt=\"%.1f cm\"", Length);
                       send_end();
                    }
                    else if(str[0] == 0x01)   //负载检测开始
                    {
//                        P6OUT &= ~BIT0;
                        _delay_cycles(2000);
                        mode = 2;
                        ADS1118_write_read();
//                        TLV5638_write();      //TLV5638输出直流
                        AD9910_SetFreAndAmp(1.0 * 10000, 0.0465 * 99.0 / 5.0);
                        _delay_cycles(400000);
                        ADS1118_write_read();
                        _delay_cycles(400000);
                        ADS1118_write_read();
//                        _delay_cycles(800000);
//                        ADS1118_write_read();
//                        _delay_cycles(10);
//                        P6OUT |= BIT0;
//                        printf("main_page.t3.txt=\"%d\"", (uint16_t)data_read);
                        send_end();
                        if(data_read > 10000)  //电容或断路
                        {
                            AD9910_SetFreAndAmp(Fre_dete * 10000, 0.0465 * 99.0 / 5.0);
                            _delay_cycles(400000);
                            ADS1118_write_read();
                            _delay_cycles(400000);
                            ADS1118_write_read();
//                            printf("main_page.t7.txt=\"%d\"", (uint16_t)data_read);
                            send_end();
                            if(data_read < 4650)  //开路
                            {
                                printf("main_page.t5.txt=\"open\"");
                                send_end();

                                printf("main_page.t7.txt=" "");
                                send_end();

                                printf("main_page.bt1.val=0");
                                send_end();
                            }
                            else                  //电容
                            {
                                int data_read_reg1;
                                int data_read_reg2;
                                int data_read_reg3;
                                int data_read_reg4;
                                Fre = 100.0;
                                Amp_l = 99;

                                AD9910_SetFreAndAmp(50.0 * 10000, 0.0465 * Amp_l / 5.0);
                                _delay_cycles(60);
                                ADS1118_write_read();
                                _delay_cycles(60);
                                ADS1118_write_read();
                                while(true)
                                {
                                     data_read_reg4 = data_read_reg3;
                                     data_read_reg3 = data_read_reg2;
                                     data_read_reg2 = data_read_reg1;
                                     data_read_reg1 = data_read;
                                     AD9910_SetFreAndAmp(Fre * 10000, 0.0465 * Amp_l / 5.0);
                                     _delay_cycles(60);
                                     ADS1118_write_read();
                                     _delay_cycles(20000);
                                     ADS1118_write_read();


                                     if(Fre > 120)
                                     {
                                        if(data_read_reg3 <= data_read_reg4 && data_read_reg2 <= data_read_reg3 && data_read_reg1 >= data_read_reg2 && data_read >= data_read_reg1)
                                        {
                                            Fre_dete_C = Fre - 1.0;
                                            break;
                                        }
                                        if(data_read_reg3 <= data_read_reg4 || data_read_reg2 <= data_read_reg3)
                                        {
                                            Fre = Fre + 1;
                                        }
                                        else if(data_read_reg3 >= data_read_reg4 || data_read_reg2 >= data_read_reg3)
                                        {
                                            Fre = Fre - 1;
                                        }
                                     }
                                     else
                                     {
                                         Fre = Fre + 1.0;
                                     }

                                     if(Fre >= 2000.0)
                                     {
                                         break;
                                     }
                                 }

                                Length_C = 0.69 * 3000000 / (Fre_dete_C * 4) * 0.94;

                                double Length_delta = 0.0;
                                Length_delta = Length_C - Length ;

//                                printf("main_page.t3.txt=\"%.1f\"", Length_delta);
                                send_end();

                                double temp = 0.0;
                                double temp1 = 0.0;
                                double temp2 = 0.0;

                                temp1 = cos(2 * 3.14 * Length_delta / (4 * Length));
                                temp2 = sin(2 * 3.14 * Length_delta / (4 * Length));

                                temp = sin(2 * 3.14 * Length_delta / (4 * Length)) / cos(2 * 3.14 * Length_delta / (4 * Length)) ;

                                double temp_fre1 = temp / (Fre_dete * 3) * 1000000 * 0.99665 + 8;

                                if(Length >= 950 && Length < 1050)
                                    temp_fre1 = temp_fre1;
                                else if(Length >= 1150 && Length < 1250)
                                    temp_fre1 = temp_fre1 - 4;
                                else if(Length >= 1250 && Length < 1350)
                                    temp_fre1 = temp_fre1 - 3;
                                else if(Length >= 1450 && Length < 1550)
                                    temp_fre1 = temp_fre1 - 17;
                                else if(Length >= 1750 && Length < 1850)
                                    temp_fre1 = temp_fre1 + 3;
                                else if(Length >= 1950 && Length < 2050)
                                    temp_fre1 = temp_fre1 + 19;

                                AD9910_SetFreAndAmp(Fre_dete * 10000, 0.0465 * Amp_l / 5.0);
                                _delay_cycles(2000);

//                                printf("main_page.t7.txt=\"%.3f %.1f\"", temp_fre1, Length);
                                printf("main_page.t7.txt=\"%.1f pF\"", temp_fre1);
                                send_end();

//                                printf("main_page.t3.txt=\"%.3f %.1f\"", temp2, Length_delta);
//                                send_end();

//                                printf("main_page.t3.txt=\"%d cm\"", (uint16_t)Length_C);
//                                send_end();

                                printf("main_page.t5.txt=\"C\"");
                                send_end();

                                printf("main_page.bt1.val=0");
                                send_end();
                             }
                        }
                        else                  //电阻
                        {
                            AD9910_SetFreAndAmp(1.0 * 10000, 0.0465 * Amp_l / 5.0);
                            _delay_cycles(2000000);
                            ADS1118_write_read();
                            _delay_cycles(8000000);
                            ADS1118_write_read();
                            R_num = data_read * 0.014 - 45.21;
                            R_num = R_num * (0.65002) - 3.8 + (Length - 1100) * 0.0011583;
                            if(R_num < 12)
                            {
                                R_num = R_num + 0.2;
                            }
                            if(Length > 1850)
                                R_num = R_num;
                            else if(Length >= 1750 && Length < 1850)
                                R_num = R_num + 0.3;
                            else if(Length >= 1650 && Length < 1750)
                                R_num = R_num + 0.5;
                            else if(Length >= 1550 && Length < 1650)
                                R_num = R_num + 0.7;
                            else if(Length >= 1450 && Length < 1550)
                                R_num = R_num + 0.9;
                            else if(Length >= 1350 && Length < 1450)
                                R_num = R_num + 1.1;
                            else if(Length >= 1250 && Length < 1350)
                                R_num = R_num + 1.3;
                            else if(Length >= 1150 && Length < 1250)
                                R_num = R_num + 1.5;
                            else if(Length >= 1050 && Length < 1150)
                                R_num = R_num + 1.8;
                            else if(Length >= 950 && Length < 1050)
                                R_num = R_num + 2.1;
                                //                            + (Length * Length * 0.006525/ 10000 - 0.2829 * Length / 100 + 2.169)
                            if(R_num > 0)
                            {
                                printf("main_page.t7.txt=\"%.1f Ohm\"", R_num);
                                send_end();
                                printf("main_page.t5.txt=\"R\"");
                                send_end();
                            }
                            else
                            {
                                printf("main_page.t5.txt=\"S\"");
                                send_end();
                                printf("main_page.t7.txt=" "");
                                send_end();
                            }

                            printf("main_page.bt1.val=0");
                            send_end();

                            AD9910_SetFreAndAmp(Fre_dete * 10000, 0.0465 * Amp_l / 5.0);
                            _delay_cycles(2000);
                            ADS1118_write_read();
                        }
                    }
                }
            }
        }
        break;

    default: break;
    }
    __enable_interrupt();
    UCA0IE |= UCRXIE; //重新打开发送中断和接收中断
    __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
}
