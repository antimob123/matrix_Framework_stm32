#ifndef __W25Q64_H
#define __M25Q64_H

void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t *MID , uint16_t *DID);
//页编程代码  (一次性写入的数据是0-256)
void W25Q64_PageProgram(uint32_t Address , uint8_t *DataArray ,uint16_t Count);  
// 扇区擦除
void W25Q64_SectorErase(uint32_t Address);
// 读取数据 没有字节限制 可以连续读取
void W25Q64_ReadDate(uint32_t Address , uint8_t *DataArray ,uint32_t Count);

#endif
