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

uint8_t leer_ADC() {
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADCH;
}

void configurar_timer() {
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
	OCR1A = 156;
	TIMSK1 = (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
	uint8_t segmentos = tabla_7seg[digitos[digito_actual]];
	
	// Control de segmentos (activar en LOW)
	PORTB = (PORTB & 0x1F) | ((segmentos & 0x01) << 5); // a (PB5)
	PORTC = (segmentos >> 1) & 0x3F; // b-g (PC0-PC5)
	
	// Multiplexación de dígitos (activar en HIGH para ánodo común)
	if (digito_actual == 0) {
		PORTB |= (1 << DIGITO1);
		PORTB &= ~(1 << DIGITO2);
		} else {
		PORTB |= (1 << DIGITO2);
		PORTB &= ~(1 << DIGITO1);
	}
	digito_actual = !digito_actual;
}

int main() {
	DDRD = 0xFF;   // LEDs contador (PD0-PD7)
	DDRB |= (1 << DIGITO1) | (1 << DIGITO2) | (1 << ALARMA) | (1 << PB5);
	DDRC |= 0x3F;  // Segmentos b-g (PC0-PC5)
	
	// Pull-ups para botones
	PORTB |= (1 << INC_PIN) | (1 << DEC_PIN);
	
	configurar_ADC();
	configurar_timer();
	sei();
	
	uint8_t estado_inc = 1, estado_dec = 1;
	
	while(1) {
		// Antirrebote
		uint8_t inc_actual = !(PINB & (1 << INC_PIN));
		uint8_t dec_actual = !(PINB & (1 << DEC_PIN));
		
		if (inc_actual && !estado_inc) {
			_delay_ms(20);
			if (!(PINB & (1 << INC_PIN))) contador++;
		}
		estado_inc = inc_actual;
		
		if (dec_actual && !estado_dec) {
			_delay_ms(20);
			if (!(PINB & (1 << DEC_PIN))) contador--;
		}
		estado_dec = dec_actual;
		
		// Actualizar LEDs
		PORTD = contador;
		
		// Procesar ADC
		valor_adc = leer_ADC();
		digitos[0] = (valor_adc >> 4) & 0x0F;
		digitos[1] = valor_adc & 0x0F;
		
		// Activar alarma si ADC es mayou que contador
		if (valor_adc > contador) PORTB |= (1 << ALARMA);
		else PORTB &= ~(1 << ALARMA);
	}
}