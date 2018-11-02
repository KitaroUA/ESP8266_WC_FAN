 #include "driver\IIC_mpr121.h"


#define HI(X) (X>>8)
#define LO(X) (X & 0xFF)


uint8 i2c_mpr121_buffer[2];
uint8 i2c_mpr121_Address;
uint8 i2c_mpr121_Init_Stage=0;
uint8 i2c_mpr121_Addresses[8];
uint8 i2c_mpr121_count;
uint8 i2c_mpr121_Flags = 0;	// 0bxxx1xxxxx - ���������� ����������� ������ ������
							// 0bx1xxxxxxx - ��� �������
							
BOOL touchStates[12];
uint8  touchTime[12];


//void DoNothing(void)	// ������� ��������, �������� �������������� ������
//{
//}



void i2c_mpr121_init_start_sending (void);
void i2c_mpr121_init_sended (void);
void i2c_mpr121_Write1(void);
//void i2c_mpr121_Readed (void);
//void i2c_mpr121_reenable_int (void);
//void i2c_mpr121_pres_time (void);


os_timer_t mpr121_timer;

uint8 mpr121_init_array [][2]=
	{
			{MPR121_ELE_CFG,			0x00},
			{MPR121_MHD_R,				0x01},	// Section A - Controls filtering when data is > baseline.
			{MPR121_NHD_R,				0x01},
			{MPR121_NCL_R,				0x00},
			{MPR121_FDL_R,				0x00},
			{MPR121_MHD_F,				0x01},// Section B - Controls filtering when data is < baseline.
			{MPR121_NHD_F,				0x01},
			{MPR121_NCL_F,				0xFF},
			{MPR121_FDL_F,  			0x02},
			{MPR121_ELE0_T,MPR121_TOU_THRESH},// Section C - Sets touch and release thresholds for each electrode
			{MPR121_ELE0_R,MPR121_REL_THRESH},
			{MPR121_ELE1_T,MPR121_TOU_THRESH},
			{MPR121_ELE1_R,MPR121_REL_THRESH},
			{MPR121_ELE2_T,MPR121_TOU_THRESH},
			{MPR121_ELE2_R,MPR121_REL_THRESH},
			{MPR121_ELE3_T,MPR121_TOU_THRESH},
			{MPR121_ELE3_R,MPR121_REL_THRESH},
			{MPR121_ELE4_T,MPR121_TOU_THRESH},
			{MPR121_ELE4_R,MPR121_REL_THRESH},
			{MPR121_ELE5_T,MPR121_TOU_THRESH},
			{MPR121_ELE5_R,MPR121_REL_THRESH},
			{MPR121_ELE6_T,MPR121_TOU_THRESH},
			{MPR121_ELE6_R,MPR121_REL_THRESH},
			{MPR121_ELE7_T,MPR121_TOU_THRESH},
			{MPR121_ELE7_R,MPR121_REL_THRESH},
			{MPR121_ELE8_T,MPR121_TOU_THRESH},
			{MPR121_ELE8_R,MPR121_REL_THRESH},
			{MPR121_ELE9_T,MPR121_TOU_THRESH},
			{MPR121_ELE9_R,MPR121_REL_THRESH},
			{MPR121_ELE10_T,MPR121_TOU_THRESH},
			{MPR121_ELE10_R,MPR121_REL_THRESH},
			{MPR121_ELE11_T,MPR121_TOU_THRESH},
			{MPR121_ELE11_R,MPR121_REL_THRESH},
			{MPR121_FIL_CFG,			0x04},  // Section D, Set the Filter Configuration, Set ESI2
			{MPR121_ELE_CFG,			0x0C},  // Section E, Electrode Configuration, Set ELE_CFG to 0x00 to return to standby mode
			{MPR121_ELE_CFG,			0x0C}  // Section F, Enable Auto Config and auto Reconfig
												/*set_register(0x5A, ATO_CFG0, 0x0B);
			  	  	  	  	  	  	  	  	  	  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
			  	  	  	  	  	  	  	  	  	  set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V





	};





/*
uint8 ICACHE_FLASH_ATTR i2c_mpr121_WriteByte(uint8 SAddr, uint8 Byte[], uint8 ByteNumbers, IIC_F WhatDo)
{

uint8 i;

if (i2c_Do & i2c_Busy) return 0;

i2c_index = 0;
i2c_ByteCount = 2;

i2c_SlaveAddress = SAddr;

for (i=0;i<2;i++)
{
	i2c_Buffer[i]=Byte[i];
}


i2c_Do = i2c_sawp;

MasterOutFunc = WhatDo;
ErrorOutFunc = WhatDo;

TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;

i2c_Do |= i2c_Busy;

return 1;
}*/























void  ICACHE_FLASH_ATTR i2c_mpr121_Write(uint8 SAddr, uint8 Byte[], uint8 ByteNumbers)
{
	uint8 i=0;
	i2c_mpr121_Address = SAddr;
		for (i=0;i<ByteNumbers;i++)
		{
			i2c_mpr121_buffer[i]=Byte[i];
		}
	i2c_mpr121_buffer_data = ByteNumbers;

	i2c_mpr121_Write1();
}


void  ICACHE_FLASH_ATTR  i2c_mpr121_Write1Byte(uint8 SAddr, uint8 Byte)
{
	i2c_mpr121_Address = SAddr;

	i2c_mpr121_buffer[0]=Byte;
	i2c_mpr121_buffer_data = 1;
	i2c_mpr121_Write1();
}



void  ICACHE_FLASH_ATTR i2c_mpr121_Writed(void)               		// ���� ������� ������
{
	i2c_Do &= i2c_Free;                  		// ����������� ����

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
	{
		SetTimerTask(mpr121_timer,i2c_mpr121_Write1,i2c_mpr121_Init_Error_Restart,0);
	}
}




void  ICACHE_FLASH_ATTR i2c_mpr121_Write1(void)
{
	if (!i2c_Write_Buffer(i2c_mpr121_Address_1, i2c_mpr121_buffer, i2c_mpr121_buffer_data, i2c_mpr121_Writed) )      // ������
	{
		SetTimerTask(mpr121_timer,i2c_mpr121_Write1,i2c_mpr121_Init_Error_Restart,0);
	}

}













































uint8   ICACHE_FLASH_ATTR i2c_mpr121_ReadByte(uint8 SAddr, IIC_F WhatDo)
{
	/*
	if (i2c_Do & i2c_Busy) return 0;

	i2c_index = 0;
	i2c_ByteCount = 2;

	i2c_SlaveAddress = SAddr;


	i2c_PageAddrIndex = 0;
	i2c_PageAddrCount = 0;

	i2c_Do = i2c_sarp;

	MasterOutFunc = WhatDo;
	ErrorOutFunc = WhatDo;

	TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;

	i2c_Do |= i2c_Busy;
*/
	return 1;
}


void   ICACHE_FLASH_ATTR i2c_mpr121_Init(void)
{


	if (i2c_mpr121_Init_Stage >= 36) return;
	INFO("\r\nMPR121 init stage %d",i2c_mpr121_Init_Stage);
	i2c_mpr121_buffer[0]=mpr121_init_array[i2c_mpr121_Init_Stage][0];
	i2c_mpr121_buffer[1]=mpr121_init_array[i2c_mpr121_Init_Stage][1];


	SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
	i2c_mpr121_Init_Stage ++;
/*

	switch (i2c_mpr121_Init_Stage)
	{
		case 0:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE_CFG;
			i2c_mpr121_buffer[1]=0x00;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 1:// Section A - Controls filtering when data is > baseline.
		{
			i2c_mpr121_buffer[0]=MPR121_MHD_R;
			i2c_mpr121_buffer[1]=0x01;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 2:
		{
			i2c_mpr121_buffer[0]=MPR121_NHD_R;
			i2c_mpr121_buffer[1]=0x01;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 3:
		{
			i2c_mpr121_buffer[0]=MPR121_NCL_R;
			i2c_mpr121_buffer[1]=0x00;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 4:
		{
			i2c_mpr121_buffer[0]=MPR121_FDL_R;
			i2c_mpr121_buffer[1]=0x00;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 5:// Section B - Controls filtering when data is < baseline.
		{
			i2c_mpr121_buffer[0]=MPR121_MHD_F;
			i2c_mpr121_buffer[1]=0x01;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 6:
		{
			i2c_mpr121_buffer[0]=MPR121_NHD_F;
			i2c_mpr121_buffer[1]=0x01;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 7:
		{
			i2c_mpr121_buffer[0]=MPR121_NCL_F;
			i2c_mpr121_buffer[1]=0xFF;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 8:
		{
			i2c_mpr121_buffer[0]=MPR121_FDL_F;
			i2c_mpr121_buffer[1]=0x02;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 9:// Section C - Sets touch and release thresholds for each electrode
		{
			i2c_mpr121_buffer[0]=MPR121_ELE0_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 10:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE0_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 11:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE1_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 12:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE1_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 13:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE2_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 14:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE2_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 15:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE3_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 16:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE3_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 17:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE4_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 18:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE4_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 19:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE5_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 20:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE5_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 21:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE6_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 22:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE6_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 23:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE7_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 24:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE7_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 25:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE8_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 26:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE8_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 27:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE9_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 28:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE9_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 29:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE10_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 30:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE10_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 31:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE11_T;
			i2c_mpr121_buffer[1]=MPR121_TOU_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
		case 32:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE11_R;
			i2c_mpr121_buffer[1]=MPR121_REL_THRESH;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
  // Section D
  // Set the Filter Configuration
  // Set ESI2
		case 33:
		{
			i2c_mpr121_buffer[0]=MPR121_FIL_CFG;
			i2c_mpr121_buffer[1]=0x04;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
		case 34:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE_CFG;
			i2c_mpr121_buffer[1]=0x0C;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
  // Section F
  // Enable Auto Config and auto Reconfig
  //set_register(0x5A, ATO_CFG0, 0x0B);
  //set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  //set_register(0x5A, ATO_CFGT, 0xB5);  // Target = 0.9*USL = 0xB5 @3.3V
		case 35:
		{
			i2c_mpr121_buffer[0]=MPR121_ELE_CFG;
			i2c_mpr121_buffer[1]=0x0C;
			
			SetTimerTask(mpr121_timer,i2c_mpr121_init_start_sending,1,0); //			SetTimerTask(i2c_mpr121_init_start_sending,1);
			i2c_mpr121_Init_Stage ++;
			break;
		}
			
	
	
	
	}*/

}



void   ICACHE_FLASH_ATTR i2c_mpr121_init_start_sending (void)
{
//		if (!i2c_mpr121_WriteByte(i2c_mpr121_Address,i2c_mpr121_buffer,&i2c_mpr121_init_sended))	// ���� ���� �� ���������
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

/*

void i2c_mpr121_Start_Reading (void)
{
	if (!i2c_mpr121_ReadByte(i2c_mpr121_Address,  i2c_mpr121_Readed) )      // ������
	{
		SetTimerTask(i2c_mpr121_Readed,i2c_mpr121_Read_Restart);                  		// ���� ������� �� ����� (���� ������), �� ������ ����� 50��.
	}
	
}

void i2c_mpr121_Readed (void)
{
	i2c_Do &= i2c_Free;                  		// ����������� ����
	
	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
	{
		SetTimerTask(i2c_mpr121_Start_Reading,i2c_mpr121_Read_Restart);     		 // ����� ������
	}
	else
	{
		    uint8 LSB = i2c_Buffer[0];
		    uint8 MSB = i2c_Buffer[1];
			uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states
			
			
		for (int i=0; i < 12; i++){  // Check what electrodes were pressed
			if(touched & (1<<i)){

				if(touchStates[i] == 0){
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
		
			SetTimerTask(i2c_mpr121_reenable_int,i2c_mpr121_Reenable_delay);
			SetTimerTask(i2c_mpr121_pres_time, 1);	//	������ ���������, ��� ����� ���������, ������ ��������� ������
		
			//	��� ������ ���������, �� ���� ��������� �������
			if ( ((touchStates[stepper_inc_pad])|(touchStates[stepper_dec_pad])) == 1)
			{
				
			}

	}
	
	
}*/
/*
void i2c_mpr121_reenable_int (void)
{
			EIMSK |= 1<<INT1;	// ����������� � ���� ���������, ����� ��������� ����
}
*/



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


