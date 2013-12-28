#include "SerialDriver.h"
#include "1602LCDDriver.h" 
#include "StepMotorDriver.h" 

main()
{	
 			
	TCON =0x08; 
	EX1 =  1;
	LCD_Initial();
	P1 = 0xff;
	LCD_Printf1("Device Init ok!");
	refLCD(  );
	InitSerial(); //Serial
	InitDevice(); //StepMotor
	SendStr("Device Init ok\r\n");
	
	while(1){}
}

/*------------------------------------------------
                     �����жϳ���
------------------------------------------------*/
void serial () interrupt 4  
{ 
    unsigned char ch;
 	unsigned char rec[9];
	static unsigned char i=0;
	static unsigned char databegin = 0;
	if(RI) {	
	    RI=0;
		ch=SBUF;
		TI=1; //��SBUF��
		if(ch == '@'){//begin
			databegin = 1;	
		}
		if(databegin ==1){
			rec[i] = ch;
			i++;	
			if(i==8){
				
				parseCMD(rec);
	
				databegin = 0;
				i=0;
			}
		}
		TI=0;
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
