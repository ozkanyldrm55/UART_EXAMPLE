/******************************************************
*                                                     *
*@Dosya          :UART.c                         	  *
*@Açýklama       :UART Veriyi gönderir, kesme    	  *
*                ile alýr.                       	  *
*@Versiyon       :v1.0.0                         	  *
*                                                     *
*@Yazar          :OZKAN YILDIRIM                 	  *
*                  						         	  *
*Test            :STM32F407VG Discovery Üstünde       *
*                 Test Edilmiþtir                     *
*******************************************************/

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

unsigned char Veri[255];
int i = 0;

void Delay(uint32_t time)
{
	while(time--);
}

/**************************************************************************************
Fonksiyon:RCC_Configuration
Amaç     :Kullanýlan sistemin clock ayarlamalarýný yapar
Kullaným :RCC_Configuration();
***************************************************************************************/

void RCC_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	/* GPIO CLOCK Ayari */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	/* UART CLOCK Ayari */
}

/**************************************************************************************
Fonksiyon:GPIO_Configuration
Amaç     :Kullanýlan sistemin GPIO ayarlamalarýný yapar
Kullaným :GPIO_Configuration();
**************************************************************************************/

void GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;

	GPIO_PinAFConfig(GPIOA , GPIO_PinSource9 , GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA , GPIO_PinSource10 , GPIO_AF_USART1);


	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; // PA.09 USART1.TX - PA.10 USART1.RX
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/***************************************************************************************
Fonksiyon:USART_Configuration
Amaç     :Kullanýlan sistemin USART1 ayarlamalarýný yapar
Kullaným :USART_Configuration(BaudRate);
***************************************************************************************/

void USART_Config(unsigned int BaudRate)
{
	USART_InitTypeDef	USART_InitStruct;

	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1 , &USART_InitStruct);			/* USART ilk ayar */
	USART_Cmd(USART1,ENABLE);						/* USART1 Aktif et */
	USART_ITConfig(USART1 ,USART_IT_RXNE,ENABLE);	/* USART1 RX kesme Aktif et*/

}

/***************************************************************************************
Fonksiyon:NVIC_Configuration
Amaç     :Kullanýlan sistemin KESME ayarlamalarýný yapar
Kullaným :NVIC_Configuration();
***************************************************************************************/

void NVIC_Config(void)
{
	NVIC_InitTypeDef	NVIV_InitStruct;

	NVIV_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIV_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIV_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIV_InitStruct.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIV_InitStruct);
}

/***************************************************************************************
Kesme ALT PROGRAMI
Amaç     : RX Bufferýna gelen deðerleri Veri[] dizisine alýr
***************************************************************************************/
void USART1_IRQHanler(void)
{
	if(USART_GetITStatus(USART1 , USART_IT_RXNE) != RESET) /* RXEN den data gelmeye devam ediyorsa if komutu calisacak */
	{
		Veri[i] = USART_ReceiveData(USART1);
		i++;
		if(i >= 255)
			i = 0;
	}
}

/***************************************************************************************
Fonksiyon:USART_Yaz
Amaç     :Gönderilmek istenen Yazýyý USART üstünden basar
Kullaným :USART_Yaz("Yazý Buraya Gelecek");
***************************************************************************************/

void USART_Yaz(unsigned char *s)
{
	while(*s)
	{
		while( !(USART1 -> SR & 0x00000040) ); /* CTS LBD TXE TC RXNE IDLE ORE NF FE PE  Data registeri bosalana kadar bekle */
		USART_SendData(USART1,*s);
		*s++;
	}
}

/***************************************************************************************
Fonksiyon:Main
Amaç     :Main Fonksiyonudur
***************************************************************************************/

int main(void)
{
	RCC_Config();
	GPIO_Config();
	USART_Config(9600);
	NVIC_Config();
  while (1)
  {
	USART_Yaz("OZKAN_YLDRM");
	Delay(3360000);
  }
}



/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
