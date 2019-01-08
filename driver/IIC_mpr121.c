 #include "driver\IIC_mpr121.h"


#define HI(X) (X>>8)
#define LO(X) (X & 0xFF)


uint8 i2c_mpr121_buffer[2];
uint8 i2c_mpr121_Address;
uint8 i2c_mpr121_Init_Stage=0;
uint8 i2c_mpr121_Addresses[8];
uint8 i2c_mpr121_count;
//uint8 i2c_mpr121_Flags = 0;	// 0bxxx1xxxxx - ���������� ����������� ������ ������
							// 0bx1xxxxxxx - ��� �������
							
BOOL touchStates[12];
uint8  touchTime[12];


//void DoNothing(void)	// ������� ��������, �������� �������������� ������
//{
//}



void i2c_mpr121_init_start_sending (void);
void i2c_mpr121_init_sended (void);
void i2c_mpr121_Write(void);

os_timer_t mpr121_timer;

uint8 mpr121_init_array [][2]=
	{
			{MPR121_ELE_CFG,			 0x00},
			{MPR121_MHD_R,				 0x01},	// Section A - Controls filtering when data is > baseline.
			{MPR121_NHD_R,				 0x01},
			{MPR121_NCL_R,				 0x00},
			{MPR121_FDL_R,				 0x00},
			{MPR121_MHD_F,				 0x01},// Section B - Controls filtering when data is < baseline.
			{MPR121_NHD_F,				 0x01},
			{MPR121_NCL_F,				 0xFF},
			{MPR121_FDL_F,  			 0x02},
			{MPR121_ELE0_T, MPR121_TOU_THRESH},// Section C - Sets touch and release thresholds for each electrode
			{MPR121_ELE0_R, MPR121_REL_THRESH},
			{MPR121_ELE1_T, MPR121_TOU_THRESH},
			{MPR121_ELE1_R, MPR121_REL_THRESH},
			{MPR121_ELE2_T, MPR121_TOU_THRESH},
			{MPR121_ELE2_R, MPR121_REL_THRESH},
			{MPR121_ELE3_T, MPR121_TOU_THRESH},
			{MPR121_ELE3_R, MPR121_REL_THRESH},
			{MPR121_ELE4_T, MPR121_TOU_THRESH},
			{MPR121_ELE4_R, MPR121_REL_THRESH},
			{MPR121_ELE5_T, MPR121_TOU_THRESH},
			{MPR121_ELE5_R, MPR121_REL_THRESH},
			{MPR121_ELE6_T, MPR121_TOU_THRESH},
			{MPR121_ELE6_R, MPR121_REL_THRESH},
			{MPR121_ELE7_T, MPR121_TOU_THRESH},
			{MPR121_ELE7_R, MPR121_REL_THRESH},
			{MPR121_ELE8_T, MPR121_TOU_THRESH},
			{MPR121_ELE8_R, MPR121_REL_THRESH},
			{MPR121_ELE9_T, MPR121_TOU_THRESH},
			{MPR121_ELE9_R, MPR121_REL_THRESH},
			{MPR121_ELE10_T,MPR121_TOU_THRESH},
			{MPR121_ELE10_R,MPR121_REL_THRESH},
			{MPR121_ELE11_T,MPR121_TOU_THRESH},
			{MPR121_ELE11_R,MPR121_REL_THRESH},
			{MPR121_FIL_CFG,			 0x04},  // Section D, Set the Filter Configuration, Set ESI2
			{MPR121_ELE_CFG,			 0x0C},  // Section E, Electrode Configuration, Set ELE_CFG to 0x00 to return to standby mode
			{MPR121_ELE_CFG,			 0x0C}  // Section F, Enable Auto Config and auto Reconfig
												/*set_register(0x5A, ATO_CFG0, 0x0B);
			  	  	  	  	  	  	  	  	  	  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
			  	  	  	  	  	  	  	  	  	  set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V
	};

void  ICACHE_FLASH_ATTR i2c_mpr121_Writed(void)               		// ���� ������� ������
{
	i2c_Do &= i2c_Free;                  		// ����������� ����

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
	{
		SetTimerTask(mpr121_timer,i2c_mpr121_Write,i2c_mpr121_Init_Error_Restart,0);
	}
}

void  ICACHE_FLASH_ATTR i2c_mpr121_Write(void)
{
	if (!i2c_Write_Buffer(i2c_mpr121_Address_1, i2c_mpr121_buffer, i2c_mpr121_buffer_data, i2c_mpr121_Writed) )      // ������
	{
		SetTimerTask(mpr121_timer,i2c_mpr121_Write,i2c_mpr121_Init_Error_Restart,0);
	}

}

void   ICACHE_FLASH_ATTR i2c_mpr121_Init(void)
{


	if (i2c_mpr121_Init_Stage >= 36) return;  // MPR initialisation complited

#ifdef mpr_debug
	INFO("\r\nMPR121 init stage %d",i2c_mpr121_Init_Stage);
#endif
	i2c_mpr121_buffer[0]=mpr121_init_array[i2c_mpr121_Init_Stage][0];
	i2c_mpr121_buffer[1]=mpr121_init_array[i2c_mpr121_Init_Stage][1];


	SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
	i2c_mpr121_Init_Stage ++;

}



void   ICACHE_FLASH_ATTR i2c_mpr121_init_start_sending (void)
{
		if (!i2c_Write_Buffer(i2c_mpr121_Address_1, i2c_mpr121_buffer, 2, i2c_mpr121_init_start_sending) )
				{
					SetTimerTask(mpr121_timer,i2c_mpr121_init_sended,i2c_mpr121_Init_Restart,0);			// ��������� ������� ����� 50��
				}
	
	
	
}


void   ICACHE_FLASH_ATTR i2c_mpr121_init_sended (void)
{
	i2c_Do &= i2c_Free;			// ����������� ����
	
	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))	// ���� ������ �� �������
	{
		SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,i2c_mpr121_Init_Error_Restart,0);	// ��������� �������
	}
	else
	{
		SetTimerTask(mpr121_timer,i2c_mpr121_Init,1,0);
	}

}


void i2c_mpr121_Readed (void);
void ICACHE_FLASH_ATTR i2c_mpr121_Start_Reading (void)
{
#ifdef mpr_debug
	INFO("\r\nMPR121 start reading\r\n");
#endif

	if (!i2c_Read_Buffer(i2c_mpr121_Address_1, 2, i2c_mpr121_Readed) )      // ������
	{
		SetTimerTask(mpr121_timer,i2c_mpr121_Start_Reading,i2c_mpr121_Read_Restart,0);
#ifdef mpr_debug
		INFO("\r\nMPR121 start reading error\r\n");
#endif
	}
}


void i2c_mpr121_reenable_int (void);
void ICACHE_FLASH_ATTR i2c_mpr121_Readed (void)
{
#ifdef mpr_debug
	INFO("\r\nMPR121 start readed in\r\n");
#endif

	i2c_Do &= i2c_Free;                  		// ����������� ����
	
	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
	{
//		SetTimerTask(             i2c_mpr121_Start_Reading,i2c_mpr121_Read_Restart);     		 // ����� ������
		SetTimerTask(mpr121_timer,i2c_mpr121_Start_Reading,i2c_mpr121_Read_Restart,0);
#ifdef mpr_debug
		INFO("\r\nMPR121 start readed error\r\n");
#endif


	}
	else
	{

		gpio_intr_deattach(12);

#ifdef mpr_debug
		INFO("\r\nMPR121 analise readed data in\r\n");
#endif
		    uint8 LSB = i2c_Buffer[0];
		    uint8 MSB = i2c_Buffer[1];
			uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states
			
			
		for (int i=0; i < 12; i++){  // Check what electrodes were pressed
			if(touched & (1<<i)){

				if(touchStates[i] == 0){
					INFO("\r\npin %.02d was just touched\n\r",  i);
					//pin i was just touched
// 									char cj[50];
// 									sprintf(cj,"pin %.02i was just touched\n\r",  i);
// 									SendStr(cj);
//									i2c_LCD_Send_String(3,i,"T");						//	������ ����� ����� - �� ��������, ���� ����� ���� ��� � USART, ��� �� �����.

					}else if(touchStates[i] == 1){
					//pin i is still being touched
				}

				touchStates[i] = 1;
				}else{
				if(touchStates[i] == 1){
					INFO("\r\npin %.02d is no longer being touched\n\r",  i);

					
// 									char cj[50];
// 									sprintf(cj,"pin %.02i is no longer being touched\n\r",  i);
// 									SendStr(cj);
//									i2c_LCD_Send_String(3,i,"U");

					//pin i is no longer being touched
				}

				touchStates[i] = 0;
				touchTime[i] = 0;
			}

		}
		
			SetTimerTask(mpr121_timer_int,i2c_mpr121_reenable_int,i2c_mpr121_Reenable_delay,0);

//			SetTimerTask(i2c_mpr121_pres_time, 1);	//	������ ���������, ��� ����� ���������, ������ ��������� ������
		
			//	��� ������ ���������, �� ���� ��������� �������

	}
	
	
}


extern void ICACHE_FLASH_ATTR intr_callback(unsigned pin, unsigned level);


void ICACHE_FLASH_ATTR i2c_mpr121_reenable_int (void)
{
	gpio_intr_init(12,GPIO_PIN_INTR_NEGEDGE);
	gpio_intr_attach(intr_callback);
	INFO("\r\nint reenabled\r\n");
}




/*
void i2c_mpr121_pres_time (void)
{
	for (int i=0; i < 12; i++)
	{
		if(touchStates[i] == 1)
		{
			touchTime[i]++;
			if (touchTime[i] >= 254)
			{
				touchTime[i] = 253;
			}
		}
		else
		{
			touchTime[i] = 0;
		}
	}
	SetTimerTask(i2c_mpr121_pres_time, i2c_mpr121_push_time_stage);
}*/


