/*
 * IIC_SSD1306.c
 *
 *  Created on: 22 ����. 2017 �.
 *      Author: Kitaro
 */


 #include "driver/IIC_SSD1306.h"


#define HI(X) (X>>8)
#define LO(X) (X & 0xFF)


uint8_t i2c_SSD1306_buffer[i2c_SSD1306_buffer_lenght];

uint8_t i2c_SSD1306_Init_Stage = 0;

uint8_t i2c_SSD1306_CBWP = 0;
uint8_t i2c_SSD1306_CBRP = 0;
#define i2c_SSD1306_CBF_Empty 0
#define i2c_SSD1306_CBF_Full 1
uint8_t i2c_SSD1306_CBF = (1<<i2c_SSD1306_CBF_Empty);
uint8_t i2c_SSD1306_CBD[i2c_SSD1306_CBL];
uint8_t i2c_SSD1306_CBD_buffer_size;






uint8_t i2c_SSD1306_clearDisplay_Index = 0;	//	������ ��� ��������� ������� ������ - 128 ����� 8�8 (8 �������� * 1 �������)
									//	������ 4 ��� - ����� (�������), ������� 4 ��� ���� �������� - 16 ����� �� 8 ��������
#define i2c_SSD1306_clearDisplay_Index_Row	0b11110000
#define i2c_SSD1306_clearDisplay_Index_Coll	0b00001111
#define i2c_SSD1306_clearDisplay_Index_Row_Offset	4




 		uint8_t ssd1306_global_buffer[i2c_SSD1306_Max_X * (i2c_SSD1306_Max_Y / 8)]; //	Screen buffer
// 		uint8_t global_buffer_update[i2c_SSD1306_Max_X * (i2c_SSD1306_Max_Y / 8)];	//	�������� ��������� ������
 		uint8 	global_buffer_empty = 1;
// 		uint8_t buffer_update_flags[(i2c_SSD1306_Max_X / i2c_SSD1306_X_Sprite_Size) * ((i2c_SSD1306_Max_Y / 8) / 8)];	//	16 ����, ����� ���� ������� �� �������� �������
																											//	1 ������ - 8�8 ����� �� ����� (8 �������� �� 1 �������)
																											//	����� �� � ���� ������� �� ���� �������, ������ 8 ������� �� ������


uint8_t update_state = 0;






static uint8  Font5x8[][5] =
{
   { 0x00, 0x00, 0x00, 0x00, 0x00 },   //   0x20  32
   { 0x00, 0x00, 0x5F, 0x00, 0x00 },   // ! 0x21  33
   { 0x00, 0x07, 0x00, 0x07, 0x00 },   // " 0x22  34
   { 0x14, 0x7F, 0x14, 0x7F, 0x14 },   // # 0x23  35
   { 0x24, 0x2A, 0x7F, 0x2A, 0x12 },   // $ 0x24  36
   { 0x4C, 0x2C, 0x10, 0x68, 0x64 },   // % 0x25  37
   { 0x36, 0x49, 0x55, 0x22, 0x50 },   // & 0x26  38
   { 0x00, 0x05, 0x03, 0x00, 0x00 },   // ' 0x27  39
   { 0x00, 0x1C, 0x22, 0x41, 0x00 },   // ( 0x28  40
   { 0x00, 0x41, 0x22, 0x1C, 0x00 },   // ) 0x29  41
   { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // * 0x2A  42
   { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // + 0x2B  43
   { 0x00, 0x00, 0x50, 0x30, 0x00 },   // , 0x2C  44
   { 0x10, 0x10, 0x10, 0x10, 0x10 },   // - 0x2D  45
   { 0x00, 0x60, 0x60, 0x00, 0x00 },   // . 0x2E  46
   { 0x20, 0x10, 0x08, 0x04, 0x02 },   // / 0x2F  47
   { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0 0x30  48
   { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1 0x31  49
   { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2 0x32  50
   { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3 0x33  51
   { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4 0x34  52
   { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5 0x35  53
   { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6 0x36  54
   { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7 0x37  55
   { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8 0x38  56
   { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9 0x39  57
   { 0x00, 0x36, 0x36, 0x00, 0x00 },   // : 0x3A  58
   { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ; 0x3B  59
   { 0x08, 0x14, 0x22, 0x41, 0x00 },   // < 0x3C  60
   { 0x14, 0x14, 0x14, 0x14, 0x14 },   // = 0x3D  61
   { 0x00, 0x41, 0x22, 0x14, 0x08 },   // > 0x3E  62
   { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ? 0x3F  63
   { 0x32, 0x49, 0x79, 0x41, 0x3E },   // @ 0x40  64
   { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A 0x41  65
   { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B 0x42  66
   { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C 0x43  67
   { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D 0x44  68
   { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E 0x45  69
   { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F 0x46  70
   { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G 0x47  71
   { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H 0x48  72
   { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I 0x49  73
   { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J 0x4A  74
   { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K 0x4B  75
   { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L 0x4C  76
   { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M 0x4D  77
   { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N 0x4E  78
   { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O 0x4F  79
   { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P 0x50  80
   { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q 0x51  81
   { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R 0x52  82
   { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S 0x53  83
   { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T 0x54  84
   { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U 0x55  85
   { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V 0x56  86
   { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W 0x57  87
   { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X 0x58  88
   { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y 0x59  89
   { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z 0x5A  90
   { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [ 0x5B  91
   { 0x02, 0x04, 0x08, 0x10, 0x20 },   // \ 0x5C  92
   { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ] 0x5D  93
   { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^ 0x5E  94
   { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _ 0x5F  95
   { 0x00, 0x01, 0x02, 0x04, 0x00 },   // ` 0x60  96
   { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a 0x61  97
   { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b 0x62  98
   { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c 0x63  99
   { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d 0x64 100
   { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e 0x65 101
   { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f 0x66 102
   { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g 0x67 103
   { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h 0x68 104
   { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i 0x69 105
   { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j 0x6A 106
   { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k 0x6B 107
   { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l 0x6C 108
   { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m 0x6D 109
   { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n 0x6E 110
   { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o 0x6F 111
   { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p 0x70 112
   { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q 0x71 113
   { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r 0x72 114
   { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s 0x73 115
   { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t 0x74 116
   { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u 0x75 117
   { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v 0x76 118
   { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w 0x77 119
   { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x 0x78 120
   { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y 0x79 121
   { 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z 0x7A 122
   { 0x00, 0x08, 0x36, 0x41, 0x00 },   // { 0x7B 123
   { 0x00, 0x00, 0x7F, 0x00, 0x00 },   // | 0x7C 124
   { 0x00, 0x41, 0x36, 0x08, 0x00 },   // } 0x7D 125
   { 0x08, 0x04, 0x08, 0x10, 0x08 },   // ~ 0x7E 126
   { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },   //   0x7F 127
   { 0x00, 0x06, 0x09, 0x09, 0x06 },   // ���� ������� 128


   { 0x7C, 0x12, 0x11, 0x12, 0x7C },   // � 0xC0 192
   { 0x7F, 0x49, 0x49, 0x49, 0x31 },   // � 0xC1 193
   { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // � 0xC2 194
   { 0x7F, 0x01, 0x01, 0x01, 0x01 },   // � 0xC3 195
   { 0x60, 0x3F, 0x21, 0x3F, 0x60 },   // � 0xC4 196
   { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // � 0xC5 197
   { 0x77, 0x08, 0x7F, 0x08, 0x77 },   // � 0xC6 198
   { 0x22, 0x41, 0x49, 0x49, 0x36 },   // � 0xC7 199
   { 0x7F, 0x10, 0x08, 0x04, 0x7F },   // � 0xC8 200
   { 0x7E, 0x10, 0x09, 0x04, 0x7E },   // � 0xC9 201
   { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // � 0xCA 202
   { 0x40, 0x3E, 0x01, 0x01, 0x7F },   // � 0xCB 203
   { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // � 0xCC 204
   { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // � 0xCD 205
   { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // � 0xCE 206
   { 0x7F, 0x01, 0x01, 0x01, 0x7F },   // � 0xCF 207
   { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // � 0xD0 208
   { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // � 0xD1 209
   { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // � 0xD2 210
   { 0x07, 0x48, 0x48, 0x48, 0x3F },   // � 0xD3 211
   { 0x0E, 0x11, 0x7F, 0x11, 0x0E },   // � 0xD4 212
   { 0x63, 0x14, 0x08, 0x14, 0x63 },   // � 0xD5 213
   { 0x3F, 0x20, 0x20, 0x3F, 0x60 },   // � 0xD6 214
   { 0x07, 0x08, 0x08, 0x08, 0x7F },   // � 0xD7 215
   { 0x7F, 0x40, 0x7E, 0x40, 0x7F },   // � 0xD8 216
   { 0x3F, 0x20, 0x3F, 0x20, 0x7F },   // � 0xD9 217
   { 0x01, 0x7F, 0x48, 0x48, 0x30 },   // � 0xDA 218
   { 0x7F, 0x48, 0x30, 0x00, 0x7F },   // � 0xDB 219
   { 0x00, 0x7F, 0x48, 0x48, 0x30 },   // � 0xDC 220
   { 0x22, 0x41, 0x49, 0x49, 0x3E },   // � 0xDD 221
   { 0x7F, 0x08, 0x3E, 0x41, 0x3E },   // � 0xDE 222
   { 0x46, 0x29, 0x19, 0x09, 0x7F },   // � 0xDF 223
   { 0x20, 0x54, 0x54, 0x54, 0x78 },   // � 0xE0 224
   { 0x3C, 0x4A, 0x4A, 0x4A, 0x31 },   // � 0xE1 225
   { 0x7C, 0x54, 0x54, 0x28, 0x00 },   // � 0xE2 226
   { 0x7C, 0x04, 0x04, 0x0C, 0x00 },   // � 0xE3 227
   { 0x60, 0x3C, 0x24, 0x3C, 0x60 },   // � 0xE4 228
   { 0x38, 0x54, 0x54, 0x54, 0x18 },   // � 0xE5 229
   { 0x6C, 0x10, 0x7C, 0x10, 0x6C },   // � 0xE6 230
   { 0x00, 0x44, 0x54, 0x54, 0x28 },   // � 0xE7 231
   { 0x7C, 0x20, 0x10, 0x08, 0x7C },   // � 0xE8 232
   { 0x7C, 0x21, 0x12, 0x09, 0x7C },   // � 0xE9 233
   { 0x7C, 0x10, 0x28, 0x44, 0x00 },   // � 0xEA 234
   { 0x40, 0x38, 0x04, 0x04, 0x7C },   // � 0xEB 235
   { 0x7C, 0x08, 0x10, 0x08, 0x7C },   // � 0xEC 236
   { 0x7C, 0x10, 0x10, 0x10, 0x7C },   // � 0xED 237
   { 0x38, 0x44, 0x44, 0x44, 0x38 },   // � 0xEE 238
   { 0x7C, 0x04, 0x04, 0x04, 0x7C },   // � 0xEF 239
   { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // � 0xF0 240
   { 0x38, 0x44, 0x44, 0x44, 0x00 },   // � 0xF1 241
   { 0x04, 0x04, 0x7C, 0x04, 0x04 },   // � 0xF2 242
   { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // � 0xF3 243
   { 0x08, 0x14, 0x7C, 0x14, 0x08 },   // � 0xF4 244
   { 0x44, 0x28, 0x10, 0x28, 0x44 },   // � 0xF5 245
   { 0x3C, 0x20, 0x20, 0x3C, 0x60 },   // � 0xF6 246
   { 0x0C, 0x10, 0x10, 0x10, 0x7C },   // � 0xF7 247
   { 0x7C, 0x40, 0x7C, 0x40, 0x7C },   // � 0xF8 248
   { 0x3C, 0x20, 0x3C, 0x20, 0x7C },   // � 0xF9 249
   { 0x04, 0x7C, 0x50, 0x50, 0x20 },   // � 0xFA 250
   { 0x7C, 0x50, 0x20, 0x00, 0x7C },   // � 0xFB 251
   { 0x00, 0x7C, 0x50, 0x50, 0x20 },   // � 0xFC 252
   { 0x28, 0x44, 0x54, 0x54, 0x38 },   // � 0xFD 253
   { 0x7C, 0x10, 0x38, 0x44, 0x38 },   // � 0xFE 254
   { 0x48, 0x54, 0x34, 0x14, 0x7C }   // � 0xFF 255

};


// Color constants
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x0400
#define LIME 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF


#define FONT5x8_Size1x 0
#define FONT5x8_Size2x 1
#define FONT5x8_NoBorder 0x00	//	���������� ��� ���������� ������� �������� ����, NoBorder - ������� �������� ���� �� �������� ������
#define FONT5x8_BorderRight 0x01	//	�������� 1 ������ *����� ������ �������� �� ������
#define FONT5x8_BorderDown 0x10	//	�������� 1 ������ *����� ������ ����� �� ������
#define FONT5x8_BorderBoth 0x11




























//void DoNothing(void)	// ������� ��������, �������� �������������� ������
//{
//}



void ICACHE_FLASH_ATTR i2c_SSD1306_Init(void)
{

	uint8_t i = 0;
	uint8_t buff[i2c_SSD1306_buffer_lenght];

	switch (i2c_SSD1306_Init_Stage)
	{
		case 0:
			{		// Comented - for 128x64, uncomented 128x32
					buff[i++] = 0x80;
					buff[i++] = 0xAE;

					buff[i++] = 0x80;
//					buff[i++] = 0xA8;
					buff[i++] = 0xD5;

					buff[i++] = 0x80;
//					buff[i++] = 0x3F;
					buff[i++] = 0x80;

					buff[i++] = 0x80;
//					buff[i++] = 0xD3;
					buff[i++] = 0xA8;

					buff[i++] = 0x80;
//					buff[i++] = 0x00;
					buff[i++] = 0x1F;

					i2c_SSD1306_CB_PUSH(buff,i);

				SetTimerTask(SSD1306_timer,i2c_SSD1306_Init,40,0);
				i2c_SSD1306_Init_Stage ++;
				break;
			}
		case 1:
		{
					buff[i++] = 0x80;
//					buff[i++] = 0xB0;
					buff[i++] = 0xD3;

					buff[i++] = 0x80;
//					buff[i++] = 0x00;
					buff[i++] = 0x00;

					buff[i++] = 0x80;
//					buff[i++] = 0x10;
					buff[i++] = 0x40;

					buff[i++] = 0x80;
//					buff[i++] = 0x20;
					buff[i++] = 0x8D;

					buff[i++] = 0x80;
//					buff[i++] = 0x00;
					buff[i++] = 0x14;

					i2c_SSD1306_CB_PUSH(buff,i);

					SetTimerTask(SSD1306_timer,i2c_SSD1306_Init,40,0);
			i2c_SSD1306_Init_Stage ++;
			break;
		}

		case 2:
		{

					buff[i++] = 0x80;
//					buff[i++] = 0x40;
//					buff[i++] = 0xA1; // no mirror X
					buff[i++] = 0xA0; // mirror X

					buff[i++] = 0x80;
//					buff[i++] = 0x81;
//					buff[i++] = 0xC8; // no mirror Y
					buff[i++] = 0xC0; // mirror Y

					buff[i++] = 0x80;
//					buff[i++] = 0xcf;
					buff[i++] = 0xDA;

					buff[i++] = 0x80;
//					buff[i++] = 0xA1;
					buff[i++] = 0x02;

					buff[i++] = 0x80;
//					buff[i++] = 0xC8;
					buff[i++] = 0x81;

					buff[i++] = 0x80;
//					buff[i++] = 0xA6;
					buff[i++] = 0xCF;
					i2c_SSD1306_CB_PUSH(buff,i);

					SetTimerTask(SSD1306_timer,i2c_SSD1306_Init,40,0);

			i2c_SSD1306_Init_Stage ++;
			break;
		}

		case 3:
		{

					buff[i++] = 0x80;
//					buff[i++] = 0xD5;
					buff[i++] = 0xD9;

					buff[i++] = 0x80;
//					buff[i++] = 0x80;
					buff[i++] = 0xF1;

					buff[i++] = 0x80;
//					buff[i++] = 0xD9;
					buff[i++] = 0xDB;

					buff[i++] = 0x80;
//					buff[i++] = 0xF1;
					buff[i++] = 0x40;

					buff[i++] = 0x80;
//					buff[i++] = 0xDA;
					buff[i++] = 0xA4;

					buff[i++] = 0x80;
//					buff[i++] = 0x12;
					buff[i++] = 0xA6;

					i2c_SSD1306_CB_PUSH(buff,i);

					SetTimerTask(SSD1306_timer,i2c_SSD1306_Init,40,0);

			i2c_SSD1306_Init_Stage ++;
			break;
		}
		case 4:
		{
					buff[i++] = 0x80;
					buff[i++] = 0x20;

					buff[i++] = 0x80;
//					buff[i++] = 0x30;
					buff[i++] = 0x00;

					buff[i++] = 0x80;
//					buff[i++] = 0x2E;
					buff[i++] = 0xA4;

					buff[i++] = 0x80;
//					buff[i++] = 0x8D;

					buff[i++] = 0x80;
//					buff[i++] = 0x14;

					i2c_SSD1306_CB_PUSH(buff,i);

					SetTimerTask(SSD1306_timer,i2c_SSD1306_Init,40,0);

			i2c_SSD1306_Init_Stage ++;
			break;

		}
		case 5:
		{
						buff[i++] = 0x80;
						buff[i++] = 0xAF;//Display On

						i2c_SSD1306_CB_PUSH(buff,i);

						SetTimerTask(SSD1306_timer,i2c_SSD1306_Init,40,0);

			i2c_SSD1306_Init_Stage ++;
			break;

		}



		case 6:
		{
			i2c_SSD1306_clearDisplay();
			SetTimerTask(SSD1306_timer,i2c_SSD1306_Init,500,0);

			i2c_SSD1306_Init_Stage ++;
			break;
		}



		case 7:
		{
			memset(&ssd1306_global_buffer[0], 0, sizeof(ssd1306_global_buffer));
//			INFO("\r\n\r\n C7\r\n\r\n");
			i2c_SSD1306_LCD_Update();
			break;
		}

	}

}


/*

void ICACHE_FLASH_ATTR i2c_SSD1306_LCD_Updated(void)
{
	i2c_Do &= i2c_Free;                  		// ����������� ����

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
	{
		SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_LCD_Update,i2c_SSD1306_Write_Error_Restart,0);     		 // ����� ������
	}
	else
	{
			if (update_state ==0)
				{
				update_state=1;
				}
			else
			{
			global_pointer = global_pointer + i2c_global_MaxBuffer-1;
			}
			if (global_pointer > i2c_SSD1306_Max_X * (i2c_SSD1306_Max_Y / 8))
			{
				global_pointer=0;
				update_state=0;
				SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_LCD_Update,100,0);

			}
			else
			{
				SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_LCD_Update,1,0);

			}

	}
}

*/




void ICACHE_FLASH_ATTR i2c_SSD1306_LCD_Updated(void)
{
	i2c_Do &= i2c_Free;                  		// ����������� ����

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
	{
		SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_LCD_Update,i2c_SSD1306_Write_Error_Restart,0);     		 // ����� ������
	}
	else
	{
//			if (update_state ==0)
//				{
//				update_state=1;
//				}
//			else
//			{
//			global_pointer = global_pointer + i2c_global_MaxBuffer-1;
//			}
			if (global_pointer >= i2c_SSD1306_Max_X * (i2c_SSD1306_Max_Y / 8))
			{
//				global_pointer=0;
//				update_state=0;
//				SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_LCD_Update,50,0);
				os_timer_disarm(&SSD1306_CB_timer);
				global_pointer=0;
			}
			else
			{
				SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_LCD_Update,2,0);

			}

	}
}



/*
void ICACHE_FLASH_ATTR i2c_SSD1306_LCD_Update (void)
{
	uint8_t buff[i2c_SSD1306_buffer_lenght];
	uint8_t i;
//			INFO("\r\n\r\n UU\r\n\r\n");

	switch (update_state)
	{
	case 0:
	{
		i = 0;
//		INFO("\r\n\r\n U0\r\n\r\n");
		buff[i++] = i2c_SSD1306_COMMAND_MODE;
		buff[i++] = (0x00 | ((0) & 0x0F));
		buff[i++] = i2c_SSD1306_COMMAND_MODE;
		buff[i++] = (0x10 | (((0)>>4)&0x07));
		buff[i++] = i2c_SSD1306_COMMAND_MODE;
		buff[i++] = (0xB0 | 0);
		if (!i2c_Write_Buffer(i2c_SSD1306_OLED_ADDRESS,buff, i, i2c_SSD1306_LCD_Updated))
			{
				SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_LCD_Update,i2c_SSD1306_Write_Restart,0);
			}
//		else
//		{
//			update_state++;
//		}
		break;
	}

	case 1:
	{
		i = 0;
//		INFO("\r\n\r\n U1\r\n\r\n");
		buff[i++] = i2c_SSD1306_DATA_MODE;
		uint8_t ii;
		for (ii = 0; ii < i2c_global_MaxBuffer-1; ii++)
		{
			if (global_pointer+ii > i2c_SSD1306_Max_X * (i2c_SSD1306_Max_Y / 8))
			{
				break;
			}
				buff[i++] = global_buffer[global_pointer+ii];
		}
		if (!i2c_Write_Buffer(i2c_SSD1306_OLED_ADDRESS,buff, i, i2c_SSD1306_LCD_Updated))
			{
				SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_LCD_Update,i2c_SSD1306_Write_Restart,0);
			}


		break;
	}
	}

}

*/



void ICACHE_FLASH_ATTR i2c_SSD1306_LCD_Update (void)
{
	uint8_t buff[i2c_SSD1306_buffer_lenght+10];
	uint8_t i;
	uint8_t ii;

//			INFO("\r\n\r\n UU\r\n\r\n");

	if ( (global_pointer > i2c_SSD1306_Max_X * (i2c_SSD1306_Max_Y / 8)) || (global_pointer==0))
	{
		i = 0;
//		INFO("\r\n\r\n U0\r\n\r\n");
		buff[i++] = i2c_SSD1306_COMMAND_MODE;
		buff[i++] = (0x00 | ((0) & 0x0F));
		buff[i++] = i2c_SSD1306_COMMAND_MODE;
		buff[i++] = (0x10 | (((0)>>4)&0x07));
		buff[i++] = i2c_SSD1306_COMMAND_MODE;
		buff[i++] = (0xB0 | 0);


		buff[i++] = i2c_SSD1306_DATA_MODE;
		for (ii = 0; ii < i2c_SSD1306_frame_Size; ii++)
		{
				buff[i++] = ssd1306_global_buffer[ii];
		}
		if (!i2c_Write_Buffer(i2c_SSD1306_OLED_ADDRESS,buff, i, i2c_SSD1306_LCD_Updated))
			{
				SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_LCD_Update,i2c_SSD1306_Write_Restart,0);
			}
		else
			{
				global_pointer = ii;
			}
	}
	else
	{
		i = 0;
//		INFO("\r\n\r\n U1\r\n\r\n");
		buff[i++] = i2c_SSD1306_DATA_MODE;
		uint8_t ii;
		for (ii = 0; ii < i2c_SSD1306_frame_Size; ii++)
		{
			if (global_pointer+ii >= i2c_SSD1306_Max_X * (i2c_SSD1306_Max_Y / 8))
			{
				break;
			}
				buff[i++] = ssd1306_global_buffer[global_pointer+ii];
		}
		if (!i2c_Write_Buffer(i2c_SSD1306_OLED_ADDRESS,buff, i, i2c_SSD1306_LCD_Updated))
			{
				SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_LCD_Update,i2c_SSD1306_Write_Restart,0);
			}
		else
		{
			global_pointer = global_pointer + i2c_SSD1306_frame_Size;
		}

	}

}




































uint8_t ICACHE_FLASH_ATTR i2c_SSD1306_Send_GB_Char ( uint8_t x,uint8_t y, SSDLcdFontSize size, uint8_t ch)
{
//	INFO ("\r\n\r\n char %c x %d y %d \r\n", ch, x ,y);
	uint8_t i, c;
//	uint8_t ii=0;
	uint8_t b1, b2;
	int  tmpIdx = 0;

	if ( (ch >= 0x20) && (ch <= 0x80) )
	{
		// �������� � ������� ��� �������� ASCII[0x20-0x80]
		ch -= 32;
	}
	else if ( ch >= 0xC0 )
	{
		// �������� � ������� ��� �������� CP1251[0xC0-0xFF]
		ch -= 96;
	}
	else
	{
		// ��������� ���������� (�� ������ ��� � ������� ��� �������� ������)
		ch = 95;
	}
	if ( size == SSDFONT_1X )
	{
//			global_buffer[ (x+ii++) * y] = i2c_SSD1306_DATA_MODE;
		for ( i = 0; i < 5; i++ )
		{
			// �������� ��� ������� �� ������� � ���
			ssd1306_global_buffer[ (x+i) + (y*128)] = Font5x8[ch][i];

		}
		ssd1306_global_buffer[ (x+i) + (y*128)] = 0x00;	//	����� ��� ���������
	}
	else if ( size == SSDFONT_2X )
	{
//		global_buffer[ (x+ii++) * y] = i2c_SSD1306_DATA_MODE;

// 		tmpIdx = LcdCacheIdx - 84;
//
// 		if ( tmpIdx < LoWaterMark )
// 		{
// 			LoWaterMark = tmpIdx;
// 		}
//
// 		if ( tmpIdx < 0 ) return OUT_OF_BORDER;
//
 		for ( i = 0; i < 5; i++ )
 		{
 			c = (Font5x8[ch][i]);

 			// �������� ��� ������� �� ������� � ��������� ����������
 			// ����������� ��������
 			// ������ �����
 			b1 =  (c & 0x01) * 3;
 			b1 |= (c & 0x02) * 6;
 			b1 |= (c & 0x04) * 12;
 			b1 |= (c & 0x08) * 24;
 			ssd1306_global_buffer[ (x+i*2) + (y*128)] = b1;
 			ssd1306_global_buffer[ (x+i*2+1) + (y*128)] = b1;

 			// �������� ��� ����� � ���
			c >>= 4;
			// ������ �����
			b2 =  (c & 0x01) * 3;
			b2 |= (c & 0x02) * 6;
			b2 |= (c & 0x04) * 12;
			b2 |= (c & 0x08) * 24;
			ssd1306_global_buffer[ (x+i*2) + ((y+1)*128)] = b2;
			ssd1306_global_buffer[ (x+i*2+1) + ((y+1)*128)] = b2;
		}
 		ssd1306_global_buffer[ (x+i*2) + (y*128)] = 0x00;	//	����� ��� ���������
 		ssd1306_global_buffer[ (x+i*2+1) + (y*128)] = 0x00;	//	����� ��� ���������
 		ssd1306_global_buffer[ (x+i*2) + ((y+1)*128)] = 0x00;
		ssd1306_global_buffer[ (x+i*2+1) + ((y+1)*128)] = 0x00;



 //		global_buffer[ (x+i) * (y+1)] = 0x00;	//	����� ��� ���������


//
// 		// ��������� x ���������� �������
// 		LcdCacheIdx = (LcdCacheIdx + 11) % LCD_CACHE_SIZE;
	}
	return 1;
}



uint8_t ICACHE_FLASH_ATTR i2c_SSD1306_Send_GB_Addr_String ( uint8_t x,uint8_t y, SSDLcdFontSize size, uint8_t dataArray[] )
{
	uint8_t i=0;
	uint8_t buff[6];
	uint8_t tmpIdx=0;
	uint8_t response;
	uint8_t nx, ny;
	nx = x;
	ny = y;
	while( dataArray[ tmpIdx ] != '\0' )
	{
		// ������� ������
		response = i2c_SSD1306_Send_GB_Char( nx, ny, size, dataArray[ tmpIdx ]);
		// �� ����� ����������� ���� ���������� OUT_OF_BORDER,
		// ������ ����� ���������� ������ �� ������ �������
		if( response == 0)
		return 0;
		// ����������� ���������
		tmpIdx++;
		switch (size)
		{
		case SSDFONT_1X:
		{
			nx = x + tmpIdx*6;
			break;
		}
		case SSDFONT_2X:
		{
			nx = x + tmpIdx*12;
			break;
		}
		}
	}
	return 1;
}








































































































void ICACHE_FLASH_ATTR i2c_SSD1306_SetNormalDisplay(void)
{
	uint8_t i=0;
	uint8_t buff[2];
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = i2c_SSD1306_CMD_NORMAL_DISPLAY;
	i2c_SSD1306_CB_PUSH(buff,i);
}

void ICACHE_FLASH_ATTR i2c_SSD1306_SetInverseDisplay(void)
{
	uint8_t i=0;
	uint8_t buff[2];
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = i2c_SSD1306_CMD_INVERSE_DISPLAY;
	i2c_SSD1306_CB_PUSH(buff,i);
}

void ICACHE_FLASH_ATTR i2c_SSD1306_SetBrightness(uint8_t Brightness)
{
	uint8_t i=0;
	uint8_t buff[4];
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = i2c_SSD1306_CMD_SET_BRIGHTNESS;
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = Brightness;
	i2c_SSD1306_CB_PUSH(buff,i);
}

void ICACHE_FLASH_ATTR i2c_SSD1306_clearDisplay(void)
{
uint8_t buff[i2c_SSD1306_buffer_lenght];
uint8_t i=0;

	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0x00 | (((i2c_SSD1306_clearDisplay_Index&i2c_SSD1306_clearDisplay_Index_Coll)*16) & 0x0F));
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0x10 | ((((i2c_SSD1306_clearDisplay_Index&i2c_SSD1306_clearDisplay_Index_Coll)*16)>>4)&0x07));
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0xB0 | ((i2c_SSD1306_clearDisplay_Index&i2c_SSD1306_clearDisplay_Index_Row)>>i2c_SSD1306_clearDisplay_Index_Row_Offset));
	i2c_SSD1306_CB_PUSH(buff,i);

	i=0;
	buff[i++] = i2c_SSD1306_DATA_MODE;
	for (uint8_t coll=0;coll<16;coll++)
	{
		buff[i++] = 0x00;
	}
	i2c_SSD1306_CB_PUSH(buff,i);

	if ( (i2c_SSD1306_clearDisplay_Index&i2c_SSD1306_clearDisplay_Index_Coll) <7 )
	{
		i2c_SSD1306_clearDisplay_Index++;
		SetTimerTask(SSD1306_timer,i2c_SSD1306_Init,37,0);

	}
	else
	{
		if ((i2c_SSD1306_clearDisplay_Index&i2c_SSD1306_clearDisplay_Index_Row) < 0x70)
		{
			i2c_SSD1306_clearDisplay_Index &= i2c_SSD1306_clearDisplay_Index_Row;
			i2c_SSD1306_clearDisplay_Index += 0x10;
			SetTimerTask(SSD1306_timer,i2c_SSD1306_Init,37,0);

		}
		else
		{
			i2c_SSD1306_clearDisplay_Index = 0;

			SetTimerTask(SSD1306_timer,i2c_SSD1306_Init,3000,0);
			i2c_SSD1306_Init_Stage ++;

		}
	}
}



uint8_t ICACHE_FLASH_ATTR i2c_SSD1306_Send_String ( SSDLcdFontSize size, uint8_t dataArray[] )
{
	uint8_t tmpIdx=0;
	uint8_t response;
	while( dataArray[ tmpIdx ] != '\0' )
	{
		// ������� ������
		response = i2c_SSD1306_Send_Char( size, dataArray[ tmpIdx ], 0 );
		// �� ����� ����������� ���� ���������� OUT_OF_BORDER,
		// ������ ����� ���������� ������ �� ������ �������
		if( response == 0)
		return 0;
		// ����������� ���������
		tmpIdx++;
	}
	return 1;
}


uint8_t ICACHE_FLASH_ATTR i2c_SSD1306_Send_Addr_String ( uint8_t x,uint8_t y, SSDLcdFontSize size, uint8_t dataArray[] )
{
	uint8_t i=0;
	uint8_t buff[6];
	uint8 nx, ny;
	nx=x;
	ny=y;
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0x00 | ((nx) & 0x0F));
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0x10 | (((nx)>>4)&0x07));
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0xB0 | ny);
	i2c_SSD1306_CB_PUSH(buff,i);

	uint8_t tmpIdx=0;
	uint8_t response;
	while( dataArray[ tmpIdx ] != '\0' )
	{
		if (size == SSDFONT_1X)
		{
		// ������� ������
		response = i2c_SSD1306_Send_Char( size, dataArray[ tmpIdx ],0 );
		// �� ����� ����������� ���� ���������� OUT_OF_BORDER,
		// ������ ����� ���������� ������ �� ������ �������
		if( response == 0)
		return 0;
		// ����������� ���������
		tmpIdx++;
		}
		else if (size ==SSDFONT_2X)
		{
			buff[i++] = i2c_SSD1306_COMMAND_MODE;
			buff[i++] = (0x00 | ((nx) & 0x0F));
			buff[i++] = i2c_SSD1306_COMMAND_MODE;
			buff[i++] = (0x10 | (((nx)>>4)&0x07));
			buff[i++] = i2c_SSD1306_COMMAND_MODE;
			buff[i++] = (0xB0 | ny);
			i2c_SSD1306_CB_PUSH(buff,i);

			response = i2c_SSD1306_Send_Char( size, dataArray[ tmpIdx ],0 );
			// �� ����� ����������� ���� ���������� OUT_OF_BORDER,
			// ������ ����� ���������� ������ �� ������ �������
			if( response == 0)
			return 0;
			// ����������� ���������


			ny=y+1;
			buff[i++] = i2c_SSD1306_COMMAND_MODE;
			buff[i++] = (0x00 | ((nx) & 0x0F));
			buff[i++] = i2c_SSD1306_COMMAND_MODE;
			buff[i++] = (0x10 | (((nx)>>4)&0x07));
			buff[i++] = i2c_SSD1306_COMMAND_MODE;
			buff[i++] = (0xB0 | ny);
			i2c_SSD1306_CB_PUSH(buff,i);

			response = i2c_SSD1306_Send_Char( size, dataArray[ tmpIdx ],1 );
			// �� ����� ����������� ���� ���������� OUT_OF_BORDER,
			// ������ ����� ���������� ������ �� ������ �������
			if( response == 0)
			return 0;
			// ����������� ���������

			ny = y;
			nx = nx+6;
			tmpIdx++;

		}
	}
	return 1;
}



uint8_t ICACHE_FLASH_ATTR i2c_SSD1306_Send_Char ( SSDLcdFontSize size, uint8_t ch, uint8 stage )
{
	uint8_t i, c;
	uint8_t ii=0;
	uint8_t b1, b2;
	int  tmpIdx = 0;
	uint8_t buff[i2c_SSD1306_buffer_lenght];

	if ( (ch >= 0x20) && (ch <= 0x80) )
	{
		// �������� � ������� ��� �������� ASCII[0x20-0x80]
		ch -= 32;
	}
	else if ( ch >= 0xC0 )
	{
		// �������� � ������� ��� �������� CP1251[0xC0-0xFF]
		ch -= 96;
	}
	else
	{
		// ��������� ���������� (�� ������ ��� � ������� ��� �������� ������)
		ch = 95;
	}
	if ( size == SSDFONT_1X )
	{
			buff[ii++] = i2c_SSD1306_DATA_MODE;
		for ( i = 0; i < 5; i++ )
		{
			// �������� ��� ������� �� ������� � ���
			buff[ii++] = Font5x8[ch][i];

		}
			buff[ii++] = 0x00;	//	����� ��� ���������
			i2c_SSD1306_CB_PUSH(buff,ii);
	}
	else if ( size == SSDFONT_2X )
	{
		buff[ii++] = i2c_SSD1306_DATA_MODE;

// 		tmpIdx = LcdCacheIdx - 84;
//
// 		if ( tmpIdx < LoWaterMark )
// 		{
// 			LoWaterMark = tmpIdx;
// 		}
//
// 		if ( tmpIdx < 0 ) return OUT_OF_BORDER;
//
 		for ( i = 0; i < 5; i++ )
 		{
 			c = Font5x8[ch][i] << 1;
 			switch (stage)
			{
 			case 0:

 			// �������� ��� ������� �� ������� � ��������� ����������
 			// ����������� ��������
 			// ������ �����
 			b1 =  (c & 0x01) * 3;
 			b1 |= (c & 0x02) * 6;
 			b1 |= (c & 0x04) * 12;
 			b1 |= (c & 0x08) * 24;
 			buff[ii++] = b1;
 			buff[ii++] = b1;
				break;

 			case 1:
 			// �������� ��� ����� � ���
			c >>= 4;
			// ������ �����
			b2 =  (c & 0x01) * 3;
			b2 |= (c & 0x02) * 6;
			b2 |= (c & 0x04) * 12;
			b2 |= (c & 0x08) * 24;
			buff[ii++] = b2;
			buff[ii++] = b2;
				break;

 			default:
 				break;
			}
 		}

		buff[ii++] = 0x00;	//	����� ��� ���������
		i2c_SSD1306_CB_PUSH(buff,ii);


//
// 		// ��������� x ���������� �������
// 		LcdCacheIdx = (LcdCacheIdx + 11) % LCD_CACHE_SIZE;
	}
	return 1;
}









void ICACHE_FLASH_ATTR i2c_SSD1306_SetCursorXY(uint8_t x,uint8_t y)
//	���������� ��������� ������, � � ����� 0...128, y � ����� 0..7
{
	uint8_t i=0;
	uint8_t buff[6];
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0x00 | ((x) & 0x0F));
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0x10 | (((x)>>4)&0x07));
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0xB0 | y);
	i2c_SSD1306_CB_PUSH(buff,i);
}

void ICACHE_FLASH_ATTR i2c_SSD1306_SetStr5CursorXY(uint8_t x,uint8_t y)
//	���������� ��������� ������ ��� ����� 5�8, � � ����� 0...21, y � ����� 0..7
//	����� ������� 5 ������ + 1 ������ �� ����� �� ��������
{
	uint8_t i=0;
	uint8_t buff[6];
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0x00 | ((x*6) & 0x0F));
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0x10 | (((x*6)>>4)&0x07));
	buff[i++] = i2c_SSD1306_COMMAND_MODE;
	buff[i++] = (0xB0 | y);
	i2c_SSD1306_CB_PUSH(buff,i);
}







void ICACHE_FLASH_ATTR i2c_SSD1306_Write1(void)
{

	if (!i2c_Write_Buffer(i2c_SSD1306_OLED_ADDRESS, i2c_SSD1306_buffer, i2c_SSD1306_buffer_data, i2c_SSD1306_Writed) )      // ������
	{
		SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_Write1,i2c_SSD1306_Write_Restart,0);                  		// ���� ������� �� ����� (���� ������), �� ������ ����� 50��.
	}

}

void ICACHE_FLASH_ATTR i2c_SSD1306_Writed(void)               		// ���� ������� ������
{
	i2c_Do &= i2c_Free;                  		// ����������� ����

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
	{
		SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_Write1,i2c_SSD1306_Write_Error_Restart,0);     		 // ����� ������
	}
}



void ICACHE_FLASH_ATTR i2c_SSD1306_CB_LOCK (void)
{
	i2c_SSD1306_CBF |= (1<<i2c_SSD1306_CBF_Full); // ������ ���� - ����� �����
//	cli(); //; ��� ����� ����� ���������, ������� ������� �� ������ ������������ ���������� ������, ������ �������� ���������
//		LED_PORT  ^=(1<<LED3);  	// �������� ������
	uint8_t i;
	while (1)
		{
			i++;
		}
}


void ICACHE_FLASH_ATTR i2c_SSD1306_CB_PUSH (uint8_t buffer[], uint8_t buffer_lenght)
{
	if ((i2c_SSD1306_CBWP!=i2c_SSD1306_CBRP)||(i2c_SSD1306_CBF&(1<<i2c_SSD1306_CBF_Empty)))
	{
		if ((i2c_SSD1306_CBF&(1<<i2c_SSD1306_CBF_Empty)))
		{
			SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_CB_POP,5,0);
		}
		i2c_SSD1306_CBF &= ~(1<<i2c_SSD1306_CBF_Empty);

		i2c_SSD1306_CBD[i2c_SSD1306_CBWP]=buffer_lenght;
		i2c_SSD1306_CBWP ++;
		if (i2c_SSD1306_CBWP == i2c_SSD1306_CBL) {i2c_SSD1306_CBWP=0;}

		if (i2c_SSD1306_CBWP==i2c_SSD1306_CBRP)
		{
			i2c_SSD1306_CB_LOCK();
		}
			for (uint8_t i=0; i<buffer_lenght;i++)
			{
				i2c_SSD1306_CBD[i2c_SSD1306_CBWP] = buffer[i];
				i2c_SSD1306_CBWP ++;
				if (i2c_SSD1306_CBWP == i2c_SSD1306_CBL) {i2c_SSD1306_CBWP=0;}

				if (i2c_SSD1306_CBWP==i2c_SSD1306_CBRP)
				{
					i2c_SSD1306_CB_LOCK();
				}
			}
	}
	else
	{
		i2c_SSD1306_CB_LOCK();
	}
}


void ICACHE_FLASH_ATTR i2c_SSD1306_CB_POP_Send(void)
{
	if (!i2c_Write_Buffer(i2c_SSD1306_OLED_ADDRESS,i2c_SSD1306_buffer, i2c_SSD1306_CBD_buffer_size, i2c_SSD1306_CB_POPED))
		{
			SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_CB_POP_Send,i2c_SSD1306_Write_Restart,0);
		}
}


void ICACHE_FLASH_ATTR i2c_SSD1306_CB_POP(void)
{
	if ((i2c_SSD1306_CBF&(1<<i2c_SSD1306_CBF_Empty)) == 0)
		{
			i2c_SSD1306_CBD_buffer_size = i2c_SSD1306_CBD[i2c_SSD1306_CBRP];
			i2c_SSD1306_CBRP++;
			for (uint8_t i=0;i<i2c_SSD1306_CBD_buffer_size;i++)
				{
					i2c_SSD1306_buffer[i] = i2c_SSD1306_CBD[i2c_SSD1306_CBRP];
					i2c_SSD1306_CBRP++;
					if (i2c_SSD1306_CBRP == i2c_SSD1306_CBL) {i2c_SSD1306_CBRP=0;}
				}
			i2c_SSD1306_CB_POP_Send();
		}
}


void ICACHE_FLASH_ATTR i2c_SSD1306_CB_POPED(void)
{
	i2c_Do &= i2c_Free;                  		// ����������� ����

	if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
	{
		SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_CB_POP_Send,i2c_SSD1306_Write_Error_Restart,0);     		 // ����� ������
	}
	else
	{
		if (i2c_SSD1306_CBWP == i2c_SSD1306_CBRP)
		{
			i2c_SSD1306_CBF |=(1<<i2c_SSD1306_CBF_Empty);	 // ������ ��������� - ����� �������
		}
		else
		{
			SetTimerTask(SSD1306_CB_timer,i2c_SSD1306_CB_POP,2,0);
		}
	}
}





































































// void i2c_SSD1306_CB_PUSH (uint8_t command,uint8_t buffer)
// {
// 	if ((i2c_SSD1306_CBWP!=i2c_SSD1306_CBRP)||(i2c_SSD1306_CBF&(1<<i2c_SSD1306_CBF_Empty)))
// 	{
//
// 		i2c_SSD1306_CBF &= ~(1<<i2c_SSD1306_CBF_Empty);
//
// 		i2c_SSD1306_CBC[i2c_SSD1306_CBWP]=command;
// 		i2c_SSD1306_CBD[i2c_SSD1306_CBWP]=buffer;
// 			i2c_SSD1306_CBWP ++;
//
// 		if (i2c_SSD1306_CBWP == i2c_SSD1306_CBL) {i2c_SSD1306_CBWP=0;}
// 	}
// 	else
// 	{
// 		i2c_SSD1306_CBF |= (1<<i2c_SSD1306_CBF_Full); // ������ ���� - ����� �����
// 		cli(); //; ��� ����� ����� ���������, ������� ������� �� ������ ������������ ���������� ������, ������ �������� ���������
// 		uint8_t i;
// 		while (1)
// 		{
// 			i++;
// 		}
// 	}
// }
//
// void i2c_SSD1306_CB_POP(void)
// {
// 	if ((i2c_SSD1306_CBF&(1<<i2c_SSD1306_CBF_Empty)) == 0)
// 	{
// 		i2c_SSD1306_buffer[0] = i2c_SSD1306_CBC[i2c_SSD1306_CBRP];
// 		i2c_SSD1306_buffer[1] = i2c_SSD1306_CBD[i2c_SSD1306_CBRP];
// 		if (!i2c_Write_Buffer(i2c_SSD1306_OLED_ADDRESS,i2c_SSD1306_buffer, 2, i2c_SSD1306_CB_POPED))
// 		{
// 			SetTimerTask(i2c_SSD1306_CB_POP,i2c_SSD1306_Write_Restart);
// 		}
//
// 	}
// 	else
// 	{
// 		SetTimerTask(i2c_SSD1306_CB_POP,200);
// 	}
//
//
// }
//
// void i2c_SSD1306_CB_POPED(void)
// {
// 		i2c_Do &= i2c_Free;                  		// ����������� ����
//
// 		if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))   		// ������ ��� ������ ����?
// 		{
// 			SetTimerTask(i2c_SSD1306_CB_POP,i2c_SSD1306_Write_Error_Restart);     		 // ����� ������
// 		}
// 		else
// 		{
// 					i2c_SSD1306_CBRP ++;
// 					if (i2c_SSD1306_CBRP == i2c_SSD1306_CBL) {i2c_SSD1306_CBRP=0;}
// 					if (i2c_SSD1306_CBWP == i2c_SSD1306_CBRP)
// 					{
// 						i2c_SSD1306_CBF |=(1<<i2c_SSD1306_CBF_Empty);	 // ������ ��������� - ����� �������
// 						SetTimerTask(i2c_SSD1306_CB_POP,200);
// 					}
// 					else
// 					{
// 						SetTimerTask(i2c_SSD1306_CB_POP,4);
// 					}
//
//  		}
//
// }
















