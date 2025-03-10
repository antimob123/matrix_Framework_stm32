#ifndef __MYSPI_H
#define __MYSPI_H

void MySPI_Init(void);
//SPI 三个时序基本单元
void Myspi_Start(void);  //片选拉低
void Myspi_Stop(void);  //片选拉高
uint8_t MySPI_Swapbyte(uint8_t ByteSend); //实现模式0

#endif
