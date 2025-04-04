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
#define INC_PIN    PB0   // Bot�n incremento (D8)
#define DEC_PIN    PB1   // Bot�n decremento (D9)


// 