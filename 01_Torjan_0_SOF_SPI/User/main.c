#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "app.h"
#include "OLED.h"
#include "Serial.h"
#include "boot.h"
#include "W25Q64.h"




uint8_t MID = 0xAA;
uint16_t DID = 0xAA;
uint8_t aa = 0xFF;
uint8_t ArrayWrite[] = {0xA1, 0xB2, 0xC3, 0xD4};
uint8_t ArrayRead[] = {0xAA, 0xAA, 0xAA, 0xAA};
//****************************************************串口配置****************************************//
u8 gUSART1_RxBuf[USART1_MAX_RECV_LEN] = {0};         //USART1_MAX_RECV_LEN 去.h文件去填写 buff 长度
typedef void (*UART_InitFunc)(uint32_t BaudRate);  
UART_InitFunc UART1_init = STM32F103_UART1_Init;     
//****************************************************************************************************//



void LED_INIT(void)                                       // LED0  PA4 
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;             // 设置为 PA4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // 推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     // 设置最大速度
    GPIO_Init(GPIOA, &GPIO_InitStructure);                // 初始化 GPIOA
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);                    // 将 PA4 拉低
}
  


int main(void)
{		
  	SysTick_Init();
	  OLED_Init();
		UART1_init(115200);
    LED_INIT();		
		while (1)
		{		
			Task_run();
			#if DEBUG
			
			#endif
		}
//     
}

