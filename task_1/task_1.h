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
extern unsigned int  MP;	//控制码盘值
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
void LED_Date_Zigbee(uint8_t One, uint8_t Two, uint8_t Three, uint8_t rank);// LED显示标志物显示数据
void Car_Nav(unsigned char mode,unsigned char speed,unsigned char temp);
void AGVcar_Left(unsigned char speed);										// 控制从车
void Gate_Switch_Zigbee(unsigned char mode);								// 道闸，0关闭，1开启
void Gate_Show_Zigbee(unsigned char *p);									// 道闸显示车牌数据
void LIGHT_Scan(unsigned char target);										// 选择路灯当前状态
void RFID_Check(void);														// 返回正确的RFID
void Looking_for_color(unsigned char start);								// TFF识别图形颜色数量
void Looking_for_license_plates(unsigned char start);						// TFT寻找正确车牌
void Looking_for_graphics(unsigned char start);								// TFT识别图形
void QR_broadcast(unsigned char start,unsigned char end);					// 语音播报二维码
void trafficA_Lights(unsigned char start);									// 交通灯识别
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
void Send_Android(uint8_t Pri,uint8_t Sec);									// 主车向Android终端发送任务请求函数
void TFT_Test_Zigbee(char Device,uint8_t Pri,uint8_t Sec1,uint8_t Sec2,uint8_t Sec3);	// TFT显示标志物控制指令
void Rotate_show_Inf(char* src, char x, char y);							// 立体显示标志物显示车牌数据
void Rotate_Dis_Inf(uint16_t dis);      									// 立体显示标志物显示距离信息
void TFT_Dis_Zigbee(char Device,uint16_t dis);								// 智能TFT显示器显示距离信息
void TFT_Show_Zigbee(char Device,char *Licence);							// TFT显示器显示车牌
void ETC_Get_Zigbee(void);													// ETC标志物
void YY_Comm_Zigbee(uint8_t Primary, uint8_t Secondary);					// 控制语音播报标志物播报语音控制命令
void YY_Play_Zigbee(char *p);												// 控制语音播报标志物播报指定文本信息

extern unsigned char Bcd2ToByte(unsigned char Value);
extern unsigned char ByteToBcd2(unsigned char Value);

#endif












