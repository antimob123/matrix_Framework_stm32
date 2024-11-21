#include "stm32f10x.h"                  // Device header
#include "app.h"
#include "OLED.h"
#include "Serial.h"
#include "boot.h"
//#include "Key.h"

volatile uint8_t i = 0x00;

///////////////////////////////////////////////串口配置部分///////////////////////////////////////////

u8 gUSART1_RxBuf[USART1_MAX_RECV_LEN] = {0};         //USART1_MAX_RECV_LEN 去.h文件去填写 buff 长度
typedef void (*UART_InitFunc)(uint32_t BaudRate);  
UART_InitFunc UART1_init = STM32F103_UART1_Init;     //更换芯片更改这里 例:STM32F103_UART1_Init

///////////////////////////////////////////////////////////////////////////////////////////////////////

void LED_INIT(void)           // LED0  APA8 
{
    RCC->APB2ENR |= (1 << 2); // 使能 GPIOA 时钟

    // 2. 配置 PA8 为推挽输出模式，最大输出速度为 50MHz
    GPIOA->CRH &= ~(0xF << (4 * 0)); // 清除 PA8 的配置信息
    GPIOA->CRH |= (0x3 << (4 * 0));  // 设置模式为 50MHz 输出模式
    GPIOA->CRH |= (0x0 << (4 * 0 + 2)); // 设置为推挽输出

    // 3. 将 PA8 置高电平，默认关闭 LED
    GPIOA->BSRR = (1 << 8);
}


int main(void)
{	
	SysTick_Init();
	OLED_Init();
  UART1_init(115200);
	LED_INIT();
	
	while (1)
	{	
		 LED_INIT();
     Task_run();

	  #ifdef DEBUG
		for(int i=0;i<999999;i++ )	   {	
	   }		
		printf("app_run\r\n");
		for(int i=0;i<999999;i++ )	   {	
	   }
    #endif	
    }
		
}	

