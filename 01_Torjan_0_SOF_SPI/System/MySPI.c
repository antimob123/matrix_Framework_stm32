#include "stm32f10x.h"  

void MySPI_W_CS(uint8_t BitValue)                                              //PA3 片选
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction)BitValue);     
}

void MySPI_W_SCK(uint8_t BitValue)                                              //PA5 时钟线
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,(BitAction)BitValue);     
}

void MySPI_W_MOSI(uint8_t BitValue)                                              //PA7  DI 线  主机输出
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_7,(BitAction)BitValue);     
}

uint8_t MySPI_R_MISO(void)
{
   return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);                             //读取输入寄存器
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7;             // 设置 输出引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                            // 推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                           // 设置最大速度
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      // 初始化 GPIOA

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                                   // 设置 输入引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                              // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                           // 设置最大速度
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                     // 初始化 GPIOA    

    MySPI_W_CS(1);     //默认不选中从机
    MySPI_W_SCK(0);    //SPI模式0默认是低电平
    //MOSI 初始化没用明确规定
    //MISO 主机是输入引脚
}


//SPI 三个时序基本单元

void Myspi_Start(void)   //片选拉低
{
    MySPI_W_CS(0);    
}

void Myspi_Stop(void)    //片选拉高
{
    MySPI_W_CS(1);    
}

uint8_t MySPI_Swapbyte(uint8_t ByteSend)   //实现模式0
{
    uint8_t i, ByteReceive = 0x00;
    //软件SPI逻辑 先SC下降沿--移出数据--SCK上升沿--移入数据--SCK下降沿--移出数据   
    for(i = 0; i < 8 ; i++ )
    {
        MySPI_W_MOSI(ByteSend & (0x80 >> i) );   //&1000 0000 屏蔽其他无关位 掩码 //写入  
        MySPI_W_SCK(1);                   //产生上升沿
        if(MySPI_R_MISO()  == 1){ ByteReceive |= (0x80 >> i);} //读取  // |= 1000 0000 先按位或 后赋值  最高位有1则1 //把最高位存在ByteReceive 里
        MySPI_W_SCK(0);                   //产生下降沿
    }
    return ByteReceive;               
}

uint8_t MySPI_Swapbyte_YW(uint8_t ByteSend)   //模式0  符合硬件的逻辑 暂不使用
{
    uint8_t i;
    //软件SPI逻辑 先SC下降沿--移出数据--SCK上升沿--移入数据--SCK下降沿--移出数据   
    for(i = 0; i < 8 ; i++ )         //一个字节完成移位模型
    {
        MySPI_W_MOSI(ByteSend & 0x80 >> i );   
        ByteSend <<= 1 ;
        MySPI_W_SCK(1);                   
        if(MySPI_R_MISO()  == 1){ ByteSend |= 0x01; } 
        MySPI_W_SCK(0);
    }                   
    return ByteSend;               
}


