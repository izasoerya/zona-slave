#include "inmp.h"

INMP441::INMP441() {}
INMP441::~INMP441() {}

bool INMP441::begin()
{
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 44100,
        .bits_per_sample = i2s_bits_per_sample_t(16),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false};

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = -1,
        .data_in_num = I2S_SD};

    i2s_set_pin(I2S_NUM_0, &pin_config);

    i2s_zero_dma_buffer(I2S_NUM_0);

    return true; // Replace with appropriate return value
}

int32_t INMP441::read()
{
    const int sample_count = 64;
    int32_t samples[sample_count];
    int32_t sum = 0;
    size_t bytes_read;
    i2s_read(I2S_NUM_0, (void *)samples, sample_count * sizeof(int32_t), &bytes_read, portMAX_DELAY);
    for (int i = 0; i < 64; i++)
    {
        Serial.println(samples[i]);
    }
    return (sum / 64); // Replace with appropriate return value
}