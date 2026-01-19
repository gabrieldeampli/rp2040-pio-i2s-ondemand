# rp2040-pio-i2s-ondemand
Deterministic, on‑demand I2S stereo output for RP2040 using PIO (no streaming, no DMA)

# I2S_PIO_RP2040

Deterministic, on-demand I2S stereo output for RP2040 using PIO.

## Features
- I2S 16-bit stereo
- On-demand (no continuous streaming)
- Uses PIO (no DMA)
- Designed for precise control and synthesis
- Tested with TDA1543 DAC

## Usage

```cpp
#include <i2s_pio.h>

constexpr uint32_t BCK_PIN  = 1;
constexpr uint32_t WS_PIN   = 2;       // MUST be BCK_PIN + 1
constexpr uint32_t DATA_PIN = 3;
constexpr uint32_t BCK_HZ   = 32*44100;  // BIT CLOCK FREQ = 32 * SAMPLE FREQ IN HZ

void setup() {
    i2s_pio_init(BCK_PIN, WS_PIN, DATA_PIN, BCK_HZ);
}

void loop() {
    i2s_pio_write_stereo_16(1000, 1000);
}
