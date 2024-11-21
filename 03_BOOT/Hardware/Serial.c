#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Serial.h"

static uint16_t	 gUart1ReceStartPtr;
//static uint16_t  gUart1ReceEndPtr;

void STM32F103_UART1_Init(uint32_t BaudRate)      //����оƬ����º���          
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);	
	USART_Cmd(USART1, ENABLE);
}

//

void SendDataToUART1(uint8_t *Data,uint8_t Len)  //���Կ���
{
	uint8_t i = 0;
	
	for(;i < Len;i++)
	{
		USART_SendData(USART1, Data[i]);

		while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TC))
		{
		}
	}
		
}
//

void USART1_SendChar(uint8_t c)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);   //�ȴ����Ƿ������ݼĴ�������
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);    //ѭ���ȴ���������֡�������
}


void USART1_IRQHandler(void)    //���Կ���
{
    u8 recvbyte;	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {		
		recvbyte = USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        gUSART1_RxBuf[gUart1ReceStartPtr] = recvbyte;
        
		if ((uint16_t)gUart1ReceStartPtr >= (uint16_t)(USART1_MAX_RECV_LEN-1))
        {
            gUart1ReceStartPtr = 0;
        }
        else
        {
            gUart1ReceStartPtr++;
        }
    }			
}
//


void ParseUSART1Buffer(void)    // ����buff����˼· ���Ը�������ˢ�µ�ʱ����  �������ü��
{
    uint16_t i = 0;  // ����һ���������������

    while (i < USART1_MAX_RECV_LEN - 1)  // ��֤���㹻���ֽڶ�ȡ����ָֹ��Խ�磩
    {
        // ����Ƿ��ҵ���ͷ A5 5A
        if (gUSART1_RxBuf[i] == 0xA5 && gUSART1_RxBuf[(i + 1) % USART1_MAX_RECV_LEN] == 0x5A) 
        {
            // ����������ֽڵ�����λ�ã������Խ�������߽�����
            uint16_t thirdByteIndex = (i + 3) % USART1_MAX_RECV_LEN;
            
            // ��ȡ�������ֽڵ�ֵ
            volatile uint8_t thirdByteValue = gUSART1_RxBuf[thirdByteIndex];  // ��������Դ���������ֽڵ�ֵ���߽�����������
            //�����������
           
            // ���������������Ѵ�����ֽڣ�����Ѱ����һ����ͷ
            i = (i + 4) % USART1_MAX_RECV_LEN;  // ������Ҫ�����ĳ�������������
        }
        else
        {
            // ���δ�ҵ���ͷ������������һ���ֽ�
            i = (i + 1) % USART1_MAX_RECV_LEN;
        }
    }

    // ������ɺ󣬽�ָ�� gUart1ReceStartPtr ����Ϊ 0x00
    gUart1ReceStartPtr = 0;
}


void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

