#ifndef __TASK_1_H
#define __TASK_1_H

#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "infrared.h"
#include "cba.h"
#include "ultrasonic.h"
#include "canp_hostcom.h"
#include "hard_can.h"
#include "bh1750.h"
#include "syn7318.h"
#include "power_check.h"
#include "can_user.h"
#include "data_base.h"
#include "roadway_check.h"
#include "tba.h"
#include "data_base.h"
#include "swopt_drv.h"
#include "uart_a72.h"
#include "Can_check.h"
#include "delay.h"
#include "can_user.h"
#include "Timer.h"
#include "rc522.h"
#include "string.h"

extern unsigned char Right_RFID[16];
extern unsigned char Error_RFID[16];
extern unsigned char finish_flag;
extern unsigned char lock;
extern unsigned char Light_Initial_Val;
extern unsigned int  MP;	//��������ֵ
extern unsigned char qr_tset[20];

void Test();
void Car_Go(unsigned char speed,unsigned int temp);
void Car_Go_ms(unsigned char speed,unsigned int temp,unsigned int ms);
void Car_Back(unsigned char speed,unsigned int temp);
void Car_Left(unsigned char speed);
void Car_Right(unsigned char speed);
void Car_Track(unsigned char speed);
void Car_Time_Track(uint8_t speed, uint16_t tims);
//void Car_L45(int8_t speed, uint16_t times);
//void Car_R45(int8_t speed, uint16_t times);
void Car_45_Right(unsigned char speed);
void Car_45_Left(unsigned char speed);
void Car_Go_RFID(unsigned char speed,unsigned int temp);
void Car_Track_RFID(unsigned char speed);
void LED_1(unsigned char mode);
void LED_JL();
void LED_color();
void LED_Date_Zigbee(uint8_t One, uint8_t Two, uint8_t Three, uint8_t rank);// LED��ʾ��־����ʾ����
void Car_Nav(unsigned char mode,unsigned char speed,unsigned char temp);
void AGVcar_Left(unsigned char speed);										// ���ƴӳ�
void Gate_Switch_Zigbee(unsigned char mode);								// ��բ��0�رգ�1����
void Gate_Show_Zigbee(unsigned char *p);									// ��բ��ʾ��������
void LIGHT_Scan(unsigned char target);										// ѡ��·�Ƶ�ǰ״̬
void RFID_Check(void);														// ������ȷ��RFID
void Looking_for_color(unsigned char start);								// TFFʶ��ͼ����ɫ����
void Looking_for_license_plates(unsigned char start);						// TFTѰ����ȷ����
void Looking_for_graphics(unsigned char start);								// TFTʶ��ͼ��
void QR_broadcast(unsigned char start,unsigned char end);					// ����������ά��
void trafficA_Lights(unsigned char start);									// ��ͨ��ʶ��
void trafficB_Lights(unsigned char start);
void RFID_Send(unsigned char *p,unsigned char len);
void RFID_AGVInitialPath(unsigned char num);
void RFID_AGVPath();
void TrafficA_AGV(unsigned int start,unsigned char colour);
void TrafficB_AGV(unsigned int start,unsigned char colour);
void GarageA(unsigned char garde);
void GarageB(unsigned char garde);
void GarageA_AGV(unsigned int start);
void GarageB_AGV();
void GarageA_Back_AGV();
void GarageB_Back_AGV();
void Send_Android(uint8_t Pri,uint8_t Sec);									// ������Android�ն˷�������������
void TFT_Test_Zigbee(char Device,uint8_t Pri,uint8_t Sec1,uint8_t Sec2,uint8_t Sec3);	// TFT��ʾ��־�����ָ��
void Rotate_show_Inf(char* src, char x, char y);							// ������ʾ��־����ʾ��������
void Rotate_Dis_Inf(uint16_t dis);      									// ������ʾ��־����ʾ������Ϣ
void TFT_Dis_Zigbee(char Device,uint16_t dis);								// ����TFT��ʾ����ʾ������Ϣ
void TFT_Show_Zigbee(char Device,char *Licence);							// TFT��ʾ����ʾ����
void ETC_Get_Zigbee(void);													// ETC��־��
void YY_Comm_Zigbee(uint8_t Primary, uint8_t Secondary);					// ��������������־�ﲥ��������������
void YY_Play_Zigbee(char *p);												// ��������������־�ﲥ��ָ���ı���Ϣ

extern unsigned char Bcd2ToByte(unsigned char Value);
extern unsigned char ByteToBcd2(unsigned char Value);

#endif












