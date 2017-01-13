#ifndef __USART_H__
#define __USART_H__


/**
 * \file
 * \brief Biblioteca pra utlização das portas seriais do ATmega328P
 *
 * Biblioteca que permite configurar e utilizar as 4 portas seriais
 * presentes no microcontrolador ATmega2560.
 *
 * Entre possíveis configurações, tem-se:
 * * Baudrate da porta
 * * Número da porta que deseja utilizar (0,1,2,3);
 * * Modo de recepção, transmissão ou recepção/tranmissão;
 * * Tamanho do frame (5, 6, 7, 8 ou 9);
 * * Modo de transmissão (assíncrono, síncrono ou master SPI);
 * * Bits de paridade (odd, even ou desativado);
 * * Bits de parada (1 ou 2);
 * * Habilitar/desabilitar interrupção de recepção.
 *
 * \author Rafael Cunha <rnascunha@gmail.com>
 * \date 10/06/2016
 */

/**
 * \defgroup usart USART
 *
 * \brief Definição da classe, tipos e macros referente as portas USART do ATmega2560.
 * @{
 */

#include <avr/io.h>
//#include "print.h"
/**
 * \brief Define a direção dos dados.
 *
 * Define se será utilizada apenas recepção, apenas transmissão ou
 * recepção e transmissão dos dados.
 */
typedef enum USARTrxtx{
	USART_RXTX = 0,//!< Recepção e transmissão
	USART_RX,      //!< Apenas recepção
	USART_TX       //!< Apenas transmissão
}USARTrxtx;

/**
 * \brief Define o modo de operação da porta serial
 *
 * Os modos de operação possíveis são assíncrono, síncrono ou funcionar como master spi.
 */
typedef enum USARTmode{
	USART_ASYNCHRONOUS = 0,//!< Modo assíncrono
	USART_SYNCHRONOUS,     //!< Modo síncrono
	USART_MASTER_SPI       //!< Modo master SPI
}USARTmode;

/**
 * \brief Habilita ou não o bit para dublicar a velocidade do prescaler.
 *
 * Habilita ou não o bit U2Xn para duplicar a velocidade do prescaler.
 */
typedef enum USART2xspeed{
	USART_ASYNC_NORMAL,			///< Para o modo normal de operação
	USART_ASYNC_2XSPEED,			///< Duplicando a velocidade
	USART_SYNC_MASTER				///< Para o modo master SPI
}USART2xspeed;

/**
 * \brief Define se será utilizado o bit de paridade na comunicação.
 *
 * Define se o bit de paridade será utilizado, e em caso positivo, como será
 * computada a paridade (par ou impar)
 */
typedef enum USARTparitiy{
	USART_DISABLE_PARITY = 0,//!< Desabilita a paridade
	USART_ODD,               //!< Paridade impar
	USART_EVEN               //!< Paridade par
}USARTparity;

/**
 * \brief Define o número de bits para terminar um frame
 *
 * Define 1 ou 2 bits como final no frame
 */
typedef enum USARTstopbit{
	USART_STOPBIT1 = 0,//!< 1 bit de parada
	USART_STOPBIT2     //!< 2 bits de parada
}USARTstopbit;

/**
 * \brief Define o tamanho do frame que será recebido/transmitido.
 */
typedef enum USARTframe{
	USART_FRAME5 = 0,//!< Frame de 5 bits
	USART_FRAME6,    //!< Frame de 6 bits
	USART_FRAME7,    //!< Frame de 7 bits
	USART_FRAME8,    //!< Frame de 8 bits
	USART_FRAME9     //!< Frame de 9 bits
}USARTframe;

/**
 * \brief Habilita ou não a interrupção de recepção de dados
 */
typedef enum USARTrxinterrupt{
	USART_INT_ENABLE = 0,		///< Habilita a interrupção
	USART_INT_DISABLE    		///< Desabilita a interrupção
}USARTrxinterrupt;

/**
 * \brief Classe para configuração e utilização das portas seriais
 *
 * Nesta classe é possível definir qual porta será utilizada e fazer todas
 * as configurações relevantes a mesma.
 */
class USART{

	public:
		/**
		 * Construtor da porta serial. Seleciona e configura os parâmetros de funcionalidade e operação da porta.
		 *
		 * @param [in] baudrate Baudrate da porta.
		 * @param [in] rxinterrupt \b Opcional. Habilita a interrupção de recepção da porta. Valor padrão: \a DISABLE.
		 * @param [in] speed \b Opcional. Define o bit de duplicação de velocidade do \a prescaler. Valor padrão: \a ASYNC_NORMAL.
		 * @param [in] rxtx \b Opcional. Define a direção da comunicação. Valor padrão: \a RXTX.
		 * @param [in] mode \b Opcional. Define o modo de operaçao da porta. Valor padrão: \a ASYNCHRONOUS.
		 * @param [in] parity \b Opcional. Define o bit de paridade do frame. Valor padrão: \a DISABLE_PARITY.
		 * @param [in] stopbit \b Opcional. Define o número de stopbits do frame. Valor padrão: \a STOPBIT1.
		 * @param [in] frame \b Opcional. Define o tamanho do frame de comunicação da porta. Valor padrão: \a FRAME8.
		 */
		USART(uint32_t baudrate,
				USARTrxinterrupt rxinterrupt = USART_INT_DISABLE,
				USART2xspeed speed = USART_ASYNC_NORMAL,
				USARTrxtx rxtx = USART_RXTX,
				USARTmode mode = USART_ASYNCHRONOUS,
				USARTparitiy parity = USART_DISABLE_PARITY,
				USARTstopbit stopbit = USART_STOPBIT1,
				USARTframe frame = USART_FRAME8);

		//void (USART::*send)(uint8_t data);
		//uint8_t (USART::*receive)(void);
		/**
		 * \brief Envia um byte pela porta serial.
		 *
		 * @param [in] data \a Byte que será envido
		 */
		void send(uint8_t data);
		/**
		 * \brief Recebe um \a byte da porta serial.
		 * @return Valor recebido pela porta serial.
		 */

		void sendln(char data);
		void sendcr(char data);

		uint8_t receive(void);

		/**
		 * \brief Envia uma string pela porta serial.
		 * @param [in] str String que será enviada.
		 * @param [in] limit Caracter que define o final da \a string.
		 */
		void send(const char *str, char limit = '\0');

		//void printTo(const unsigned char data);
};

/** @} */
#endif //__USART_H__
