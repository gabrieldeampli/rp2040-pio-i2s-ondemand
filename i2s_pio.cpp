
#include "i2s_pio.h"

// Incluís el .pio.h generado con pioasm
#include "i2s_tda1543_stereo_on_demand.pio.h"

#include "hardware/pio.h"
#include "hardware/clocks.h"

// Estado interno del módulo (intencionalmente oculto)
static PIO  s_pio = pio0;
static uint s_sm  = 0;

void i2s_pio_init(
    uint32_t bck_pin,
    uint32_t ws_pin,
    uint32_t data_pin,
    uint32_t bck_hz
) {
    // Regla innegociable del programa PIO
    // (mejor fallar temprano que mal funcionar)
    if (ws_pin != bck_pin + 1) {
        while (true) {
            tight_loop_contents(); // error fatal de configuración
        }
    }

    const uint offset =
        pio_add_program(s_pio, &i2s_tda1543_stereo_on_demand_program);

    pio_sm_config c =
        i2s_tda1543_stereo_on_demand_program_get_default_config(offset);

    // DATA
    sm_config_set_out_pins(&c, data_pin, 1);

    // SIDESET: BCK y WS
    sm_config_set_sideset_pins(&c, bck_pin);

    // MSB-first, 16 bits por palabra
    sm_config_set_out_shift(
        &c,
        false,   // shift left (MSB first)
        false,   // no autopull
        16
    );

    // FIFO TX unido (8 palabras)
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);

    // GPIO setup
    pio_gpio_init(s_pio, data_pin);
    pio_gpio_init(s_pio, bck_pin);
    pio_gpio_init(s_pio, ws_pin);

    // Carga del SM
    pio_sm_init(s_pio, s_sm, offset, &c);

    const uint32_t pin_mask =
        (1u << data_pin) |
        (1u << bck_pin)  |
        (1u << ws_pin);

    pio_sm_set_pins_with_mask(s_pio, s_sm, 0, pin_mask);
    pio_sm_set_pindirs_with_mask(s_pio, s_sm, pin_mask, pin_mask);

    // Reloj:
    //
    // El programa usa 2 instrucciones por bit:
    //   f_sm = 2 * bck_hz
    //
    // clk_sys puede ser 250 MHz (tu caso)
    const uint32_t clk = clock_get_hz(clk_sys);
    const float div = (float)clk / (2.0f * (float)bck_hz);

    pio_sm_set_clkdiv(s_pio, s_sm, div);

    pio_sm_set_enabled(s_pio, s_sm, true);
}

void i2s_pio_write_stereo_16(int16_t left, int16_t right) {
    // Alineación MSB (16 bits útiles)
    uint32_t L = (uint16_t)left;
    uint32_t R = (uint16_t)right;

    L <<= 16;
    R <<= 16;

    // Orden IMPORTANTE: primero L, luego R
    // El PIO está esperando dos pull block
    while (pio_sm_is_tx_fifo_full(s_pio, s_sm)) {
        // rara vez ocurre
    }

    pio_sm_put_blocking(s_pio, s_sm, L);
    pio_sm_put_blocking(s_pio, s_sm, R);
}
