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

This function sends data over the I2S protocol on demand, one at a time. For example, when performing real-time signal processing, there is no recorded signal; the signal is calculated sample by sample and sent to a DAC. To avoid discontinuities, it is important that the time it takes to send a sample to the DAC is shorter than the time our signal processing program takes to calculate a new output. In this library, the write time must be specified through the bit rate. For example, if my program works at 25KHz, stereo data should be sent to the DAC at a bit rate of 2 channels * 16 bits * 25,000 Hz = 800,000 bps. We should set a number higher than that (for example, 1,000,000) to ensure that the data arrives complete in each cycle of the main program.

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
