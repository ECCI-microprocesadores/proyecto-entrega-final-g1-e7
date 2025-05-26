#pragma config FOSC = INTIO67
#pragma config PLLCFG = OFF
#pragma config PRICLKEN = ON
#pragma config WDTEN = OFF
#pragma config PWRTEN = OFF
#pragma config BOREN = OFF
#pragma config MCLRE = EXTMCLR
#pragma config PBADEN = OFF

#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdlib.h>
#include "i2c.h"
#include "i2c_lcd.h"

#define BTN1 PORTDbits.RD0
#define BTN2 PORTDbits.RD1
#define BUZZER LATCbits.LATC0
#define TRIS_BUZZER TRISCbits.TRISC0

typedef enum { MODO_NORMAL, MODO_TRYHARD } ModoJuego;

const char* categorias[] = {
    "Animal", "Color", "Verduras", "Frutas", "Pais", "Nombre", "Cosas", "Comida", "Ciudades", "Profesiones", "Apellidos", "Partes del cuerpo", "Objetos Casa", "Bebidas", "Marcas", "Prendas", "Deportes", "Peliculas", "Dibujos Animados", "Series TV", "Cosas Escuela", "VideoJuegos", "Juguetes", "Famosos", "Equipos Futbol", "Palabra Ingles", "Cosas Frias", "Cosas Hot", "Instrumentos",
};
const char letras[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

unsigned char jugando = 0;
unsigned char sonido_activado = 0;
unsigned long tiempo_restante = 0;
unsigned long tick_ms = 0;
unsigned long ultimo_tictac = 0;

unsigned char btn1_ant = 1;
unsigned char btn2_ant = 1;

#define TIC_TAC_INTERVAL 50
#define TIEMPO_LIMITE 2300

// beep_simple genera un sonido de ~1kHz por duracion_ms milisegundos
void beep_simple(unsigned int duracion_ms) {
    unsigned int ciclos = duracion_ms * 2; // 2 medios ciclos por ms
    for (unsigned int i = 0; i < ciclos; i++) {
        BUZZER = 1;
        __delay_us(500);
        BUZZER = 0;
        __delay_us(500);
    }
}

// Tic tac: sonido corto, tipo "ametralladora"
void sonido_tictac() {
    for (int i = 0; i < 50; i++) {
        BUZZER = 1;
        __delay_us(30);
        BUZZER = 0;
        __delay_us(100);
    }
}

// Explosión: pitido agudo y prolongado
void sonido_explosion() {
    for (int i = 0; i < 800; i++) {
        BUZZER = 1;
        __delay_us(150);
        BUZZER = 0;
        __delay_us(20);
    }
}


unsigned char random_letra() {
    return letras[rand() % 26];
}

const char* random_categoria() {
    return categorias[rand() % (sizeof(categorias) / sizeof(categorias[0]))];
}

void mostrar_inicio(ModoJuego modo) {
    lcd_clear();
    lcd_set_cursor(0, 2);
    lcd_write_string("Game Basta");
    lcd_set_cursor(1, 0);
    lcd_write_string("Modo: ");
    lcd_write_string((modo == MODO_NORMAL) ? "Normal " : "Tryhard");
}

void mostrar_juego(const char* categoria, char letra) {
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_write_string(categoria);
    lcd_set_cursor(1, 7);
    lcd_write_char(letra);
}

void main(void) {
    TRISD0 = 1;
    TRISD1 = 1;
    ANSELD = 0x00;
    TRIS_BUZZER = 0;
    BUZZER = 0;

    I2C_init();
    lcd_init();
    srand(1234);

    ModoJuego modo = MODO_NORMAL;
    char letra_actual = 'A';
    const char* categoria_actual = "Animal";

    mostrar_inicio(modo);

    unsigned char explosion_sonada = 0;

    while (1) {
        // FLANCO BTN1
        if (!BTN1 && btn1_ant) {
            __delay_ms(20);
            if (!BTN1) {
                if (!jugando) {
                    modo = (modo == MODO_NORMAL) ? MODO_TRYHARD : MODO_NORMAL;
                    mostrar_inicio(modo);
                } else {
                    letra_actual = random_letra();
                    if (modo == MODO_TRYHARD)
                        categoria_actual = random_categoria();
                    mostrar_juego(categoria_actual, letra_actual);
                    tiempo_restante = TIEMPO_LIMITE;
                    ultimo_tictac = tick_ms;
                    explosion_sonada = 0;
                }
            }
        }
        btn1_ant = BTN1;

        // FLANCO BTN2
        if (!BTN2 && btn2_ant) {
            __delay_ms(20);
            if (!BTN2) {
                if (!jugando) {
                    jugando = 1;
                    sonido_activado = 1;
                    tick_ms = 0;
                    tiempo_restante = TIEMPO_LIMITE;
                    ultimo_tictac = 0;
                    letra_actual = random_letra();
                    categoria_actual = random_categoria();
                    mostrar_juego(categoria_actual, letra_actual);
                    explosion_sonada = 0;
                } else {
                    jugando = 0;
                    sonido_activado = 0;
                    mostrar_inicio(modo);
                }
            }
        }
        btn2_ant = BTN2;

        // Sonido y tiempo
        if (jugando && sonido_activado) {
            if (tiempo_restante > 0) {
                if (tick_ms - ultimo_tictac >= TIC_TAC_INTERVAL) {
                    sonido_tictac();
                    ultimo_tictac = tick_ms;
                }
                tiempo_restante--;
            } else {
                if (!explosion_sonada) {
                    sonido_explosion();
                    explosion_sonada = 1;
                    jugando = 0;
                    sonido_activado = 0;
                    mostrar_inicio(modo);
                }
            }
        }

        __delay_ms(1);
        tick_ms++;
    }
}
