void delay(int d)
{
	while(d--);
}


int main()
{
     
	unsigned int *pReg;
	/* 使能GPIOA_所在的APB2时钟 */
	pReg  =  (unsigned int *) (0x40021000 + 0x18);  
	*pReg |= (1<<2) ;  //相当于把值读出,再继续写入,不改变其他位
	
	
	/* 设置GPIOA4 为输出引脚 */
	pReg = (unsigned int *)(0x40010800 + 0x00);   //GPIOA的基地址 不加偏移
	*pReg |= (0x3 << (4 * 4));  // 等价于 0b0011
	
	pReg = (unsigned int *)(0x40010800 + 0x0C);  
	
   while (1)
	 {	   
		 *pReg |= (1 << 4);
		 delay(100000);
		 *pReg &= ~(1 << 4);
		 delay(100000);	 
	 }
}  
