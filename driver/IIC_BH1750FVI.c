
 #include "driver/IIC_BH1750FVI.h"

#define HI(X) (X>>8)
#define LO(X) (X & 0xFF)


uint8 i2c_BH1750FVI_buffer[i2c_BH1750FVI_buffer_lenght];

uint8 i2c_BH1750FVI_Address;
//uint8 i2c_BH1750FVI_Addresses[8];
uint8 i2c_BH1750FVI_count;
uint16_t i2c_BH1750FVI_Illuminance;
uint8 i2c_BH1750FVI_L;
uint8 i2c_BH1750FVI_H;


//void DoNothing(void)	// ������� ��������, �������� �������������� ������
//{
//}

/*
uint8 i2c_BH1750FVI_WriteByte(uint8 SAddr, uint8 Byte[], uint8 ByteNumbers, IIC_F WhatDo)
{

uint8 i;

if (i2c_Do & i2c_Busy) return 0;

i2c_index = 0;
i2c_ByteCount = ByteNumbers;

i2c_SlaveAddress = SAddr;

		for (i=0;i<ByteNumbers;i++)
		{
			i2c_Buffer[i]=Byte[i];
		}


i2c_Do = i2c_sawp;

MasterOutFunc = WhatDo;
ErrorOutFunc = WhatDo;

TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;

i2c_Do |= i2c_Busy;

return 1;
}


uint8 i2c_BH1750FVI_ReadByte(uint8 SAddr, IIC_F WhatDo)
{
if (i2c_Do & i2c_Busy) return 0;

i2c_index = 0;
i2c_ByteCount = 2;

i2c_SlaveAddress = SAddr;

// i2c_PageAddress[0] = 0x02;
// 
// i2c_PageAddrIndex = 0;
// i2c_PageAddrCount = 1;

i2c_Do = i2c_sarp;

MasterOutFunc = WhatDo;
ErrorOutFunc = WhatDo;

TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;

i2c_Do |= i2c_Busy;

return 1;
}
*/






void i2c_BH1750FVI_Write(void)
{
	i2c_BH1750FVI_Address = 0xB8;
	i2c_BH1750FVI_buffer[0] = 0x10;
	i2c_BH1750FVI_buffer_data = 1;
	i2c_BH1750FVI_Write1();
}

void i2c_BH1750FVI_Read(void)
{
	i2c_BH1750FVI_Address = 0xB8;
	i2c_BH1750FVI_buffer_data = 2;
	i2c_BH1750FVI_Read1();
}




void i2c_BH1750FVI_Write1(void)
{
	
	if (!i2c_Write_Buffer(i2c_BH1750FVI_Address, i2c_BH1750FVI_buffer, i2c_BH1750FVI_buffer_data, i2c_BH1750FVI_Writed) )      // ������
	{
		SetTimerTask(i2c_BH1750FVI_timer, i2c_BH1750FVI_Write1,i2c_BH1750FVI_Write_Restart,0);                  		// ���� ������� �� ����� (���� ������), �� ������ ����� 50��.
	//	SetTimerTask(ds18b20_timer, ds18b20_search, 1500, 0);
	}
	
}

void i2c_BH1750FVI_Writed(void)               		// ���� ������� ������
{
	i2c_Do &= i2c_Free;                  		// ����������� ����
	
	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
	{
		if (!(i2c_BH1750FVI_errors & i2c_BH1750FVI_errors_NA))
		{
					SetTimerTask(i2c_BH1750FVI_timer, i2c_BH1750FVI_Write1,i2c_BH1750FVI_Write_Error_Restart, 0);     		 // ����� ������
//					SetTimerTask(i2c_BH1750FVI_Write1,i2c_BH1750FVI_Write_Error_Restart);     		 // ����� ������
							if(i2c_Do & (i2c_ERR_NA))
							{
								i2c_BH1750FVI_errors++;
							}
		}
	}
	else
	{
		i2c_BH1750FVI_errors = 0;
	}
//	SetTimerTask(i2c_BH1750FVI_Read,400);
	SetTimerTask(i2c_BH1750FVI_timer,i2c_BH1750FVI_Read,400,0);
}




void i2c_BH1750FVI_Read1(void)
{
	
	if (!i2c_Read_Buffer(i2c_BH1750FVI_Address, 2,i2c_BH1750FVI_Readed) )      // ������
	{
		SetTimerTask(i2c_BH1750FVI_timer,i2c_BH1750FVI_Read1,i2c_BH1750FVI_Write_Restart,0);                  		// ���� ������� �� ����� (���� ������), �� ������ ����� 50��.
//		SetTimerTask(i2c_BH1750FVI_Read1,i2c_BH1750FVI_Write_Restart);                  		// ���� ������� �� ����� (���� ������), �� ������ ����� 50��.
	}
	
}

void i2c_BH1750FVI_Readed(void)               		// ���� ������� ������
{
	i2c_Do &= i2c_Free;                  		// ����������� ����
	
	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
	{
		if (!(i2c_BH1750FVI_errors & i2c_BH1750FVI_errors_NA))
		{
			SetTimerTask(i2c_BH1750FVI_timer,i2c_BH1750FVI_Read1,i2c_BH1750FVI_Write_Error_Restart,0);     		 // ����� ������
//			SetTimerTask(i2c_BH1750FVI_Read1,i2c_BH1750FVI_Write_Error_Restart);     		 // ����� ������
			if(i2c_Do & (i2c_ERR_NA))
			{
				i2c_BH1750FVI_errors++;
			}
		}

	}
	else
	{
		i2c_BH1750FVI_H = i2c_Buffer[0];
		i2c_BH1750FVI_L = i2c_Buffer[1];
		
		if ((i2c_BH1750FVI_H&i2c_BH1750FVI_L) == 0xFF)
		{
			i2c_BH1750FVI_Illuminance = 65535;
		} 
		else
		{
//			i2c_BH1750FVI_Illuminance = (   (  ( (i2c_Buffer[0]<<8) +i2c_Buffer[1])  /12)   *10);
			i2c_BH1750FVI_Illuminance = (    ( (i2c_BH1750FVI_H<<8) | i2c_BH1750FVI_L)*0.83);

//			INFO("\r\n lux:%d \r\n", i2c_BH1750FVI_Illuminance);
		}
		i2c_BH1750FVI_errors = 0;


	}
}



















