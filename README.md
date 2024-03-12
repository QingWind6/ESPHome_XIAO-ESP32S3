# ESPHome custom components for XIAO-ESP32S3

## ESPHome ReSpeakerV3.yaml：
```ESPHome
esphome:
  name: esp32s3
  friendly_name: ReSpeakerv3
  platformio_options:
    board_build.flash_mode: dio
    board_build.mcu: esp32s3

esp32:
  board: esp32-s3-devkitc-1
  variant: esp32s3
  framework:
    type: esp-idf
    version: recommended

logger:
api:
ota:

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

# improv_serial:

# esp32_improv:
#   authorizer: none

# captive_portal:

external_components:
  - source: github://QingWind6/ESPHome_XIAO-ESP32S3

i2s_audio_xiao:
  i2s_lrclk_pin: GPIO7
  i2s_bclk_pin: GPIO8
  i2s_mclk_pin: GPIO9


microphone:
  - platform: i2s_audio_xiao
    id: xiao_mic
    adc_type: external
    i2s_din_pin: GPIO44
    pdm: false
    bits_per_sample: 32bit
    channel: left

speaker:
  - platform: i2s_audio_xiao
    id: xiao_speaker
    dac_type: external
    i2s_dout_pin: GPIO43
    mode: stereo

   
voice_assistant:
  microphone: xiao_mic
  use_wake_word: true
  noise_suppression_level: 0
  auto_gain: 0dBFS
  volume_multiplier: 1
  speaker: xiao_speaker
  id: assist
  on_listening:
    - light.turn_on:
        id: led
        effect: "Fast Breathing Light"
        # effect: none
  on_stt_vad_end:
    - light.turn_on:
        id: led
        # effect: "Fast Breathing Light"
        effect: none
  on_tts_start:
    - light.turn_on:
        id: led
        effect: none
  on_end:
    - delay: 100ms
    - wait_until:
        not:
          speaker.is_playing:
    - light.turn_off:
        id: led
    # - script.execute: reset_led
  on_error:
    - light.turn_off:
        id: led
        # effect: none
    # - delay: 1s
    # - script.execute: reset_led
  on_client_connected:
    - if:
        condition:
          switch.is_on: use_wake_word
        then:
          - voice_assistant.start_continuous
  on_client_disconnected:
    - if:
        condition:
          switch.is_on: use_wake_word
        then:
          - voice_assistant.stop

light:
  - platform: monochromatic
    id: led
    name: "Desk Lamp"
    output: light_output
    effects:
      - pulse:
          name: "Slow Breathing Light"
          transition_length: 5s  # 缓慢呼吸灯，渐变时间较长
      - pulse:
          name: "Fast Breathing Light"
          transition_length: 1s  # 快速呼吸灯，渐变时间较短

output:
  - platform: ledc
    id: light_output
    pin: GPIO21
    inverted: true


script:
  - id: reset_led
    then:
      - if:
          condition:
            - switch.is_on: use_wake_word
            - switch.is_on: use_listen_light
          then:
            - light.turn_on:
                id: led
                effect: none

          else:
            - light.turn_off: 
               id: led

switch:
  - platform: template
    name: Use wake word
    id: use_wake_word
    optimistic: true
    restore_mode: RESTORE_DEFAULT_ON
    entity_category: config
    on_turn_on:
      - lambda: id(assist).set_use_wake_word(true);
      - if:
          condition:
            not:
              - voice_assistant.is_running
          then:
            - voice_assistant.start_continuous
    on_turn_off:
      - voice_assistant.stop
      - lambda: id(assist).set_use_wake_word(false);

  - platform: template
    name: Use Listen Light
    id: use_listen_light
    optimistic: true
    restore_mode: RESTORE_DEFAULT_ON
    entity_category: config
    on_turn_on:
      - script.execute: reset_led
    on_turn_off:
      - script.execute: reset_led
```
