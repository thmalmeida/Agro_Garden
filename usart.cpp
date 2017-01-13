#include "usart.h"

//#define UDR	UDR			//!< USART Transmit Data Buffer Register e USART Receive Data Buffer Registers

/* Habilitando recebimento e/ou envio de dados  */
#define RXEnable		UCSRB|=(1<<RXEN)					//!< Habilitando o recebimento de dados
#define TXEnable		UCSRB|=(1<<TXEN)					//!< Habilitando o envio envio de dados
#define TXRXEnable		UCSRB|=(1<<TXEN)|(1<<RXEN)		//!< Habilitando envio e recebimento de dados

#define BAUDRATE		UBRRH = (uint8_t)(ubrr>>8); \
						UBRRL = (uint8_t)ubrr				//!< Definindo baud rate da USART
															//!< \note Necess�rio ter definido a v�riavel <b>ubrr</b>

/* Velociodade de Opera��o da USART */
#define ASYNC_NORMAL										//!< Definindo modo de opera��o em assincrono normal
#define ASYNC_2XSPEED	UCSRA|=(1<<U2X)					//!< Definindo modo de opera��o em assincrono double speed
#define SYNC_MASTER											//!< Definindo vari�veis para modo de opera��o s�ncrono

/* Mode de opera��o da USART */
#define ASYNCHRONOUS										//!< Modo de opera��o ass�ncrono
#define SYNCHRONOUS		UCSRC|=(1<<UMSEL)				//!< Modo de opera��o s�ncrono
#define MASTER_SPI		UCSRC|=(1<<UMSEL)				//!< Modo de opera��o como Master SPI

/* Parity Bit Mode */
#define PARITY_DISABLE										//!< Paridade desabilitada
#define PARITY_EVEN		UCSRC|=(1<<UPM1)					//!< Paridade par
#define PARITY_ODD		UCSRC|=(1<<UPM1)|(1<<UPM0)		//!< Paridade impar

/* Stop bit */
#define STOP_BIT1											//!< 1 bit para stop bit
#define STOP_BIT2		UCSRC|=(1<<USBS)					//!< 2 bits para stop bit

/* Tamanho do frame */
#define FRAME5												//!< Define o frame com tamanho de 5 bits
#define FRAME6			UCSRC|=(1<<UCSZ0)					//!< Define o frame com tamanho de 6 bits
#define FRAME7			UCSRC|=(1<<UCSZ1)					//!< Define o frame com tamanho de 7 bits
#define FRAME8			UCSRC|=(1<<UCSZ0)|(1<<UCSZ1)		//!< Define o frame com tamanho de 8 bits
#define FRAME9			UCSRC|=(1<<UCSZ0)|(1<<UCSZ1); \
									UCSRB|=(1<<UCSZ2)		//!< Define o frame com tamanho de 9 bits

#define RX_INT_EN		UCSRB|=(1<<RXCIE)

USART::USART(uint32_t baudrate,
			USARTrxinterrupt rxinterrupt,
			USART2xspeed speed,
			USARTrxtx rxtx,
			USARTmode mode,
			USARTparitiy parity,
			USARTstopbit stopbit,
			USARTframe frame)
{
		if(speed == USART_ASYNC_NORMAL){
			ASYNC_NORMAL;
			uint16_t ubrr=F_CPU/16/baudrate-1;
			BAUDRATE;
		}
		else if(speed == USART_ASYNC_2XSPEED){
			ASYNC_2XSPEED;
			uint16_t ubrr=F_CPU/8/baudrate-1;
			BAUDRATE;
		}
		else{
			SYNC_MASTER;
			uint16_t ubrr=F_CPU/2/baudrate-1;
			BAUDRATE;
		}

		if(rxtx == USART_RXTX)
			TXRXEnable;
		else if(rxtx == USART_RX)
			RXEnable;
		else
			TXEnable;

		if(mode == USART_ASYNCHRONOUS)
			ASYNCHRONOUS;
		else if(mode == USART_SYNCHRONOUS)
			SYNCHRONOUS;
		else
			MASTER_SPI;

		if(parity == USART_DISABLE_PARITY)
			PARITY_DISABLE;
		else if(parity == USART_ODD)
			PARITY_ODD;
		else
			PARITY_EVEN;

		if(stopbit == USART_STOPBIT1)
			STOP_BIT1;
		else
			STOP_BIT2;

		if(frame == USART_FRAME5)
			FRAME5;
		else if(frame ==  USART_FRAME6)
			FRAME6;
		else if(frame == USART_FRAME7)
			FRAME7;
		else if(frame == USART_FRAME8)
			FRAME8;
		else{
			FRAME9;
		}

		if(rxinterrupt == USART_INT_ENABLE)
			RX_INT_EN;
}

void USART::send(uint8_t data)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = data;
}
void USART::sendln(char data)
{
		while ( !( UCSRA & (1<<UDRE)) );
		UDR = data;

		while ( !( UCSRA & (1<<UDRE)) );
		UDR = 0x0a;
}
void USART::sendcr(char data)
{
		while ( !( UCSRA & (1<<UDRE)) );
		UDR = data;

		while ( !( UCSRA & (1<<UDRE)) );
		UDR = 0x0d;
}

uint8_t USART::receive(void)
{
	while ( !(UCSRA & (1<<RXC)) ) ;

	return UDR;
}

void USART::send(const char *str, char limit)
{
	while(*str != limit){
		send(*str);
		str++;
	}
}

//inline void USART::printTo(const unsigned char data){
//	send(data);
//}
