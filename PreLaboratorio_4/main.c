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
volatile uint8_t valor_adc = 0;
volatile uint8_t digitos[2];
volatile uint8_t digito_actual = 0;

// Tabla 7 segmentos invertida para ÁNODO COMÚN (segmentos activos en LOW)
// Conexión: PB5=a, PC0=b, PC1=c, PC2=d, PC3=e, PC4=f, PC5=g
const uint8_t tabla_7seg[] = {
	// bits: g f e d c b a (0=encendido)
	0xC0, // 0: 1 1 0 0 0 0 0 (0x40 invertido)
	0xF9, // 1: 1 1 1 1 1 0 0 (0x9F invertido)
	0xA4, // 2: 1 0 1 0 0 1 0 (0x25 invertido)
	0xB0, // 3: 1 0 1 1 0 0 0 (0x0D invertido)
	0x99, // 4: 1 0 0 1 1 0 0 (0x99 invertido)
	0x92, // 5: 1 0 0 1 0 0 1 (0x49 invertido)
	0x82, // 6: 1 0 0 0 0 0 1 (0x41 invertido)
	0xF8, // 7: 1 1 1 1 1 0 0 (0x1F invertido)
	0x80, // 8: 1 0 0 0 0 0 0 (0x01 invertido)
	0x90, // 9: 1 0 0 1 0 0 0 (0x09 invertido)
	0x88, // A: 1 0 0 0 1 0 0 (0x11 invertido)
	0x83, // B: 1 0 0 0 0 0 1 (0xC1 invertido)
	0xC6, // C: 1 1 0 0 0 1 1 (0x63 invertido)
	0xA1, // D: 1 0 1 0 0 0 1 (0x85 invertido)
	0x86, // E: 1 0 0 0 0 1 1 (0x61 invertido)
	0x8E  // F: 1 0 0 0 1 1 1 (0x71 invertido)
};

void configurar_ADC() {
	ADMUX = (1 << REFS0) | (1 << ADLAR) | POTENCIOMETRO; // ADC7 (A7)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}



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