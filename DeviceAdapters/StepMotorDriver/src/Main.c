#include "SerialDriver.h"
#include "1602LCDDriver.h" 
#include "StepMotorDriver.h" 

	unsigned char ch;
	unsigned char rec[8];
	unsigned char i=0;
	bool databegin = 0;

main()
{
    EA = 1;//�����ж�
    EX1 = 1 ;// ���ⲿ�ж�1
	LCD_Initial();
	P1 = 0xff;
	LCD_Printf1("Device Init ok!");
	
	InitSerial(); //Serial
	InitDevice(); //StepMotor

	SendStr("Device Init ok\r\n");
	refLCD(  );
	while(1){}
}

/*------------------------------------------------
                     �����жϳ���
------------------------------------------------*/
void serial () interrupt 4
{

	if(RI) {	
	
		ch=SBUF;
		
		
		if(ch == '@'){//begin
			databegin = 1;	
		}
	
		if(databegin ==1){
			rec[i] = ch;
			i++;	
			
			if(i==7){ 
			databegin = 0;
			i=0;
			rec[7] = '\0';
			parseCMD(rec);				
			}

		}
	 	RI=0;
	}
}

/*------------------------------------------------
                     �ⲿ�жϳ���
------------------------------------------------*/
void key_scan() interrupt 2 //ʹ�����ⲿ�ж�0�ļ���ɨ���Ӻ���
{
	uchar tick = 0;
	if(_manualUpPort==0) //�м������𣿣�k1=0 ?��
	{ 
		delay_ms(10); //��ʱ����
		if(_manualUpPort==0)     //ȷʵ���м����£���
		{
			_directionPort = 0;
			LCD_Printf1("MOVE UP [um]");
			if(_manualAcceleratePort ==  0){

				while(_manualUpPort == 0 &&  _highLimitPort== 1)
				{
					ManualMove(0,1);

				}
				refLCD();
			}else{

				while(_manualUpPort == 0 &&  _highLimitPort== 1)
				{
					ManualMove(0,0);

				}
				refLCD();
			}
		}
	} //�ȴ������ſ�



	if(_manualDownPort==0) //�м������𣿣�k1=0 ?��
	{ 
		delay_ms(10); //��ʱ����
		if(_manualDownPort==0)     //ȷʵ���м����£���
		{
			_directionPort = 1;
			LCD_Printf1("MOVE DOWN [um]");
			if(_manualAcceleratePort ==  0){

				while(_manualDownPort  == 0 && _lowLimitPort == 1 )
				{
					ManualMove(1,1);
				}
				refLCD();
			}else{

				while(_manualDownPort  == 0 && _lowLimitPort == 1)
				{
					ManualMove(1,0);

				}
				refLCD();

			}
		} //�ȴ������ſ�
	} 
}
