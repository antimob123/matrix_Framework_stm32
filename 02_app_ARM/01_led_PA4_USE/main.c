void delay(int d)
{
	while(d--);
}


int main()
{
     
	unsigned int *pReg;
	/* ʹ��GPIOA_���ڵ�APB2ʱ�� */
	pReg  =  (unsigned int *) (0x40021000 + 0x18);  
	*pReg |= (1<<2) ;  //�൱�ڰ�ֵ����,�ټ���д��,���ı�����λ
	
	
	/* ����GPIOA4 Ϊ������� */
	pReg = (unsigned int *)(0x40010800 + 0x00);   //GPIOA�Ļ���ַ ����ƫ��
	*pReg |= (0x3 << (4 * 4));  // �ȼ��� 0b0011
	
	pReg = (unsigned int *)(0x40010800 + 0x0C);  
	
   while (1)
	 {	   
		 *pReg |= (1 << 4);
		 delay(100000);
		 *pReg &= ~(1 << 4);
		 delay(100000);	 
	 }
}  
