#include "i2s_audio_xiao.h"

#ifdef USE_ESP32

#include "esphome/core/log.h"

namespace esphome {
namespace i2s_audio_xiao {

static const char *const TAG = "i2s_audio_xiao";

void I2SAudioComponent::setup() {
  static i2s_port_t next_port_num = I2S_NUM_0;

  if (next_port_num >= I2S_NUM_MAX) {
    ESP_LOGE(TAG, "Too many I2S Audio components!");
    this->mark_failed();
    return;
  }

  this->port_ = next_port_num;
  next_port_num = (i2s_port_t) (next_port_num + 1);

  ESP_LOGCONFIG(TAG, "Setting up I2S Audio...");
}

}  // namespace i2s_audio_xiao
}  // namespace esphome

#endif  // USE_ESP32
