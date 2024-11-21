#include "stm32f10x.h"                  // Device header
#include "app.h"
#include "OLED.h"
#include "Serial.h"
//#include "Key.h"
#define APP_START_ADDRESS  0x08010000  // Ӧ�ó������ʼ��ַ
typedef void (*app_entry_t)(void);

void JumpToApplication(void) {
    uint32_t appStack = *((volatile uint32_t*)APP_START_ADDRESS);   // ����ջָ�루SP��
    uint32_t appEntry = *((volatile uint32_t*)(APP_START_ADDRESS + 4)); // ���ó����������PC��

    __set_MSP(appStack);  // ��������ջָ��
    app_entry_t appEntryPoint = (app_entry_t)appEntry;
    appEntryPoint();  // ��ת��Ӧ�ó���
}


volatile uint8_t i = 0xFF;

///////////////////////////////////////////////�������ò���///////////////////////////////////////////

u8 gUSART1_RxBuf[USART1_MAX_RECV_LEN] = {0};         //USART1_MAX_RECV_LEN ȥ.h�ļ���дbuff����
typedef void (*UART_InitFunc)(uint32_t BaudRate);  
UART_InitFunc UART1_init = STM32F103_UART1_Init;     //��оƬ��������   ��:mm32f0120_UART1_Init

///////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{	
	SysTick_Init();
	OLED_Init();
  UART1_init(115200);
//    #ifdef DEBUG
////    USART1_SendChar(i);
//	printf("Holleworld\r\n");
//		for(int i=0;i<999999;i++ ){	
//	}
//    #endif
	JumpToApplication();
	
}	

