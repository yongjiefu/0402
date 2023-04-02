#include "task_1.h"

static uint8_t Infrared[6];         // 红外发送数据缓存
static uint8_t Zigbee[8];           // Zigbee发送数据缓存
static uint8_t YY_Init[5] = {0xFD, 0x00, 0x00, 0x01, 0x01};
uint16_t tim_a,tim_b;
static unsigned char ZIGBEE_Buff[8];				//用于缓存zigbee数据		
unsigned char wifi_RX[WIFI_MAX_NUM] = {NULL};		//空数组
unsigned char len;									//数据长度
unsigned char Right_RFID[16];
unsigned char Error_RFID[16] = {NULL};
unsigned char TX_RFID[16];
unsigned char finish_flag = 0;
unsigned char lock = 0;
unsigned char Light_Initial_Val = 0;
unsigned int  MP;									//控制码盘值
unsigned char qr_tset[20];
unsigned char RFID[20];

void Car_Go(unsigned char speed,unsigned int temp)
{
	Roadway_mp_syn();	//码盘同步
	RFID_flag  	    = 0;
	Stop_Flag		= 0;
	Go_Flag   		= 1;
	wheel_L_Flag 	= 0;
	wheel_R_Flag 	= 0;
	wheel_Nav_Flag 	= 0;
	Back_Flag 		= 0;
	Track_Flag 		= 0;
	Car_Spend 		= speed;
	temp_MP			= temp;
	Control(Car_Spend,Car_Spend);
	while(Stop_Flag != 0X03);
}
void Car_Go_ms(unsigned char speed,unsigned int temp,unsigned int ms)
{

	Roadway_mp_syn();			//码盘同步
	RFID_flag  	 = 0;
	MP           = 0;
	finish_flag	 = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	Go_Flag 	 = 1;
	Back_Flag    = 0;
	Track_Flag   = 0;
	Stop_Flag    = 0;
	temp_MP      = temp;
	Control(speed - 10,speed);
	//while(Stop_Flag != 0x03);	//根据通信协议4  的竞争平台循行状态表
	delay_ms(ms);
	lock=1;
	
}

void Car_Back(unsigned char speed,unsigned int temp)
{
	Roadway_mp_syn();	//码盘同步
	RFID_flag       = 0;
	Stop_Flag		= 0;
	Go_Flag   		= 0;
	wheel_L_Flag 	= 0;
	wheel_R_Flag 	= 0;
	wheel_Nav_Flag 	= 0;
	Back_Flag 		= 1;
	Track_Flag 		= 0;
	Car_Spend 		= speed;
	temp_MP			= temp;
	Control(-Car_Spend,-Car_Spend);
	while(Stop_Flag != 0X03);
}

void Car_Left(unsigned char speed)
{
	RFID_flag       = 0;
	Stop_Flag		= 0;
	Go_Flag   		= 0;
	wheel_L_Flag 	= 1;
	wheel_R_Flag 	= 0;
	wheel_Nav_Flag 	= 0;
	Back_Flag 		= 0;
	Track_Flag 		= 0;
	Car_Spend 		= speed;
	Control(-Car_Spend,Car_Spend);
	while(Stop_Flag != 0X02);
}

void Car_Right(unsigned char speed)
{
	RFID_flag       = 0;
	Stop_Flag		= 0;
	Go_Flag   		= 0;
	wheel_L_Flag 	= 0;
	wheel_R_Flag 	= 1;
	wheel_Nav_Flag 	= 0;
	Back_Flag 		= 0;
	Track_Flag 		= 0;
	Car_Spend 		= speed;
	Control(Car_Spend,-Car_Spend);
	while(Stop_Flag != 0X02);
}

void Car_Track(unsigned char speed)
{
	RFID_flag       = 0;
	Stop_Flag		= 0;
	Go_Flag   		= 0;
	wheel_L_Flag 	= 0;
	wheel_R_Flag 	= 0;
	wheel_Nav_Flag 	= 0;
	Back_Flag 		= 0;
	Track_Flag 		= 1;
	Car_Spend 		= speed;
	Control(Car_Spend,Car_Spend);
	while(Stop_Flag != 0X01);
}

void Car_Go_RFID(unsigned char speed,unsigned int temp)
{
	Roadway_mp_syn();		//码盘同步
	RFID_flag		= 1;	//RFID
	Stop_Flag		= 0;
	Go_Flag   		= 1;
	wheel_L_Flag 	= 0;
	wheel_R_Flag 	= 0;
	wheel_Nav_Flag 	= 0;
	Back_Flag 		= 0;
	Track_Flag 		= 0;
	Car_Spend 		= speed;
	temp_MP			= temp;
	Control(Car_Spend,Car_Spend);
	while(Stop_Flag != 0X03);
}
void Car_Track_RFID(unsigned char speed)
{
	RFID_flag		= 1;	//RFID
	Stop_Flag		= 0;
	Go_Flag   		= 0;
	wheel_L_Flag 	= 0;
	wheel_R_Flag 	= 0;
	wheel_Nav_Flag 	= 0;
	Back_Flag 		= 0;
	Track_Flag 		= 1;
	Car_Spend 		= speed;	
	Control(Car_Spend,Car_Spend);	
	while(Stop_Flag != 0X01);
}

void Car_Time_Track(uint8_t speed, uint16_t tims)		// 小车循迹 参数：速度，时间
{
	Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 1;         // 循迹标志位
    Car_Spend = speed;      // 速度值
	Control(Car_Spend, Car_Spend);  // 电机驱动函数 *
	if(tims <= 790)
	{
		delay_ms(tims);
	}
	else
	{
		delay_ms(790);
		delay_ms(tims-790);
	}
	Roadway_Flag_clean();	// 清除标志位状态	
	Send_UpMotor(0,0);		// 停车
	delay_ms(100);
}


void Car_45_Left(unsigned char speed)
{
	Roadway_mp_syn();	//码盘同步
	Stop_Flag		= 0;
	Go_Flag   		= 1;
	wheel_L_Flag 	= 0;
	wheel_R_Flag 	= 0;
	wheel_Nav_Flag 	= 0;
	Back_Flag 		= 0;
	Track_Flag 		= 0;
	Car_Spend 		= speed;
	temp_MP			= 450;
	Control(-Car_Spend,Car_Spend);
}

void Car_45_Right(unsigned char speed)
{
	Roadway_mp_syn();	//码盘同步
	Stop_Flag		= 0;
	Go_Flag   		= 1;
	wheel_L_Flag 	= 0;
	wheel_R_Flag 	= 0;
	wheel_Nav_Flag 	= 0;
	Back_Flag 		= 0;
	Track_Flag 		= 0;
	Car_Spend 		= speed;
	temp_MP			= 450;
	Control(Car_Spend,-Car_Spend);
}

void AGVcar_Left(unsigned char speed)
{
	AGV_Go[3] = speed;
	AGV_Go[6] = (AGV_Go[2]+AGV_Go[3]+AGV_Go[4]+AGV_Go[5])%256;
	Send_ZigbeeData_To_Fifo(AGV_Go,8);
}

void LED_1(unsigned char mode)
{	
	switch(mode)
	{
		case ENABLE:
			Send_ZigbeeData_To_Fifo(SEG_TimOpen,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(SEG_TimOpen,8);
		break;
		case DISABLE:
			Send_ZigbeeData_To_Fifo(SMG_TimClose,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(SMG_TimClose,8);
		break;
	}
}

void LED_JL()
{
	delay_us(1);
	SMG_JL[4] = ByteToBcd2((uint16_t)Ultrasonic_Value / 100);
	delay_us(1);
	SMG_JL[5] = ByteToBcd2((uint16_t)Ultrasonic_Value % 100);
	delay_us(1);
	SMG_JL[6] = (SMG_JL[2]+SMG_JL[3]+SMG_JL[4]+SMG_JL[5])%256;
	delay_us(1);
	Send_ZigbeeData_To_Fifo(SMG_JL,8);
	delay_us(1);
}

void LED_color()
{
	delay_us(1);
	SMG_JL[4] = ByteToBcd2((uint16_t)Ultrasonic_Value / 100);
	delay_us(1);
	SMG_JL[5] = ByteToBcd2((uint16_t)Ultrasonic_Value % 100);
	delay_us(1);
	SMG_JL[6] = (SMG_JL[2]+SMG_JL[3]+SMG_JL[4]+SMG_JL[5])%256;
	delay_us(1);
	Send_ZigbeeData_To_Fifo(SMG_JL,8);
	delay_us(1);
}

/**********************************************************************
 * 函 数 名 ：  LED显示标志物显示数据
 * 参    数 ：  One,Two,Three   数据（十六进制显示格式）
                rank    1 -> 第一行数码管显示数据
                        2 -> 第二行数码管显示数据（默认）
 * 返 回 值 ：  无
 * 简    例 ：  LED_Date_Zigbee(0x12,0x34,0x56,0x01);
**********************************************************************/
void LED_Date_Zigbee(uint8_t One, uint8_t Two, uint8_t Three, uint8_t rank)
{
    Zigbee[0] = 0x55;
    Zigbee[1] = 0x04;
    if(rank == 1)  Zigbee[2] = 0x01;
    else  Zigbee[2] = 0x02;
    Zigbee[3] = One;
    Zigbee[4] = Two;
    Zigbee[5] = Three;
    Zigbee[6] = (Zigbee[2] + Zigbee[3] + Zigbee[4] + Zigbee[5]) % 256;
    Zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Zigbee, 8);
    delay_ms(100);
}

void Car_Nav(unsigned char mode,unsigned char speed,unsigned char temp)
{
	Roadway_mp_syn();	//码盘同步
	Stop_Flag		= 0; 
	Go_Flag			= 0; 
	wheel_L_Flag	= 0;
	wheel_R_Flag	= 0;
	wheel_Nav_Flag	= 1;
	Back_Flag		= 0; 
	Track_Flag		= 0;
	temp_Nav		= temp;
	Car_Spend 		= speed;
	if(mode == 1){	//左转
		Send_UpMotor(-Car_Spend ,Car_Spend);
	}else{			//右转
		Send_UpMotor(Car_Spend ,-Car_Spend);
	}
	while(Stop_Flag != 0X02);	//等待旋转完成
}

void Gate_Switch_Zigbee(unsigned char mode)
{

	switch(mode){
		case 0:
			Send_ZigbeeData_To_Fifo(Gate_Close,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(Gate_Close,8);
		break;
		case 1:
			Send_ZigbeeData_To_Fifo(Gate_Open,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(Gate_Open,8);
		break;
	}
//	unsigned char i;
//	
//	if(mode == 0){
//		Send_ZigbeeData_To_Fifo(Gate_Close,8);	//道闸关闭
//	}else{
//		Send_ZigbeeData_To_Fifo(Gate_Open,8);	//道闸开启
//	}
//	Stop_Flag = 0;
//	while(Stop_Flag != 0X05){
//		delay_ms(10);
//		i++;
//		if(i >= 100){
//			i = 0;
//			Gate_Switch_Zigbee(mode);			//递归
//		}
//		if(Zigbee_Rx_flag){						//判断Zigbee数据回传
//			if(Zigb_Rx_Buf[1] == 0X03){ 		//道闸（包头） 		
//			   if(Zigb_Rx_Buf[2]==0X01){	    				
//					Stop_Flag=Zigb_Rx_Buf[4];	  			 																		
//				}		
//			}		
//		}
//	}
}

void Gate_Show_Zigbee(unsigned char *p)
{
	ZIGBEE_Buff[0] = 0X55;		//包头【1】
	ZIGBEE_Buff[1] = 0X03;		//包头【2】
	ZIGBEE_Buff[2] = 0X10;		//主指令
	ZIGBEE_Buff[3] = *(p+0);	//副指令【1】
	ZIGBEE_Buff[4] = *(p+1);	//副指令【2】
	ZIGBEE_Buff[5] = *(p+2);	//副指令【3】
	ZIGBEE_Buff[6] = (ZIGBEE_Buff[2]+ZIGBEE_Buff[3]+ZIGBEE_Buff[4]+ZIGBEE_Buff[5])%256;	//校验和
	ZIGBEE_Buff[7] = 0XBB;		//包尾
	Send_ZigbeeData_To_Fifo(ZIGBEE_Buff,8);
	delay_ms(100);
	ZIGBEE_Buff[2] = 0X11;		//主指令
	ZIGBEE_Buff[3] = *(p+3);	//副指令【1】
	ZIGBEE_Buff[4] = *(p+4);	//副指令【2】
	ZIGBEE_Buff[5] = *(p+5);	//副指令【3】
	ZIGBEE_Buff[6] = (ZIGBEE_Buff[2]+ZIGBEE_Buff[3]+ZIGBEE_Buff[4]+ZIGBEE_Buff[5])%256;	//校验和
	ZIGBEE_Buff[7] = 0XBB;		//包尾
	Send_ZigbeeData_To_Fifo(ZIGBEE_Buff,8);
	delay_ms(100);
}
void LIGHT_Scan(unsigned char target)
{	
	unsigned int light;
	unsigned int last_light;
	
	for(Light_Initial_Val = 0;Light_Initial_Val < 4;Light_Initial_Val++){
		light = Get_Bh_Value();
		last_light = light;
		Infrared_Send(Light_plus1,4);
		delay_ms(500);
		delay_ms(500);
		light = Get_Bh_Value();
		if(light<last_light){
			Light_Initial_Val = (4 - Light_Initial_Val);
			break;
		}
	}
	switch(Light_Initial_Val){
		case 1:
			SYN_TTS("初始档位为一");
		break;
		case 2:
			SYN_TTS("初始档位为二");
		break;
		case 3:
			SYN_TTS("初始档位为三");
		break;
		case 4:
			SYN_TTS("初始档位为四");
		break;
	}
	switch(target){
		case 1:	
		break;
		case 2:	
			Infrared_Send(Light_plus1,4);	
		break;
		case 3:	
			Infrared_Send(Light_plus2,4);	
		break;
		case 4:	
			Infrared_Send(Light_plus3,4);	
		break;
	}
}

void RFID_Check(void)
{	
	unsigned char i;
	
	if((READ_RFID[0] > '9') && (READ_RFID[2] > '9') && (READ_RFID[4] > '9')){			//判断数组0，2，4位是否为字母
		Send_InfoData_To_Fifo("[0][2][4]ok\n",13);
		if((READ_RFID[1] < '9') && (READ_RFID[3] < '9') && (READ_RFID[5] < '9')){		//判断数组1，3，5位是否为数字
			Send_InfoData_To_Fifo("[1][3][5]ok\n",13);
			for(len = 0;len < 16;len++){
				Right_RFID[len] = READ_RFID[len];
			}
			for(i = 0;i <= 16;i++){
				RFID[i] = Right_RFID[i];
			}
		}else {
			Send_InfoData_To_Fifo("[1][3][5]error\n",16);
			for(len = 0;len < 16;len++){
				Right_RFID[len] = Error_RFID[len];
			}
		}		
	}else {
		Send_InfoData_To_Fifo("[0][2][4]error\n",16);
		for(len = 0;len < 16;len++){
			Right_RFID[len] = Error_RFID[len];
		}
	}	
}
void Test()
{	


}
/***********************************************************
主车请求上位机识别TFT中图形,若不符合要求则翻页
Wifi_Rx_Buf[2]三角形，Wifi_Rx_Buf[3]矩形，Wifi_Rx_Buf[4]圆形
Principal_Tab[];为主车发送上位机数据数组
Wifi_Rx_Buf[];为主车接收上位机数据数组
***********************************************************/
void Looking_for_graphics(unsigned char start)
{	

	unsigned char num          = 0;				
	unsigned char flag         = 1;
	unsigned int broadcast[1]  = {NULL};				  //播报数组
	
	Principal_Tab[10] = 0xa1;											// 识别图形
	Send_WifiData_To_Fifo(Principal_Tab, 12);   
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	
	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
	
	while(flag){
		if(Wifi_Rx_Buf[start] != 0x1a){
			num += 1;
			
//			SYN_TTS("不是图形");
			Send_ZigbeeData_To_Fifo(TFT_PageDown,8);					// TFT下翻页
			delay_ms(500);delay_ms(500);
			
			Principal_Tab[10] = 0xa1;									// 识别图形
			Send_WifiData_To_Fifo(Principal_Tab, 12);   
			delay_ms(500);
			Principal_Tab[10] = 0x00;
			Send_WifiData_To_Fifo(Principal_Tab, 12);
			
			delay_ms(500);delay_ms(500);	
			flag = 1;
			
			if(num > 3){
//				SYN_TTS("三角形数量为一");
//				SYN_TTS("矩形数量为二");
//				SYN_TTS("圆形数量三");
				
				TFT_Test_Zigbee('B',0x40,0xA2,0xD1,0xE2);
				delay_ms(200);
				TFT_Test_Zigbee('B',0x40,0xA2,0xD1,0xE2);
				SYN_TTS("图形识别结束");
				flag = 0;
			}
		}else if(Wifi_Rx_Buf[start] == 0x1a){
			SYN_TTS("是图形");
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TFT_PageUp,8);
//			TFT_Test_Zigbee(0x40,Wifi_Rx_Buf[(start + 1)],Wifi_Rx_Buf[(start + 2)],Wifi_Rx_Buf[(start + 3)]);
			
//			SYN_TTS("三角形数量为");
//			switch(Wifi_Rx_Buf[(start + 1)]){
//				case 0x00:
//					SYN_TTS("0");
//				break;
//				case 0x01:
//					SYN_TTS("1");
//				break;
//				case 0x02:
//					SYN_TTS("2");
//				break;
//				case 0x03:
//					SYN_TTS("3");
//				break;
//				case 0x04:
//					SYN_TTS("4");
//				break;
//				case 0x05:
//					SYN_TTS("5");
//				break;
//				case 0x06:
//					SYN_TTS("6");
//				break;
//				case 0x07:
//					SYN_TTS("7");
//				break;
//				case 0x08:
//					SYN_TTS("8");
//				break;
//				case 0x09:
//					SYN_TTS("9");
//				break;
//			}
//			
//			SYN_TTS("矩形数量为");
//			switch(Wifi_Rx_Buf[(start + 2)]){
//				case 0x00:
//					SYN_TTS("0");
//				break;
//				case 0x01:
//					SYN_TTS("1");
//				break;
//				case 0x02:
//					SYN_TTS("2");
//				break;
//				case 0x03:
//					SYN_TTS("3");
//				break;
//				case 0x04:
//					SYN_TTS("4");
//				break;
//				case 0x05:
//					SYN_TTS("5");
//				break;
//				case 0x06:
//					SYN_TTS("6");
//				break;
//				case 0x07:
//					SYN_TTS("7");
//				break;
//				case 0x08:
//					SYN_TTS("8");
//				break;
//				case 0x09:
//					SYN_TTS("9");
//				break;
//			}
//				
//			SYN_TTS("圆形数量为");
//			switch(Wifi_Rx_Buf[(start + 3)]){
//				case 0x00:
//					SYN_TTS("0");
//				break;
//				case 0x01:
//					SYN_TTS("1");
//				break;
//				case 0x02:
//					SYN_TTS("2");
//				break;
//				case 0x03:
//					SYN_TTS("3");
//				break;
//				case 0x04:
//					SYN_TTS("4");
//				break;
//				case 0x05:
//					SYN_TTS("5");
//				break;
//				case 0x06:
//					SYN_TTS("6");
//				break;
//				case 0x07:
//					SYN_TTS("7");
//				break;
//				case 0x08:
//					SYN_TTS("8");
//				break;
//				case 0x09:
//					SYN_TTS("9");
//				break;
//			}

			SYN_TTS("图形识别完成");
			flag = 0;		
		}
	}
}
void Looking_for_color(unsigned char start)
{
	unsigned char flag = 1;
	unsigned char num  = 0;
	unsigned char i;
	unsigned char null = 0x00;
	
	for(i = 0;i < WIFI_MAX_NUM;i++){
		Wifi_Rx_Buf[i] = null;
		delay_us(1);
	}

	Principal_Tab[10] = 0xa1;							// 识别车牌
	Send_WifiData_To_Fifo(Principal_Tab, 12);   
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);

	while(flag){
		if(Wifi_Rx_Buf[(start + 1)] != 0x1a){
			num += 1;
			SYN_TTS("不是图形");
			Send_ZigbeeData_To_Fifo(TFT_PageDown,8);					// TFT下翻页
			
			Principal_Tab[10] = 0xa1;									// 识别车牌
			Send_WifiData_To_Fifo(Principal_Tab, 12);   
			delay_ms(500);
			Principal_Tab[10] = 0x00;
			Send_WifiData_To_Fifo(Principal_Tab, 12);   
			delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
			flag = 1;
			if(num > 5){
				flag = 0;
			}	
		}else if((Wifi_Rx_Buf[(start + 1)] == 0x1a) && (Wifi_Rx_Buf[start] == 0x55)){
			SYN_TTS("是图形");
			LED_Date_Zigbee((0xF0 + Wifi_Rx_Buf[(start + 2)]),(0xF0 + Wifi_Rx_Buf[(start + 3)]),(0xF0 + Wifi_Rx_Buf[(start + 4)]),0x02);
			delay_ms(200);
			LED_Date_Zigbee((0xF0 + Wifi_Rx_Buf[(start + 2)]),(0xF0 + Wifi_Rx_Buf[(start + 3)]),(0xF0 + Wifi_Rx_Buf[(start + 4)]),0x02);
			flag = 0;
		}
	}
	
}
/*********************************************
主车请求上位机识别TFT中车牌,若不符合要求则翻页
Principal_Tab[];为主车发送上位机数据数组
Wifi_Rx_Buf[];为主车接收上位机数据数组
*********************************************/
void Looking_for_license_plates(unsigned char start)
{	
	unsigned char i;
	unsigned char num = 0;
	unsigned int  plates[20];
	unsigned char null   = 0x00;
	unsigned char flag   = 1;
	unsigned char flag_2 = 0;
	
	for(i = 0;i < WIFI_MAX_NUM;i++){
		Wifi_Rx_Buf[i] = null;
		delay_us(1);
	}
	
	delay_ms(500);
	Principal_Tab[10] = 0xa4;							// 识别车牌
	Send_WifiData_To_Fifo(Principal_Tab, 12);   
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 	
	
	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);	
	
	while(flag){
		if(Wifi_Rx_Buf[start] != 0x4a){
			num += 1;
			SYN_TTS("不是车牌");
			Send_ZigbeeData_To_Fifo(TFT_PageDown,8);					// TFT下翻页
			
			Principal_Tab[10] = 0xa4;									// 识别车牌
			Send_WifiData_To_Fifo(Principal_Tab, 12);   
			delay_ms(500);
			Principal_Tab[10] = 0x00;
			Send_WifiData_To_Fifo(Principal_Tab, 12);   
			delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
			delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
			flag = 1;
			if(num > 5){
				flag = 0;
			}
		}else if(Wifi_Rx_Buf[start] == 0x4a){
			delay_ms(500);
			if((start + 3) != 0x00){
				for(i = 0;i < 20;i++){
					plates[i] =  Wifi_Rx_Buf[(start + 3) + i];			//保存车牌信息到test
					delay_us(1);
				}
				Send_InfoData_To_Fifo((char *)plates,20);
				Send_InfoData_To_Fifo("\n",2);
				Send_InfoData_To_Fifo("\n",2);
				
				flag   = 0;
			}else flag = 1;
		}
	}
	
//	unsigned char flag = 1;
//	unsigned char i;
//	
//	delay_ms(500);delay_ms(500);										// 等待图片刷新
//	
//	Principal_Tab[10] = 0xa4;											// 识别车牌
//	Send_WifiData_To_Fifo(Principal_Tab, 12);   
//	delay_ms(500);
//	Principal_Tab[10] = 0x00;
//	Send_WifiData_To_Fifo(Principal_Tab, 12); 
//	
//	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
//	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
//	
//	while(flag){
//		if(Wifi_Rx_Buf[start] != 0x4a){
//			Send_ZigbeeData_To_Fifo(TFT_PageDown,8);					// TFT下翻页
//			Principal_Tab[10] = 0xa4;									// 识别车牌
//			Send_WifiData_To_Fifo(Principal_Tab, 12);   
//			delay_ms(500);
//			Principal_Tab[10] = 0x00;
//			Send_WifiData_To_Fifo(Principal_Tab, 12);   
//			delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
//			delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
//			flag = 1;
//		}else flag = 0;
//	}
//	Send_WifiData_To_Fifo("license_plates:\n",17);
//	Send_WifiData_To_Fifo(Wifi_Rx_Buf,WIFI_MAX_NUM);
//	Send_WifiData_To_Fifo("\n",2);
//	SYN_TTS("张煜牛马");
}
/******************
播报二维码数据函数
*******************/
void QR_broadcast(unsigned char start,unsigned char end)
{			
	unsigned char broadcast[1] = {NULL};				  //播报数组				
	unsigned char i;
		
	for(i = 0;i < WIFI_MAX_NUM;i++){
		Wifi_Rx_Buf[i] = broadcast[1];
	}
	
	delay_ms(500);delay_ms(500);
	Principal_Tab[10] = 0xa3;							 // 二维码识别
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 	
		
	SYN_TTS("打印二维码消息");
	Send_InfoData_To_Fifo("Wifi_Rx_Buf:\n",14);	
	Send_InfoData_To_Fifo((char *)Wifi_Rx_Buf,20);		//打印到debug区域
	Send_InfoData_To_Fifo("\n",2);
	
	SYN_TTS("二维码信息为");

	for(i = start;i <= end;i++){
		qr_tset[i] = Wifi_Rx_Buf[i];
		delay_us(1);
	}
	
	for(i = start;i <= end;i++){
		broadcast[0] = Wifi_Rx_Buf[i];
		SYN_TTS((char *)broadcast);
	}
	

	
	Send_InfoData_To_Fifo("broadcast:\n",12);
	Send_InfoData_To_Fifo((char *)broadcast,1);			//打印到debug区域
	Send_InfoData_To_Fifo("\n",2);
	

//	SYN_TTS("张煜加油");
}
/*******************
主车交通灯识别 
*******************/
void trafficA_Lights(unsigned char start)
{	
	
	unsigned char flag   = 0;
	unsigned char flag_2 = 1;
	unsigned char test   = 0x00;
	unsigned int i;
	
	for(i = 0;i < WIFI_MAX_NUM;i++){	//清空数组
		Wifi_Rx_Buf[i] = test;
		delay_us(1);
	}
	
	Principal_Tab[10] = 0xa2;								// 识别交通灯,摄像头抬头
	Send_WifiData_To_Fifo(Principal_Tab, 12);   
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	
	Send_ZigbeeData_To_Fifo(TrafficA_Open,8);				// 进入识别模式
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(TrafficA_Open,8);				// 进入识别模式
	delay_ms(200);
	

	while(flag_2){
		for(i = start;i <= WIFI_MAX_NUM;i++){
			if((Wifi_Rx_Buf[i] == 0xC1) || (Wifi_Rx_Buf[i] == 0xC2) || (Wifi_Rx_Buf[i] == 0xC3)){	// 查表
//				SYN_TTS("ok");
				start  = i;	
				start  = start;			
				flag_2 = 0;	
				flag   = 1;
			}
		}
	}	
	while(flag){		
		switch(Wifi_Rx_Buf[start]){
			case 0xc1:
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Red,8);	
				SYN_TTS("识别结果为红色");
				Send_ZigbeeData_To_Fifo(TrafficA_Red,8);
				flag = 0;
			break;
			case 0xc2:
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Yellow,8);	
				SYN_TTS("识别结果为黄色");
				Send_ZigbeeData_To_Fifo(TrafficA_Yellow,8);	
				flag = 0;
			break;
			case 0xc3:
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				SYN_TTS("识别结果为绿色");
				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				flag = 0;
			break;
			default:
//				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				delay_ms(200);
//				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				SYN_TTS("识别结果为随机");

				flag = 1;
//				flag_2 = 1;
		}
	}
//	Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,start);
//	SYN_TTS("交通灯识别完成");
}
void trafficB_Lights(unsigned char start)
{	
	unsigned char flag   = 0;
	unsigned char flag_2 = 1;
	unsigned char test   = 0x00;
	unsigned int i;
	
	for(i = 0;i < WIFI_MAX_NUM;i++){
		Wifi_Rx_Buf[i] = test;
		delay_us(1);
	}
	
	Principal_Tab[10] = 0xa2;								// 识别交通灯,摄像头抬头
	Send_WifiData_To_Fifo(Principal_Tab, 12);   
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	
	Send_ZigbeeData_To_Fifo(TrafficB_Open,8);				// 进入识别模式
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(TrafficB_Open,8);				// 进入识别模式
	delay_ms(200);
	
	while(flag_2){
		for(i = 0;i <= WIFI_MAX_NUM;i++){
			if(Wifi_Rx_Buf[i] == 0x1a){		
//				SYN_TTS("找到1a");
				start  = i + 2;	
				flag   = 1;
				flag_2 = 0;
				
			}
		}
	}	

	while(flag){		
		switch(Wifi_Rx_Buf[start]){
		case 0xc1:
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Red,8);	
			SYN_TTS("识别结果为红色");
			Send_ZigbeeData_To_Fifo(TrafficB_Red,8);
			flag = 0;
		break;
		case 0xc2:
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Yellow,8);	
			SYN_TTS("识别结果为黄色");
			Send_ZigbeeData_To_Fifo(TrafficB_Yellow,8);	
			flag = 0;
		break;
		case 0xc3:
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
			SYN_TTS("识别结果为绿色");
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
			flag = 0;
		break;
		default:
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
			SYN_TTS("识别结果为随机");
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
			flag = 0;
		}
	}
//	Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,start);
	SYN_TTS("交通灯识别完成");
}
/*******************
从车交通灯识别 
*******************/
void TrafficA_AGV(unsigned int start,unsigned char colour)
{	
	unsigned char AGV_Traffic[8] = 
	{0x55,0x02,0x99,0x00,0x00,0x00,(0x99+0x00+0x00+0x00)%256,0xBB};		// 移动机器人交通灯结束
	unsigned char flag   = 0;
	unsigned char flag_2 = 1;
	unsigned char i;
	unsigned char null = 0x00;
	unsigned char num = 0;
	
	for(i = 0;i < ZIGB_RX_MAX;i++){
		Zigb_Rx_Buf[i] = null;
		delay_us(1);
	}
//	SYN_TTS("清空数组");
	
	while(flag_2){
		if((Zigb_Rx_Buf[start] == 0x55) && (Zigb_Rx_Buf[(start + 1)] == 0x02) && (Zigb_Rx_Buf[(start + 10)] == 0x01)){
			Send_ZigbeeData_To_Fifo(TrafficA_Open,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficA_Open,8);
//			SYN_TTS("打开交通灯");
			flag_2 = 0;		
			flag   = 1;		
		}
		num += 1;
		if(num > 3){
			flag_2 = 0;
		}
		SYN_TTS("等待请求");
	}
	
	while(flag){
		switch(colour){
			case 0x01:
				Send_ZigbeeData_To_Fifo(TrafficA_Red,8);
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Red,8);
				SYN_TTS("识别结果为红色");

				flag = 0;
			break;
			case 0x02:
				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				SYN_TTS("识别结果为绿色");

				flag = 0;
			break;
			case 0x03:
				Send_ZigbeeData_To_Fifo(TrafficA_Yellow,8);
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Yellow,8);
				SYN_TTS("识别结果为黄色");

				flag = 0;
			break;
		}
	}
	
//	SYN_TTS("从车交通灯识别完成");
}
void TrafficB_AGV(unsigned int start,unsigned char colour)
{	
	unsigned char AGV_Traffic[8] = 
	{0x55,0x02,0x99,0x00,0x00,0x00,(0x99+0x00+0x00+0x00)%256,0xBB};		// 移动机器人交通灯结束
	unsigned char flag = 0;
	unsigned char flag_2 = 1;
	
	while(flag_2){
		if(Zigb_Rx_Buf[start] == 0x01){
			Send_ZigbeeData_To_Fifo(TrafficB_Open,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Open,8);
			SYN_TTS("打开交通灯");
			flag   = 1;
			flag_2 = 0;
						
		}
	}
	while(flag){
		switch(colour){
		case 0x01:
			Send_ZigbeeData_To_Fifo(TrafficB_Red,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Red,8);
//			SYN_TTS("识别结果为红色");

			flag = 0;
		break;
		case 0x02:
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
//			SYN_TTS("识别结果为绿色");

			flag = 0;
		break;
		case 0x03:
			Send_ZigbeeData_To_Fifo(TrafficB_Yellow,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Yellow,8);
//			SYN_TTS("识别结果为黄色");

			flag = 0;
		break;
		default:
			flag = 1;
		}
	}
	
//	Send_ZigbeeData_To_Fifo(AGV_Traffic,8);
//	delay_ms(200);
//	Send_ZigbeeData_To_Fifo(AGV_Traffic,8);
	
//	SYN_TTS("从车交通灯识别完成");
}
/*******************
车库调节
*******************/
void GarageA(unsigned char garde)
{
	switch(garde){
		case 1:
			Send_ZigbeeData_To_Fifo(GarageA_To1,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(GarageA_To1,8);
		break;
		case 2:
			Send_ZigbeeData_To_Fifo(GarageA_To2,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(GarageA_To2,8);
		break;
		case 3:
			Send_ZigbeeData_To_Fifo(GarageA_To3,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(GarageA_To3,8);
		break;
		case 4:
			Send_ZigbeeData_To_Fifo(GarageA_To4,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(GarageA_To4,8);
		break;
	}
}
void GarageB(unsigned char garde)
{
	switch(garde){
		case 1:
			Send_ZigbeeData_To_Fifo(GarageB_To1,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(GarageB_To1,8);
		break;
		case 2:
			Send_ZigbeeData_To_Fifo(GarageB_To2,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(GarageB_To2,8);
		break;
		case 3:
			Send_ZigbeeData_To_Fifo(GarageB_To3,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(GarageB_To3,8);
		break;
		case 4:
			Send_ZigbeeData_To_Fifo(GarageB_To4,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(GarageB_To4,8);
		break;
	}
}
void GarageA_AGV(unsigned int start)
{
	unsigned char garage = 0;
	unsigned char flag   = 1;
	unsigned char test   = 0x00;
	unsigned int i;
	
	for(i = 0;i < ZIGB_RX_MAX;i++){
		Zigb_Rx_Buf[i] = test;
		delay_us(1);
	}
//	SYN_TTS("清空数组");
	while(flag){
		for(i = 0;i <= ZIGB_RX_MAX;i++){
			if(Zigb_Rx_Buf[i] == 0x55){
				start  = i + 3;
				flag   = 0;
				garage = 1;
			}
		}
		i = 0;
	}	
//	SYN_TTS("调整车库档位");
	while(garage){
		switch(Zigb_Rx_Buf[start]){
		case 0x01:
			GarageA(1);
			SYN_TTS("车库设为1裆");
			garage = 0;
			
		break;
		case 0x02:
			GarageA(2);
			SYN_TTS("车库设为2裆");
			garage = 0;
			
		break;
		case 0x03:
			GarageA(3);
			SYN_TTS("车库设为3裆");
			garage = 0;
			
		break;
		case 0x04:
			GarageA(4);
			SYN_TTS("车库设为4裆");
			garage = 0;
			
		break;
//		default:
//			garage = 1;
		}		
	}	
	SYN_TTS("车库调整完成");
}

void GarageB_AGV()
{
	unsigned char garage = 0;
	
	garage = 1;	
	while(garage){
		switch(Zigb_Rx_Buf[3]){
		case 0x01:
			GarageB(1);
			garage = 0;
		break;
		case 0x02:
			GarageB(2);
			garage = 0;
		break;
		case 0x03:
			GarageB(3);
			garage = 0;
		break;
		case 0x04:
			GarageB(4);
			garage = 0;
		break;
		}		
	}	
}
void GarageA_Back_AGV()
{
	Send_ZigbeeData_To_Fifo(GarageA_Get_Floor,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(GarageA_Get_Floor,8);
	delay_ms(200);
	
	switch(Zigb_Rx_Buf[4]){
		case 0x01:
			SYN_TTS("车库位于第一层");
		break;
		case 0x02:
			SYN_TTS("车库位于第二层");
		break;
		case 0x03:
			SYN_TTS("车库位于第三层");
		break;
		case 0x04:
			SYN_TTS("车库位于第四层");
		break;
	}
}
void GarageB_Back_AGV()
{
	Send_ZigbeeData_To_Fifo(GarageB_Get_Floor,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(GarageB_Get_Floor,8);
	delay_ms(200);
	switch(Zigb_Rx_Buf[4]){
	case 0x01:
		SYN_TTS("车库位于第一层");
	break;
	case 0x02:
		SYN_TTS("车库位于第二层");
	break;
	case 0x03:
		SYN_TTS("车库位于第三层");
	break;
	case 0x04:
		SYN_TTS("车库位于第四层");
	break;
	}
}

void RFID_Send(unsigned char *p,unsigned char len)
{
	Principal_Tab[10] = 0xa5;	//RFID准备发送					 
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	
//	Send_WifiData_To_Fifo(p,len);
//	delay_ms(500);
}

void RFID_AGVInitialPath(unsigned char num)
{	
	unsigned char InitialPath[8] = 
	{0x55,0x02,0xF3,0x00,0x00,0x00,(0xF3+0x00+0x00+0x00)%256,0xBB};
	
	Principal_Tab[10] = 0xa6;	//RFID中从车初始位置					 
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	delay_ms(500);
	SYN_TTS("发送a6了哈张煜");
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12);
	
	SYN_TTS("准备接收张煜发来的消息");
	switch(Wifi_Rx_Buf[num])
	{
		case 0x01:
			InitialPath[2] = 0xF1;
			SYN_TTS("位置是F1别跑偏了呦");
			Send_ZigbeeData_To_Fifo(InitialPath,8);	//发送从车初始位置信息
		break;
		case 0x02:
			InitialPath[2] = 0xF2;
			SYN_TTS("位置是F2别跑偏了呦");
			Send_ZigbeeData_To_Fifo(InitialPath,8);	//发送从车初始位置信息
		break;
		case 0x03:
			InitialPath[2] = 0xF3;
			SYN_TTS("位置是F3别跑偏了呦");
			Send_ZigbeeData_To_Fifo(InitialPath,8);	//发送从车初始位置信息
		break;
	}
	while((Wifi_Rx_Buf[num] != 0x01) && (Wifi_Rx_Buf[num] != 0x02) && (Wifi_Rx_Buf[num] != 0x03));
	SYN_TTS("毕彦哲初始位置识别完成啦");
//	Send_ZigbeeData_To_Fifo(InitialPath,8);	//发送从车初始位置信息
}
void RFID_AGVPath()
{
	Principal_Tab[10] = 0xa7;	//RFID中从车路径					 
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	
}

/*********************************************************************************
 * 函 数 名 ：	主车向Android终端发送任务请求函数
 * 参    数 ：	Pri  -> 主指令
				Sec  -> 副职令
				详见附录1、附录2
 * 返 回 值 ：	无
 * 简    例 ：	Send_Android(0xA1, 0x01);		// 调用摄像头预设位1


附录1：主车向Android终端发送调用任务指令结构
--------------------------------------------------------
| 主指令 | 副指令 |   说明
|--------|--------|-------------------------------------
|  0xA0  |  0x00  | 启动全自动
|--------|--------|-------------------------------------
|  0xA1  |  0x01  | 调用摄像头预设为1
|        |  0x02  | 调用摄像头预设为2
|        |  0x03  | 调用摄像头预设为3
|        |  0x04  | 调用摄像头预设为4
|        |  0x05  | 调用摄像头预设为5
|--------|--------|-------------------------------------
|  0xA2  |  0x00  | 调用二维码识别
|--------|--------|-------------------------------------
|  0xA3  |  0x00  | 调用交通灯识别
|--------|--------|-------------------------------------
|  0xA4  |  0x00  | 调用车牌识别
|--------|--------|-------------------------------------
|  0xA4  |  0x00  | 调用图形识别
--------------------------------------------------------

附录2：Android终端向主车发送识别结果指令结构
-------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |		说明
|--------|-----------|-----------|-----------|---------------------------------
|  0xA2  |   0x00    |   0x00    |   0x00	 | 二维码识别成功
|--------|-----------|-----------|-----------|---------------------------------
|  0xA3  |   0x01    |   0x00    |   0x00	 | 交通灯识别成功（红色）
|        |   0x02    |   0x00    |   0x00	 | 交通灯识别成功（黄色）
|        |   0x03    |   0x00    |   0x00	 | 交通灯识别成功（绿色）
|--------|-----------|-----------|-----------|---------------------------------
|  0xA4  |   0xXX    |   0xXX    |   0xXX	 | 车牌识别成功 车牌前三位ASICC
|--------|-----------|-----------|-----------|---------------------------------
|  0xA5  |   0xXX    |   0xXX    |   0xXX	 | 车牌识别成功 车牌后三位ASICC
|--------|-----------|-----------|-----------|---------------------------------
|  0xA6  |   0xXX    |   0xXX    |   0xXX	 | 图形识别 
|        |（三角形） | （圆形）  | （矩形）  | 
-------------------------------------------------------------------------------
*********************************************************************************/
void Send_Android(uint8_t Pri,uint8_t Sec)
{
	Principal_Tab[0] = 0x55;
	Principal_Tab[1] = 0xAA;
	Principal_Tab[2] = Pri;
	Principal_Tab[3] = Sec;
	Send_WifiData_To_Fifo(Principal_Tab, 4);   // 通过Wifi上传主车数据
	UartA72_TxClear();
	UartA72_TxAddStr(Principal_Tab, 4);        // 通过串口上传主车数据
	UartA72_TxStart();
}


/********************************************************************************
 * 函 数 名 ：  TFT显示标志物控制指令
 * 参    数 ：  Device -> 选择设备
				Pri	   -> 主指令
                Sec1   -> 副职令【1】
				Sec2   -> 副职令【2】
				Sec3   -> 副职令【3】
                详见附录1
 * 返 回 值 ：  无
 * 简    例 ：  TFT_Test_Zigbee('A',0x40,0xA1,0xB2,0xC3);    // TFT显示器显示图形信息

附录1：
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|  0x10  |	 0x00	 | 0x01~0x20 |   0x00    | 由第二副指令指定显示那张图片
|        |	 0x01    |	 0x00	 |   0x00    | 图片向上翻页
|        |   0x02    |	 0x00	 |   0x00    | 图片向下翻页
|		 |   0x03    |	 0x00    |   0x00    | 图片自动向下翻页显示，间隔时间 10S
|--------|-----------|-----------|-----------|-----------------------------------
|  0x20  |	 0xXX    |	 0xXX	 |   0xXX	 | 车牌前三位数据（ASCII）
|--------|-----------|-----------|-----------|-----------------------------------
|  0x21  |	 0xXX	 |	 0xXX	 |   0xXX	 | 车牌后三位数据（ASCII）
|--------|-----------|-----------|-----------|-----------------------------------
|  0x30  |	 0x00	 |	 0x00	 |   0x00	 | 计时模式关闭
|	 	 |	 0x01	 |	 0x00 	 |   0x00	 | 计时模式打开
| 		 |	 0x02	 |	 0x00	 |   0x00	 | 计时模式清零
|--------|-----------|-----------|-----------|-----------------------------------
|  0x40  |	 0xXX	 |	 0xXX    |   0xXX	 | 六位显示数据（HEX格式）
|--------|-----------|-----------|-----------|-----------------------------------
|  0x50  |	 0x00	 |	 0x0X	 |   0xXX	 | 距离显示模式（十进制）
--------------------------------------------------------------------------------
********************************************************************************/
void TFT_Test_Zigbee(char Device,uint8_t Pri,uint8_t Sec1,uint8_t Sec2,uint8_t Sec3)
{
	Zigbee[0] = 0x55;
	if (Device == 'B') { Zigbee[1] = 0x08; }
	else { Zigbee[1] = 0x0B; }
	Zigbee[2] = Pri;
	Zigbee[3] = Sec1;
	Zigbee[4] = Sec2;
	Zigbee[5] = Sec3;
	Zigbee[6] = (Zigbee[2]+Zigbee[3]+Zigbee[4]+Zigbee[5])%256;
	Zigbee[7] = 0xBB;
	Send_ZigbeeData_To_Fifo(Zigbee, 8);
	delay_ms(100);
}
/**********************************************************************
 * 函 数 名 ：  立体显示标志物显示车牌数据
 * 参    数 ：  *src    车牌数据（ASICC）
                x,y     坐标信息
 * 返 回 值 ：  无
 * 简    例 ：  Rotate_show_Inf("A123B4",'C','5');
**********************************************************************/
void Rotate_show_Inf(char* src, char x, char y)
{
    Infrared[0] = 0xFF;			// 起始位
    Infrared[1] = 0x20;			// 模式
    Infrared[2] = *(src + 0);	// 数据【1】
    Infrared[3] = *(src + 1);	// 数据【2】
    Infrared[4] = *(src + 2);	// 数据【3】
    Infrared[5] = *(src + 3);	// 数据【4】
    Infrared_Send(Infrared, 6);
    delay_ms(500);
    Infrared[1] = 0x10;			// 模式
    Infrared[2] = *(src + 4);	// 数据【1】
    Infrared[3] = *(src + 5);	// 数据【2】
    Infrared[4] = x;			// 数据【3】
    Infrared[5] = y;			// 数据【4】
    Infrared_Send(Infrared, 6);
    delay_ms(10);
}

/**********************************************************************
 * 函 数 名 ：  立体显示标志物显示距离信息（单位：ms）
 * 参    数 ：  dis  测距信息（四舍五入）
 * 返 回 值 ：  无
 * 简    例 ：  Rotate_Dis_Inf(123);
**********************************************************************/
void Rotate_Dis_Inf(uint16_t dis)
{
    uint16_t csb = dis; //缓存超声波数据值

    csb += 5;   //四舍五入
    Infrared[0] = 0xFF;
    Infrared[1] = 0x11; //显示距离模式
    Infrared[2] = 0x30 + (uint8_t)(csb / 100 % 10); //距离十位--cm
    Infrared[3] = 0x30 + (uint8_t)(csb / 10 % 10); //距离个位--cm
    Infrared[4] = 0x00;
    Infrared[5] = 0x00;
    Infrared_Send(Infrared, 6);
    delay_ms(100);
}

/**********************************************************************
 * 函 数 名 ：  智能TFT显示器显示距离信息
 * 参    数 ：  dis	距离值
 * 返 回 值 ：  无
 * 简    例 ：  TFT_Dis_Zigbee('A',123);
**********************************************************************/
void TFT_Dis_Zigbee(char Device,uint16_t dis)
{
	Zigbee[0] = 0x55;
	if (Device == 'B') { Zigbee[1] = 0x08; }
	else { Zigbee[1] = 0x0B; }
	Zigbee[2] = 0x50;
	Zigbee[3] = 0x00;
	Zigbee[4] = dis/100%10;
	Zigbee[5] = ((dis/10%10)*16+dis%10);
	Zigbee[6] = (Zigbee[2]+Zigbee[3]+Zigbee[4]+Zigbee[5])%256;
	Zigbee[7] = 0xBB;
	Send_ZigbeeData_To_Fifo(Zigbee, 8);  //发送Zigbee数据
	delay_ms(100);
}


/**********************************************************************
 * 函 数 名 ：  TFT显示器显示车牌
 * 参    数 ：  *Licence -> 车牌数据（ASICC）
 * 返 回 值 ：  无
 * 简    例 ：  TFT_Show_Zigbee('A',"A123B4");
**********************************************************************/
void TFT_Show_Zigbee(char Device,char *Licence)
{
    Zigbee[0] = 0x55;
    if (Device == 'B') { Zigbee[1] = 0x08; }
	else { Zigbee[1] = 0x0B; }
    Zigbee[2] = 0x20;
    Zigbee[3] = *(Licence + 0);     // 车牌数据【1】
    Zigbee[4] = *(Licence + 1);     // 车牌数据【2】
    Zigbee[5] = *(Licence + 2);     // 车牌数据【3】
    Zigbee[6] = (Zigbee[2] + Zigbee[3] + Zigbee[4] + Zigbee[5]) % 256;
    Zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Zigbee, 8);     // 发送Zigbee数据
    delay_ms(500);
    Zigbee[2] = 0x21;
    Zigbee[3] = *(Licence + 3);     // 车牌数据【4】
    Zigbee[4] = *(Licence + 4);     // 车牌数据【5】
    Zigbee[5] = *(Licence + 5);     // 车牌数据【6】
    Zigbee[6] = (Zigbee[2] + Zigbee[3] + Zigbee[4] + Zigbee[5]) % 256;
    Zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Zigbee, 8);     // 发送Zigbee数据
    delay_ms(100);
}

/*****************************************************************
 * 函 数 名 ：	ETC系统检测
 * 参    数 ：	无
 * 返 回 值 ：	无
 * 简     例：	ETC_Get_Zigbee();	// ETC系统检测
*****************************************************************/
void ETC_Get_Zigbee(void)
{
	
	
	tim_a = 0;
	tim_b = 0;
	Stop_Flag = 0;
	while(Stop_Flag != 0x06)
	{
		tim_a++;
		delay_ms(1);
		if(tim_a > 800)
		{
			tim_a = 0;
			tim_b++;
			if(tim_b < 6)
			{
				Car_Back(40, 100);	// 后退
				delay_ms(100);
				Car_Go(40, 110);	//小车前进
			}
			else
			{
				SYN_TTS("ETC系统识别失败，强制通行");
				break;
			}
		}
		
		if(Zigbee_Rx_flag == 1)	 // zigbee返回
		{
			Zigbee_Rx_flag = 0;
			delay_us(5);
			if(Zigb_Rx_Buf[1] == 0x0C)		// ETC系统
			{
				if(Zigb_Rx_Buf[2] == 0x01)
				{
					if(Zigb_Rx_Buf[3] == 0x01)
					{
						Stop_Flag = Zigb_Rx_Buf[4];	  // ETC闸门开启成功
					}
				}
			}
		}
	}
}
/**********************************************************************
 * 函 数 名 ：  控制语音播报标志物播报指定文本信息
 * 参    数 ：  *p  --> 需要发送的数据
 * 返 回 值 ：  无
 * 简    例 ：  YY_Play_Zigbee("北京欢迎您");
**********************************************************************/
void YY_Play_Zigbee(char *p)
{
    uint16_t p_len = strlen(p);             // 文本长度

    YY_Init[1] = 0xff & ((p_len + 2) >> 8); // 数据区长度高八位
    YY_Init[2] = 0xff & (p_len + 2);        // 数据区长度低八位
    Send_ZigbeeData_To_Fifo(YY_Init, 5);
    Send_ZigbeeData_To_Fifo((uint8_t *)p, p_len);
    delay_ms(100);
	
	
}

/**********************************************************************
 * 函 数 名 ：  控制语音播报标志物播报语音控制命令
 * 参    数 ：  Primary   -> 主指令
                Secondary -> 副职令
                详见附录1
 * 返 回 值 ：  无
 * 简    例 ：  YY_Comm_Zigbee(0x20, 0x01);     // 语音播报随机语音命令

附录1：
-----------------------------------------------------------------------
| Primary | Secondary | 说明
|---------|-----------|------------------------------------------------
|  0x10   |  0x02     | 向右转弯
|         |  0x03     | 禁止右转
|         |  0x04     | 左侧行驶
|         |  0x05     | 左行被禁
|         |  0x06     | 原地掉头
|---------|-----------|------------------------------------------------
|  0x20   |  0x01     | 随机指令
|---------|-----------|------------------------------------------------
***********************************************************************/
void YY_Comm_Zigbee(uint8_t Primary, uint8_t Secondary)
{
    Zigbee[0] = 0x55;
    Zigbee[1] = 0x06;
    Zigbee[2] = Primary;
    Zigbee[3] = Secondary;
    Zigbee[4] = 0x00;
    Zigbee[5] = 0x00;
    Zigbee[6] = (Zigbee[2] + Zigbee[3] + Zigbee[4] + Zigbee[5]) % 256;
    Zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Zigbee, 8);
}

unsigned char ByteToBcd2(unsigned char Value)
{
	unsigned char bcdhigh = 0;
	while(Value >= 10)
	{
		bcdhigh++;
		Value -= 10;
	}
	return((unsigned char)(bcdhigh << 4) | Value);
}

unsigned char Bcd2ToByte(unsigned char Value)
{
	unsigned char tmp = 0;
	tmp = ((unsigned char)(Value & (unsigned char)0xF0) >> (unsigned char)0x4) * 10;
	return (tmp + (Value & (unsigned char)0x0F));
}























