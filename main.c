#include <REGX52.H>
#include "Delay.h"
#include "MatrixLED.h"

//动画数据
unsigned char code Animation[]=
{
    0x00,0x01,0x0A,0x1C,0x0B,0x00,0x00,0x00,

    0x00,0x00,0x0B,0x1C,0x0A,0x01,0x00,0x00,
}

void main()
{
	unsigned char i,Offset=0,Count=0;
	MatrixLED_Init();
	while(1)
	{
		for(i=0;i<8;i++)	//循环8次，显示8列数据
		{
			MatrixLED_ShowColumn(i,Animation[i+Offset]);
		}
		Count++;			//计次延时
		if(Count>15)
		{
			Count=0;
			Offset+=8;		//偏移+8，切换下一帧画面
			if(Offset>16)
			{
				Offset=0;
			}
		}
	}
}

