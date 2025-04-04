/*
 * PreLaboratorio_4.c
 *
 * Created: 04/04/2025 09:30:46 a. m.
 * Author : David Carranza
 */ 

// Encabezado
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Constantes
#define INC_PIN    PB0   // Botón incremento (D8)
#define DEC_PIN    PB1   // Botón decremento (D9)

volatile uint8_t contador = 0;

// Puertos
void configurar_puertos() {
	DDRD = 0xFF;    // PD0-PD7 como salidas (LEDs)
	DDRB &= ~((1 << INC_PIN) | (1 << DEC_PIN)); // PB0-PB1 como entradas
	PORTB |= (1 << INC_PIN) | (1 << DEC_PIN);   // Activar pull-ups
}

void actualizar_leds() {
	PORTD = contador; // Mostrar el contador directamente en PD0-PD7
}

// Configuración Puertos
int main() {
	configurar_puertos();
	uint8_t estado_inc = 1, estado_dec = 1;
	
	while(1) {
	 // Leer estados de los botones (pull-up activado: 0 = presionado)
	 uint8_t inc_actual = !(PINB & (1 << INC_PIN));
	 uint8_t dec_actual = !(PINB & (1 << DEC_PIN));