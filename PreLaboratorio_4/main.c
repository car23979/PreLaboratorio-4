/*
 * PostLaboratorio_4.c
 *
 * Created: 03/04/2025 09:30:46 a. m.
 * Author : David Carranza
 */ 

// Encabezado
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Constantes
#define INC_PIN    PB0	 // Botón Incremento (D8)
#define DEC_PIN    PB1	 // Botón Decremento (D9)
#define DIGITO1    PB2   // Control dígito 1 (D10)
#define DIGITO2    PB3   // Control dígito 2 (D11)
#define ALARMA     PB4   // LED alarma (D12)
#define POTENCIOMETRO 7  // ADC7 (A7)

volatile uint8_t contador = 0;

// Puertos
void configurar_puertos() {
	DDRD = 0xFF;    // Todos los pines D como salidas
	DDRB &= ~((1 << INC_PIN) | (1 << DEC_PIN)); // PB0 y PB1 como entradas
	PORTB |= (1 << INC_PIN) | (1 << DEC_PIN);   // Pull-ups activados
}

void actualizar_leds() {
	PORTD = contador;
}

int main() {
	configurar_puertos();
	uint8_t estado_inc = 1, estado_dec = 1;

	while(1) {
		uint8_t inc_actual = !(PINB & (1 << INC_PIN));
		uint8_t dec_actual = !(PINB & (1 << DEC_PIN));  // Variable ahora usada

		// Lógica para incremento
		if (inc_actual && !estado_inc) {
			_delay_ms(20);
			if (!(PINB & (1 << INC_PIN))) {
				contador++;
			}
		}
		estado_inc = inc_actual;

		// Lógica para decremento (corregido)
		if (dec_actual && !estado_dec) {  // Ahora usa las variables correctas
			_delay_ms(20);
			if (!(PINB & (1 << DEC_PIN))) {
				contador--;
			}
		}
		estado_dec = dec_actual;  // Variable ahora usada

		actualizar_leds();
	}
}