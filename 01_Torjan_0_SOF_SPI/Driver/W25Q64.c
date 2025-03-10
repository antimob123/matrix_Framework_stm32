#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "MySPI.h"                  
#include "W25Q64_Ins.h"    
#include "OLED.h"
#include "Serial.h"

//FLASH  写入不能跨页 读取可以 只能写1不能写0 所有写入之前要先擦除
//初始化
void W25Q64_Init(void)
{
	  MySPI_Init();	 
} 

//读ID
void W25Q64_ReadID(uint8_t *MID , uint16_t *DID)
{
		Myspi_Start();
		MySPI_Swapbyte(W25Q64_JEDEC_ID);
		*MID = MySPI_Swapbyte(W25Q64_DUMMY_BYTE);
		*DID = MySPI_Swapbyte(W25Q64_DUMMY_BYTE);
		*DID  <<= 8;
		*DID |= MySPI_Swapbyte(W25Q64_DUMMY_BYTE);
		Myspi_Stop();	
}

//写使能
void W25Q64_WriteEnable(void)       
{
		Myspi_Start();
		MySPI_Swapbyte(W25Q64_WRITE_ENABLE);
		Myspi_Stop();
}

//读状态寄存器  寄存器可以连续读出
void W25Q64_WaitBusy(void)       
{
		uint32_t Timeout =  50000;
		Myspi_Start();
		MySPI_Swapbyte(W25Q64_READ_STATUS_REGISTER_1);
		while ((MySPI_Swapbyte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)  //检验SPI的状态标准位是否是忙状态
		{
			Timeout -- ;
			if(Timeout == 0)
			{
				#if USART_DEBUG                                            
				printf("SPI Timeout!\n");
				#endif
				#if OLED_DEBUG                                            
				OLED_ShowString(3, 1, "SPI Timeout!");
				#endif	
				break;
			}
		} 
		Myspi_Stop();
		
}

//页编程代码  (一次性写入的数据是0-256)
void W25Q64_PageProgram(uint32_t Address , uint8_t *DataArray ,uint16_t Count)  
{
	uint16_t i;
	W25Q64_WriteEnable();
	Myspi_Start();
	MySPI_Swapbyte(W25Q64_PAGE_PROGRAM);
	MySPI_Swapbyte(Address >> 16);     //高位字节先发送 Address 右移16位 , 是最高位的字节
	MySPI_Swapbyte(Address >> 8);
	MySPI_Swapbyte(Address);//写入数据
	for(i = 0 ; i < Count ; i ++)
	{
		MySPI_Swapbyte(DataArray[i]);
	}
	Myspi_Stop();
	
	W25Q64_WaitBusy();  //事后等待
}

// 扇区擦除  每个扇区的地址都是XXX000 - XXXFFF
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();

	Myspi_Start();
	MySPI_Swapbyte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_Swapbyte(Address >> 16);     //高位字节先发送 Address 右移16位 , 是最高位的字节
	MySPI_Swapbyte(Address >> 8);
	MySPI_Swapbyte(Address);
	Myspi_Stop();
	W25Q64_WaitBusy();  //事后等待
}


// 读取数据 没有字节限制 可以连续读取
void W25Q64_ReadDate(uint32_t Address , uint8_t *DataArray ,uint32_t Count)
{
	uint32_t i;
	Myspi_Start();
	MySPI_Swapbyte(W25Q64_READ_DATA);
	MySPI_Swapbyte(Address >> 16);    
	MySPI_Swapbyte(Address >> 8);
	MySPI_Swapbyte(Address);
	for (i = 0 ; i < Count ; i++)
	{
		DataArray[i] = MySPI_Swapbyte(W25Q64_DUMMY_BYTE);
	}
	//读数据数据后不需要等待
}

