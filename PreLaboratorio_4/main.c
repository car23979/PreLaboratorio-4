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
#define DIGITO1    PB2   // Control dígito 1 (D10)
#define DIGITO2    PB3   // Control dígito 2 (D11)
#define ALARMA     PB4   // LED alarma (D12)
#define POTENCIOMETRO 7  // ADC7 (A7