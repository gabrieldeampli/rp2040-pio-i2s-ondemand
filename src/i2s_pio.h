#include <cstdint>

#pragma once

#include <stdint.h>

// Inicializa el I2S por PIO (estéreo, 16-bit, on-demand)
//
// bck_pin : pin del Bit Clock
// ws_pin  : pin del Word Select (debe ser bck_pin + 1)
// data_pin: pin de datos
// bck_hz  : frecuencia del bit clock (ej: 32 * 44100)
void i2s_pio_init(
    uint32_t bck_pin,
    uint32_t ws_pin,
    uint32_t data_pin,
    uint32_t bck_hz
);

// Envía UN frame I2S estéreo (16-bit por canal)
//
// left  : muestra canal izquierdo
// right : muestra canal derecho
void i2s_pio_write_stereo_16(int16_t left, int16_t right);
