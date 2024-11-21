#include "stm32f10x.h"                  // Device header
#include "app.h"
#include "OLED.h"
#include "Serial.h"
//#include "Key.h"
#define APP_START_ADDRESS  0x08010000  // 应用程序的起始地址
typedef void (*app_entry_t)(void);

void JumpToApplication(void) {
    uint32_t appStack = *((volatile uint32_t*)APP_START_ADDRESS);   // 设置栈指针（SP）
    uint32_t appEntry = *((volatile uint32_t*)(APP_START_ADDRESS + 4)); // 设置程序计数器（PC）

    __set_MSP(appStack);  // 设置主堆栈指针
    app_entry_t appEntryPoint = (app_entry_t)appEntry;
    appEntryPoint();  // 跳转到应用程序
}


volatile uint8_t i = 0xFF;

///////////////////////////////////////////////串口配置部分///////////////////////////////////////////

u8 gUSART1_RxBuf[USART1_MAX_RECV_LEN] = {0};         //USART1_MAX_RECV_LEN 去.h文件填写buff长度
typedef void (*UART_InitFunc)(uint32_t BaudRate);  
UART_InitFunc UART1_init = STM32F103_UART1_Init;     //换芯片更改这里   例:mm32f0120_UART1_Init

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

