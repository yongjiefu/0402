#include "task_1.h"

static uint8_t Infrared[6];         // ���ⷢ�����ݻ���
static uint8_t Zigbee[8];           // Zigbee�������ݻ���
static uint8_t YY_Init[5] = {0xFD, 0x00, 0x00, 0x01, 0x01};
uint16_t tim_a,tim_b;
static unsigned char ZIGBEE_Buff[8];				//���ڻ���zigbee����		
unsigned char wifi_RX[WIFI_MAX_NUM] = {NULL};		//������
unsigned char len;									//���ݳ���
unsigned char Right_RFID[16];
unsigned char Error_RFID[16] = {NULL};
unsigned char TX_RFID[16];
unsigned char finish_flag = 0;
unsigned char lock = 0;
unsigned char Light_Initial_Val = 0;
unsigned int  MP;									//��������ֵ
unsigned char qr_tset[20];
unsigned char RFID[20];

void Car_Go(unsigned char speed,unsigned int temp)
{
	Roadway_mp_syn();	//����ͬ��
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

	Roadway_mp_syn();			//����ͬ��
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
	//while(Stop_Flag != 0x03);	//����ͨ��Э��4  �ľ���ƽ̨ѭ��״̬��
	delay_ms(ms);
	lock=1;
	
}

void Car_Back(unsigned char speed,unsigned int temp)
{
	Roadway_mp_syn();	//����ͬ��
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
	Roadway_mp_syn();		//����ͬ��
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

void Car_Time_Track(uint8_t speed, uint16_t tims)		// С��ѭ�� �������ٶȣ�ʱ��
{
	Stop_Flag = 0;          // ����״̬��־λ
    Go_Flag = 0;            // ǰ����־λ
    wheel_L_Flag = 0;       // ��ת��־λ
    wheel_R_Flag = 0;       // ��ת��־λ
    wheel_Nav_Flag = 0;     // ������ת��־λ
    Back_Flag = 0;          // ���˱�־λ
    Track_Flag = 1;         // ѭ����־λ
    Car_Spend = speed;      // �ٶ�ֵ
	Control(Car_Spend, Car_Spend);  // ����������� *
	if(tims <= 790)
	{
		delay_ms(tims);
	}
	else
	{
		delay_ms(790);
		delay_ms(tims-790);
	}
	Roadway_Flag_clean();	// �����־λ״̬	
	Send_UpMotor(0,0);		// ͣ��
	delay_ms(100);
}


void Car_45_Left(unsigned char speed)
{
	Roadway_mp_syn();	//����ͬ��
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
	Roadway_mp_syn();	//����ͬ��
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
 * �� �� �� ��  LED��ʾ��־����ʾ����
 * ��    �� ��  One,Two,Three   ���ݣ�ʮ��������ʾ��ʽ��
                rank    1 -> ��һ���������ʾ����
                        2 -> �ڶ����������ʾ���ݣ�Ĭ�ϣ�
 * �� �� ֵ ��  ��
 * ��    �� ��  LED_Date_Zigbee(0x12,0x34,0x56,0x01);
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
	Roadway_mp_syn();	//����ͬ��
	Stop_Flag		= 0; 
	Go_Flag			= 0; 
	wheel_L_Flag	= 0;
	wheel_R_Flag	= 0;
	wheel_Nav_Flag	= 1;
	Back_Flag		= 0; 
	Track_Flag		= 0;
	temp_Nav		= temp;
	Car_Spend 		= speed;
	if(mode == 1){	//��ת
		Send_UpMotor(-Car_Spend ,Car_Spend);
	}else{			//��ת
		Send_UpMotor(Car_Spend ,-Car_Spend);
	}
	while(Stop_Flag != 0X02);	//�ȴ���ת���
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
//		Send_ZigbeeData_To_Fifo(Gate_Close,8);	//��բ�ر�
//	}else{
//		Send_ZigbeeData_To_Fifo(Gate_Open,8);	//��բ����
//	}
//	Stop_Flag = 0;
//	while(Stop_Flag != 0X05){
//		delay_ms(10);
//		i++;
//		if(i >= 100){
//			i = 0;
//			Gate_Switch_Zigbee(mode);			//�ݹ�
//		}
//		if(Zigbee_Rx_flag){						//�ж�Zigbee���ݻش�
//			if(Zigb_Rx_Buf[1] == 0X03){ 		//��բ����ͷ�� 		
//			   if(Zigb_Rx_Buf[2]==0X01){	    				
//					Stop_Flag=Zigb_Rx_Buf[4];	  			 																		
//				}		
//			}		
//		}
//	}
}

void Gate_Show_Zigbee(unsigned char *p)
{
	ZIGBEE_Buff[0] = 0X55;		//��ͷ��1��
	ZIGBEE_Buff[1] = 0X03;		//��ͷ��2��
	ZIGBEE_Buff[2] = 0X10;		//��ָ��
	ZIGBEE_Buff[3] = *(p+0);	//��ָ�1��
	ZIGBEE_Buff[4] = *(p+1);	//��ָ�2��
	ZIGBEE_Buff[5] = *(p+2);	//��ָ�3��
	ZIGBEE_Buff[6] = (ZIGBEE_Buff[2]+ZIGBEE_Buff[3]+ZIGBEE_Buff[4]+ZIGBEE_Buff[5])%256;	//У���
	ZIGBEE_Buff[7] = 0XBB;		//��β
	Send_ZigbeeData_To_Fifo(ZIGBEE_Buff,8);
	delay_ms(100);
	ZIGBEE_Buff[2] = 0X11;		//��ָ��
	ZIGBEE_Buff[3] = *(p+3);	//��ָ�1��
	ZIGBEE_Buff[4] = *(p+4);	//��ָ�2��
	ZIGBEE_Buff[5] = *(p+5);	//��ָ�3��
	ZIGBEE_Buff[6] = (ZIGBEE_Buff[2]+ZIGBEE_Buff[3]+ZIGBEE_Buff[4]+ZIGBEE_Buff[5])%256;	//У���
	ZIGBEE_Buff[7] = 0XBB;		//��β
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
			SYN_TTS("��ʼ��λΪһ");
		break;
		case 2:
			SYN_TTS("��ʼ��λΪ��");
		break;
		case 3:
			SYN_TTS("��ʼ��λΪ��");
		break;
		case 4:
			SYN_TTS("��ʼ��λΪ��");
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
	
	if((READ_RFID[0] > '9') && (READ_RFID[2] > '9') && (READ_RFID[4] > '9')){			//�ж�����0��2��4λ�Ƿ�Ϊ��ĸ
		Send_InfoData_To_Fifo("[0][2][4]ok\n",13);
		if((READ_RFID[1] < '9') && (READ_RFID[3] < '9') && (READ_RFID[5] < '9')){		//�ж�����1��3��5λ�Ƿ�Ϊ����
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
����������λ��ʶ��TFT��ͼ��,��������Ҫ����ҳ
Wifi_Rx_Buf[2]�����Σ�Wifi_Rx_Buf[3]���Σ�Wifi_Rx_Buf[4]Բ��
Principal_Tab[];Ϊ����������λ����������
Wifi_Rx_Buf[];Ϊ����������λ����������
***********************************************************/
void Looking_for_graphics(unsigned char start)
{	

	unsigned char num          = 0;				
	unsigned char flag         = 1;
	unsigned int broadcast[1]  = {NULL};				  //��������
	
	Principal_Tab[10] = 0xa1;											// ʶ��ͼ��
	Send_WifiData_To_Fifo(Principal_Tab, 12);   
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	
	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
	
	while(flag){
		if(Wifi_Rx_Buf[start] != 0x1a){
			num += 1;
			
//			SYN_TTS("����ͼ��");
			Send_ZigbeeData_To_Fifo(TFT_PageDown,8);					// TFT�·�ҳ
			delay_ms(500);delay_ms(500);
			
			Principal_Tab[10] = 0xa1;									// ʶ��ͼ��
			Send_WifiData_To_Fifo(Principal_Tab, 12);   
			delay_ms(500);
			Principal_Tab[10] = 0x00;
			Send_WifiData_To_Fifo(Principal_Tab, 12);
			
			delay_ms(500);delay_ms(500);	
			flag = 1;
			
			if(num > 3){
//				SYN_TTS("����������Ϊһ");
//				SYN_TTS("��������Ϊ��");
//				SYN_TTS("Բ��������");
				
				TFT_Test_Zigbee('B',0x40,0xA2,0xD1,0xE2);
				delay_ms(200);
				TFT_Test_Zigbee('B',0x40,0xA2,0xD1,0xE2);
				SYN_TTS("ͼ��ʶ�����");
				flag = 0;
			}
		}else if(Wifi_Rx_Buf[start] == 0x1a){
			SYN_TTS("��ͼ��");
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TFT_PageUp,8);
//			TFT_Test_Zigbee(0x40,Wifi_Rx_Buf[(start + 1)],Wifi_Rx_Buf[(start + 2)],Wifi_Rx_Buf[(start + 3)]);
			
//			SYN_TTS("����������Ϊ");
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
//			SYN_TTS("��������Ϊ");
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
//			SYN_TTS("Բ������Ϊ");
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

			SYN_TTS("ͼ��ʶ�����");
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

	Principal_Tab[10] = 0xa1;							// ʶ����
	Send_WifiData_To_Fifo(Principal_Tab, 12);   
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);

	while(flag){
		if(Wifi_Rx_Buf[(start + 1)] != 0x1a){
			num += 1;
			SYN_TTS("����ͼ��");
			Send_ZigbeeData_To_Fifo(TFT_PageDown,8);					// TFT�·�ҳ
			
			Principal_Tab[10] = 0xa1;									// ʶ����
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
			SYN_TTS("��ͼ��");
			LED_Date_Zigbee((0xF0 + Wifi_Rx_Buf[(start + 2)]),(0xF0 + Wifi_Rx_Buf[(start + 3)]),(0xF0 + Wifi_Rx_Buf[(start + 4)]),0x02);
			delay_ms(200);
			LED_Date_Zigbee((0xF0 + Wifi_Rx_Buf[(start + 2)]),(0xF0 + Wifi_Rx_Buf[(start + 3)]),(0xF0 + Wifi_Rx_Buf[(start + 4)]),0x02);
			flag = 0;
		}
	}
	
}
/*********************************************
����������λ��ʶ��TFT�г���,��������Ҫ����ҳ
Principal_Tab[];Ϊ����������λ����������
Wifi_Rx_Buf[];Ϊ����������λ����������
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
	Principal_Tab[10] = 0xa4;							// ʶ����
	Send_WifiData_To_Fifo(Principal_Tab, 12);   
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 	
	
	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
	delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);	
	
	while(flag){
		if(Wifi_Rx_Buf[start] != 0x4a){
			num += 1;
			SYN_TTS("���ǳ���");
			Send_ZigbeeData_To_Fifo(TFT_PageDown,8);					// TFT�·�ҳ
			
			Principal_Tab[10] = 0xa4;									// ʶ����
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
					plates[i] =  Wifi_Rx_Buf[(start + 3) + i];			//���泵����Ϣ��test
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
//	delay_ms(500);delay_ms(500);										// �ȴ�ͼƬˢ��
//	
//	Principal_Tab[10] = 0xa4;											// ʶ����
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
//			Send_ZigbeeData_To_Fifo(TFT_PageDown,8);					// TFT�·�ҳ
//			Principal_Tab[10] = 0xa4;									// ʶ����
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
//	SYN_TTS("����ţ��");
}
/******************
������ά�����ݺ���
*******************/
void QR_broadcast(unsigned char start,unsigned char end)
{			
	unsigned char broadcast[1] = {NULL};				  //��������				
	unsigned char i;
		
	for(i = 0;i < WIFI_MAX_NUM;i++){
		Wifi_Rx_Buf[i] = broadcast[1];
	}
	
	delay_ms(500);delay_ms(500);
	Principal_Tab[10] = 0xa3;							 // ��ά��ʶ��
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 	
		
	SYN_TTS("��ӡ��ά����Ϣ");
	Send_InfoData_To_Fifo("Wifi_Rx_Buf:\n",14);	
	Send_InfoData_To_Fifo((char *)Wifi_Rx_Buf,20);		//��ӡ��debug����
	Send_InfoData_To_Fifo("\n",2);
	
	SYN_TTS("��ά����ϢΪ");

	for(i = start;i <= end;i++){
		qr_tset[i] = Wifi_Rx_Buf[i];
		delay_us(1);
	}
	
	for(i = start;i <= end;i++){
		broadcast[0] = Wifi_Rx_Buf[i];
		SYN_TTS((char *)broadcast);
	}
	

	
	Send_InfoData_To_Fifo("broadcast:\n",12);
	Send_InfoData_To_Fifo((char *)broadcast,1);			//��ӡ��debug����
	Send_InfoData_To_Fifo("\n",2);
	

//	SYN_TTS("���ϼ���");
}
/*******************
������ͨ��ʶ�� 
*******************/
void trafficA_Lights(unsigned char start)
{	
	
	unsigned char flag   = 0;
	unsigned char flag_2 = 1;
	unsigned char test   = 0x00;
	unsigned int i;
	
	for(i = 0;i < WIFI_MAX_NUM;i++){	//�������
		Wifi_Rx_Buf[i] = test;
		delay_us(1);
	}
	
	Principal_Tab[10] = 0xa2;								// ʶ��ͨ��,����ͷ̧ͷ
	Send_WifiData_To_Fifo(Principal_Tab, 12);   
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	
	Send_ZigbeeData_To_Fifo(TrafficA_Open,8);				// ����ʶ��ģʽ
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(TrafficA_Open,8);				// ����ʶ��ģʽ
	delay_ms(200);
	

	while(flag_2){
		for(i = start;i <= WIFI_MAX_NUM;i++){
			if((Wifi_Rx_Buf[i] == 0xC1) || (Wifi_Rx_Buf[i] == 0xC2) || (Wifi_Rx_Buf[i] == 0xC3)){	// ���
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
				SYN_TTS("ʶ����Ϊ��ɫ");
				Send_ZigbeeData_To_Fifo(TrafficA_Red,8);
				flag = 0;
			break;
			case 0xc2:
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Yellow,8);	
				SYN_TTS("ʶ����Ϊ��ɫ");
				Send_ZigbeeData_To_Fifo(TrafficA_Yellow,8);	
				flag = 0;
			break;
			case 0xc3:
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				SYN_TTS("ʶ����Ϊ��ɫ");
				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				flag = 0;
			break;
			default:
//				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				delay_ms(200);
//				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				SYN_TTS("ʶ����Ϊ���");

				flag = 1;
//				flag_2 = 1;
		}
	}
//	Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,start);
//	SYN_TTS("��ͨ��ʶ�����");
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
	
	Principal_Tab[10] = 0xa2;								// ʶ��ͨ��,����ͷ̧ͷ
	Send_WifiData_To_Fifo(Principal_Tab, 12);   
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	
	Send_ZigbeeData_To_Fifo(TrafficB_Open,8);				// ����ʶ��ģʽ
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(TrafficB_Open,8);				// ����ʶ��ģʽ
	delay_ms(200);
	
	while(flag_2){
		for(i = 0;i <= WIFI_MAX_NUM;i++){
			if(Wifi_Rx_Buf[i] == 0x1a){		
//				SYN_TTS("�ҵ�1a");
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
			SYN_TTS("ʶ����Ϊ��ɫ");
			Send_ZigbeeData_To_Fifo(TrafficB_Red,8);
			flag = 0;
		break;
		case 0xc2:
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Yellow,8);	
			SYN_TTS("ʶ����Ϊ��ɫ");
			Send_ZigbeeData_To_Fifo(TrafficB_Yellow,8);	
			flag = 0;
		break;
		case 0xc3:
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
			SYN_TTS("ʶ����Ϊ��ɫ");
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
			flag = 0;
		break;
		default:
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
			SYN_TTS("ʶ����Ϊ���");
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
			flag = 0;
		}
	}
//	Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,start);
	SYN_TTS("��ͨ��ʶ�����");
}
/*******************
�ӳ���ͨ��ʶ�� 
*******************/
void TrafficA_AGV(unsigned int start,unsigned char colour)
{	
	unsigned char AGV_Traffic[8] = 
	{0x55,0x02,0x99,0x00,0x00,0x00,(0x99+0x00+0x00+0x00)%256,0xBB};		// �ƶ������˽�ͨ�ƽ���
	unsigned char flag   = 0;
	unsigned char flag_2 = 1;
	unsigned char i;
	unsigned char null = 0x00;
	unsigned char num = 0;
	
	for(i = 0;i < ZIGB_RX_MAX;i++){
		Zigb_Rx_Buf[i] = null;
		delay_us(1);
	}
//	SYN_TTS("�������");
	
	while(flag_2){
		if((Zigb_Rx_Buf[start] == 0x55) && (Zigb_Rx_Buf[(start + 1)] == 0x02) && (Zigb_Rx_Buf[(start + 10)] == 0x01)){
			Send_ZigbeeData_To_Fifo(TrafficA_Open,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficA_Open,8);
//			SYN_TTS("�򿪽�ͨ��");
			flag_2 = 0;		
			flag   = 1;		
		}
		num += 1;
		if(num > 3){
			flag_2 = 0;
		}
		SYN_TTS("�ȴ�����");
	}
	
	while(flag){
		switch(colour){
			case 0x01:
				Send_ZigbeeData_To_Fifo(TrafficA_Red,8);
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Red,8);
				SYN_TTS("ʶ����Ϊ��ɫ");

				flag = 0;
			break;
			case 0x02:
				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Green,8);
				SYN_TTS("ʶ����Ϊ��ɫ");

				flag = 0;
			break;
			case 0x03:
				Send_ZigbeeData_To_Fifo(TrafficA_Yellow,8);
				delay_ms(200);
				Send_ZigbeeData_To_Fifo(TrafficA_Yellow,8);
				SYN_TTS("ʶ����Ϊ��ɫ");

				flag = 0;
			break;
		}
	}
	
//	SYN_TTS("�ӳ���ͨ��ʶ�����");
}
void TrafficB_AGV(unsigned int start,unsigned char colour)
{	
	unsigned char AGV_Traffic[8] = 
	{0x55,0x02,0x99,0x00,0x00,0x00,(0x99+0x00+0x00+0x00)%256,0xBB};		// �ƶ������˽�ͨ�ƽ���
	unsigned char flag = 0;
	unsigned char flag_2 = 1;
	
	while(flag_2){
		if(Zigb_Rx_Buf[start] == 0x01){
			Send_ZigbeeData_To_Fifo(TrafficB_Open,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Open,8);
			SYN_TTS("�򿪽�ͨ��");
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
//			SYN_TTS("ʶ����Ϊ��ɫ");

			flag = 0;
		break;
		case 0x02:
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Green,8);
//			SYN_TTS("ʶ����Ϊ��ɫ");

			flag = 0;
		break;
		case 0x03:
			Send_ZigbeeData_To_Fifo(TrafficB_Yellow,8);
			delay_ms(200);
			Send_ZigbeeData_To_Fifo(TrafficB_Yellow,8);
//			SYN_TTS("ʶ����Ϊ��ɫ");

			flag = 0;
		break;
		default:
			flag = 1;
		}
	}
	
//	Send_ZigbeeData_To_Fifo(AGV_Traffic,8);
//	delay_ms(200);
//	Send_ZigbeeData_To_Fifo(AGV_Traffic,8);
	
//	SYN_TTS("�ӳ���ͨ��ʶ�����");
}
/*******************
�������
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
//	SYN_TTS("�������");
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
//	SYN_TTS("�������⵵λ");
	while(garage){
		switch(Zigb_Rx_Buf[start]){
		case 0x01:
			GarageA(1);
			SYN_TTS("������Ϊ1��");
			garage = 0;
			
		break;
		case 0x02:
			GarageA(2);
			SYN_TTS("������Ϊ2��");
			garage = 0;
			
		break;
		case 0x03:
			GarageA(3);
			SYN_TTS("������Ϊ3��");
			garage = 0;
			
		break;
		case 0x04:
			GarageA(4);
			SYN_TTS("������Ϊ4��");
			garage = 0;
			
		break;
//		default:
//			garage = 1;
		}		
	}	
	SYN_TTS("����������");
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
			SYN_TTS("����λ�ڵ�һ��");
		break;
		case 0x02:
			SYN_TTS("����λ�ڵڶ���");
		break;
		case 0x03:
			SYN_TTS("����λ�ڵ�����");
		break;
		case 0x04:
			SYN_TTS("����λ�ڵ��Ĳ�");
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
		SYN_TTS("����λ�ڵ�һ��");
	break;
	case 0x02:
		SYN_TTS("����λ�ڵڶ���");
	break;
	case 0x03:
		SYN_TTS("����λ�ڵ�����");
	break;
	case 0x04:
		SYN_TTS("����λ�ڵ��Ĳ�");
	break;
	}
}

void RFID_Send(unsigned char *p,unsigned char len)
{
	Principal_Tab[10] = 0xa5;	//RFID׼������					 
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
	
	Principal_Tab[10] = 0xa6;	//RFID�дӳ���ʼλ��					 
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	delay_ms(500);
	SYN_TTS("����a6�˹�����");
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12);
	
	SYN_TTS("׼���������Ϸ�������Ϣ");
	switch(Wifi_Rx_Buf[num])
	{
		case 0x01:
			InitialPath[2] = 0xF1;
			SYN_TTS("λ����F1����ƫ����");
			Send_ZigbeeData_To_Fifo(InitialPath,8);	//���ʹӳ���ʼλ����Ϣ
		break;
		case 0x02:
			InitialPath[2] = 0xF2;
			SYN_TTS("λ����F2����ƫ����");
			Send_ZigbeeData_To_Fifo(InitialPath,8);	//���ʹӳ���ʼλ����Ϣ
		break;
		case 0x03:
			InitialPath[2] = 0xF3;
			SYN_TTS("λ����F3����ƫ����");
			Send_ZigbeeData_To_Fifo(InitialPath,8);	//���ʹӳ���ʼλ����Ϣ
		break;
	}
	while((Wifi_Rx_Buf[num] != 0x01) && (Wifi_Rx_Buf[num] != 0x02) && (Wifi_Rx_Buf[num] != 0x03));
	SYN_TTS("�����ܳ�ʼλ��ʶ�������");
//	Send_ZigbeeData_To_Fifo(InitialPath,8);	//���ʹӳ���ʼλ����Ϣ
}
void RFID_AGVPath()
{
	Principal_Tab[10] = 0xa7;	//RFID�дӳ�·��					 
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	delay_ms(500);
	Principal_Tab[10] = 0x00;
	Send_WifiData_To_Fifo(Principal_Tab, 12); 
	
}

/*********************************************************************************
 * �� �� �� ��	������Android�ն˷�������������
 * ��    �� ��	Pri  -> ��ָ��
				Sec  -> ��ְ��
				�����¼1����¼2
 * �� �� ֵ ��	��
 * ��    �� ��	Send_Android(0xA1, 0x01);		// ��������ͷԤ��λ1


��¼1��������Android�ն˷��͵�������ָ��ṹ
--------------------------------------------------------
| ��ָ�� | ��ָ�� |   ˵��
|--------|--------|-------------------------------------
|  0xA0  |  0x00  | ����ȫ�Զ�
|--------|--------|-------------------------------------
|  0xA1  |  0x01  | ��������ͷԤ��Ϊ1
|        |  0x02  | ��������ͷԤ��Ϊ2
|        |  0x03  | ��������ͷԤ��Ϊ3
|        |  0x04  | ��������ͷԤ��Ϊ4
|        |  0x05  | ��������ͷԤ��Ϊ5
|--------|--------|-------------------------------------
|  0xA2  |  0x00  | ���ö�ά��ʶ��
|--------|--------|-------------------------------------
|  0xA3  |  0x00  | ���ý�ͨ��ʶ��
|--------|--------|-------------------------------------
|  0xA4  |  0x00  | ���ó���ʶ��
|--------|--------|-------------------------------------
|  0xA4  |  0x00  | ����ͼ��ʶ��
--------------------------------------------------------

��¼2��Android�ն�����������ʶ����ָ��ṹ
-------------------------------------------------------------------------------
| ��ָ�� | ��ָ��[1] | ��ָ��[2] | ��ָ��[3] |		˵��
|--------|-----------|-----------|-----------|---------------------------------
|  0xA2  |   0x00    |   0x00    |   0x00	 | ��ά��ʶ��ɹ�
|--------|-----------|-----------|-----------|---------------------------------
|  0xA3  |   0x01    |   0x00    |   0x00	 | ��ͨ��ʶ��ɹ�����ɫ��
|        |   0x02    |   0x00    |   0x00	 | ��ͨ��ʶ��ɹ�����ɫ��
|        |   0x03    |   0x00    |   0x00	 | ��ͨ��ʶ��ɹ�����ɫ��
|--------|-----------|-----------|-----------|---------------------------------
|  0xA4  |   0xXX    |   0xXX    |   0xXX	 | ����ʶ��ɹ� ����ǰ��λASICC
|--------|-----------|-----------|-----------|---------------------------------
|  0xA5  |   0xXX    |   0xXX    |   0xXX	 | ����ʶ��ɹ� ���ƺ���λASICC
|--------|-----------|-----------|-----------|---------------------------------
|  0xA6  |   0xXX    |   0xXX    |   0xXX	 | ͼ��ʶ�� 
|        |�������Σ� | ��Բ�Σ�  | �����Σ�  | 
-------------------------------------------------------------------------------
*********************************************************************************/
void Send_Android(uint8_t Pri,uint8_t Sec)
{
	Principal_Tab[0] = 0x55;
	Principal_Tab[1] = 0xAA;
	Principal_Tab[2] = Pri;
	Principal_Tab[3] = Sec;
	Send_WifiData_To_Fifo(Principal_Tab, 4);   // ͨ��Wifi�ϴ���������
	UartA72_TxClear();
	UartA72_TxAddStr(Principal_Tab, 4);        // ͨ�������ϴ���������
	UartA72_TxStart();
}


/********************************************************************************
 * �� �� �� ��  TFT��ʾ��־�����ָ��
 * ��    �� ��  Device -> ѡ���豸
				Pri	   -> ��ָ��
                Sec1   -> ��ְ�1��
				Sec2   -> ��ְ�2��
				Sec3   -> ��ְ�3��
                �����¼1
 * �� �� ֵ ��  ��
 * ��    �� ��  TFT_Test_Zigbee('A',0x40,0xA1,0xB2,0xC3);    // TFT��ʾ����ʾͼ����Ϣ

��¼1��
--------------------------------------------------------------------------------
| ��ָ�� | ��ָ��[1] | ��ָ��[2] | ��ָ��[3] |			˵��
|--------|-----------|-----------|-----------|-----------------------------------
|  0x10  |	 0x00	 | 0x01~0x20 |   0x00    | �ɵڶ���ָ��ָ����ʾ����ͼƬ
|        |	 0x01    |	 0x00	 |   0x00    | ͼƬ���Ϸ�ҳ
|        |   0x02    |	 0x00	 |   0x00    | ͼƬ���·�ҳ
|		 |   0x03    |	 0x00    |   0x00    | ͼƬ�Զ����·�ҳ��ʾ�����ʱ�� 10S
|--------|-----------|-----------|-----------|-----------------------------------
|  0x20  |	 0xXX    |	 0xXX	 |   0xXX	 | ����ǰ��λ���ݣ�ASCII��
|--------|-----------|-----------|-----------|-----------------------------------
|  0x21  |	 0xXX	 |	 0xXX	 |   0xXX	 | ���ƺ���λ���ݣ�ASCII��
|--------|-----------|-----------|-----------|-----------------------------------
|  0x30  |	 0x00	 |	 0x00	 |   0x00	 | ��ʱģʽ�ر�
|	 	 |	 0x01	 |	 0x00 	 |   0x00	 | ��ʱģʽ��
| 		 |	 0x02	 |	 0x00	 |   0x00	 | ��ʱģʽ����
|--------|-----------|-----------|-----------|-----------------------------------
|  0x40  |	 0xXX	 |	 0xXX    |   0xXX	 | ��λ��ʾ���ݣ�HEX��ʽ��
|--------|-----------|-----------|-----------|-----------------------------------
|  0x50  |	 0x00	 |	 0x0X	 |   0xXX	 | ������ʾģʽ��ʮ���ƣ�
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
 * �� �� �� ��  ������ʾ��־����ʾ��������
 * ��    �� ��  *src    �������ݣ�ASICC��
                x,y     ������Ϣ
 * �� �� ֵ ��  ��
 * ��    �� ��  Rotate_show_Inf("A123B4",'C','5');
**********************************************************************/
void Rotate_show_Inf(char* src, char x, char y)
{
    Infrared[0] = 0xFF;			// ��ʼλ
    Infrared[1] = 0x20;			// ģʽ
    Infrared[2] = *(src + 0);	// ���ݡ�1��
    Infrared[3] = *(src + 1);	// ���ݡ�2��
    Infrared[4] = *(src + 2);	// ���ݡ�3��
    Infrared[5] = *(src + 3);	// ���ݡ�4��
    Infrared_Send(Infrared, 6);
    delay_ms(500);
    Infrared[1] = 0x10;			// ģʽ
    Infrared[2] = *(src + 4);	// ���ݡ�1��
    Infrared[3] = *(src + 5);	// ���ݡ�2��
    Infrared[4] = x;			// ���ݡ�3��
    Infrared[5] = y;			// ���ݡ�4��
    Infrared_Send(Infrared, 6);
    delay_ms(10);
}

/**********************************************************************
 * �� �� �� ��  ������ʾ��־����ʾ������Ϣ����λ��ms��
 * ��    �� ��  dis  �����Ϣ���������룩
 * �� �� ֵ ��  ��
 * ��    �� ��  Rotate_Dis_Inf(123);
**********************************************************************/
void Rotate_Dis_Inf(uint16_t dis)
{
    uint16_t csb = dis; //���泬��������ֵ

    csb += 5;   //��������
    Infrared[0] = 0xFF;
    Infrared[1] = 0x11; //��ʾ����ģʽ
    Infrared[2] = 0x30 + (uint8_t)(csb / 100 % 10); //����ʮλ--cm
    Infrared[3] = 0x30 + (uint8_t)(csb / 10 % 10); //�����λ--cm
    Infrared[4] = 0x00;
    Infrared[5] = 0x00;
    Infrared_Send(Infrared, 6);
    delay_ms(100);
}

/**********************************************************************
 * �� �� �� ��  ����TFT��ʾ����ʾ������Ϣ
 * ��    �� ��  dis	����ֵ
 * �� �� ֵ ��  ��
 * ��    �� ��  TFT_Dis_Zigbee('A',123);
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
	Send_ZigbeeData_To_Fifo(Zigbee, 8);  //����Zigbee����
	delay_ms(100);
}


/**********************************************************************
 * �� �� �� ��  TFT��ʾ����ʾ����
 * ��    �� ��  *Licence -> �������ݣ�ASICC��
 * �� �� ֵ ��  ��
 * ��    �� ��  TFT_Show_Zigbee('A',"A123B4");
**********************************************************************/
void TFT_Show_Zigbee(char Device,char *Licence)
{
    Zigbee[0] = 0x55;
    if (Device == 'B') { Zigbee[1] = 0x08; }
	else { Zigbee[1] = 0x0B; }
    Zigbee[2] = 0x20;
    Zigbee[3] = *(Licence + 0);     // �������ݡ�1��
    Zigbee[4] = *(Licence + 1);     // �������ݡ�2��
    Zigbee[5] = *(Licence + 2);     // �������ݡ�3��
    Zigbee[6] = (Zigbee[2] + Zigbee[3] + Zigbee[4] + Zigbee[5]) % 256;
    Zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Zigbee, 8);     // ����Zigbee����
    delay_ms(500);
    Zigbee[2] = 0x21;
    Zigbee[3] = *(Licence + 3);     // �������ݡ�4��
    Zigbee[4] = *(Licence + 4);     // �������ݡ�5��
    Zigbee[5] = *(Licence + 5);     // �������ݡ�6��
    Zigbee[6] = (Zigbee[2] + Zigbee[3] + Zigbee[4] + Zigbee[5]) % 256;
    Zigbee[7] = 0xBB;
    Send_ZigbeeData_To_Fifo(Zigbee, 8);     // ����Zigbee����
    delay_ms(100);
}

/*****************************************************************
 * �� �� �� ��	ETCϵͳ���
 * ��    �� ��	��
 * �� �� ֵ ��	��
 * ��     ����	ETC_Get_Zigbee();	// ETCϵͳ���
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
				Car_Back(40, 100);	// ����
				delay_ms(100);
				Car_Go(40, 110);	//С��ǰ��
			}
			else
			{
				SYN_TTS("ETCϵͳʶ��ʧ�ܣ�ǿ��ͨ��");
				break;
			}
		}
		
		if(Zigbee_Rx_flag == 1)	 // zigbee����
		{
			Zigbee_Rx_flag = 0;
			delay_us(5);
			if(Zigb_Rx_Buf[1] == 0x0C)		// ETCϵͳ
			{
				if(Zigb_Rx_Buf[2] == 0x01)
				{
					if(Zigb_Rx_Buf[3] == 0x01)
					{
						Stop_Flag = Zigb_Rx_Buf[4];	  // ETCբ�ſ����ɹ�
					}
				}
			}
		}
	}
}
/**********************************************************************
 * �� �� �� ��  ��������������־�ﲥ��ָ���ı���Ϣ
 * ��    �� ��  *p  --> ��Ҫ���͵�����
 * �� �� ֵ ��  ��
 * ��    �� ��  YY_Play_Zigbee("������ӭ��");
**********************************************************************/
void YY_Play_Zigbee(char *p)
{
    uint16_t p_len = strlen(p);             // �ı�����

    YY_Init[1] = 0xff & ((p_len + 2) >> 8); // ���������ȸ߰�λ
    YY_Init[2] = 0xff & (p_len + 2);        // ���������ȵͰ�λ
    Send_ZigbeeData_To_Fifo(YY_Init, 5);
    Send_ZigbeeData_To_Fifo((uint8_t *)p, p_len);
    delay_ms(100);
	
	
}

/**********************************************************************
 * �� �� �� ��  ��������������־�ﲥ��������������
 * ��    �� ��  Primary   -> ��ָ��
                Secondary -> ��ְ��
                �����¼1
 * �� �� ֵ ��  ��
 * ��    �� ��  YY_Comm_Zigbee(0x20, 0x01);     // �������������������

��¼1��
-----------------------------------------------------------------------
| Primary | Secondary | ˵��
|---------|-----------|------------------------------------------------
|  0x10   |  0x02     | ����ת��
|         |  0x03     | ��ֹ��ת
|         |  0x04     | �����ʻ
|         |  0x05     | ���б���
|         |  0x06     | ԭ�ص�ͷ
|---------|-----------|------------------------------------------------
|  0x20   |  0x01     | ���ָ��
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























