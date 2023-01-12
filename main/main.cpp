#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <math.h>
#include "./driver/codec/codec.h"
#include "./driver/codec/aic3204.h"
#include "./midi_impl.h"
#include "./fm_lib/fmtone.h"
#include "./fm_lib/synth_ctrl.h"
#include "./fm_lib/timbre_manager.h"
#include "./machine_config.h"

extern "C"{
    void app_main();
    void main_process(void *pvParameters);
}

void app_main(){
    xTaskCreatePinnedToCore(main_process, "main processing task", 32768, NULL, 5, NULL, 1);
}

void main_process(void *pvParameters){
    esp_err_t ret;
    ret = sucodec_init();
    aic3204_set_headphone_volume(AIC3204_BOTH,0);
    aic3204_set_line_out_volume(AIC3204_BOTH,4.0);
    aic3204_set_line_out_mute(AIC3204_BOTH,false);
    aic3204_set_dac_digital_volume(AIC3204_BOTH,0.0);
    sucodec_set_amp_mute(false);
    if(ret != ESP_OK)printf("ERR at sucodec_init\n");
    su_midi::midi_receiver_impl uart_midi(0xffff ^ (1 << 9));
    uint8_t uart_buf[10];
    int32_t out_buf[96];


    uart_port_t midi_port = MIDI_UART_SOURCE;
    uart_config_t pc_uart_config;
    uart_config_t midi_uart_config;

    pc_uart_config.baud_rate = 115200;
    pc_uart_config.data_bits = UART_DATA_8_BITS;
    pc_uart_config.parity    = UART_PARITY_DISABLE;
    pc_uart_config.stop_bits = UART_STOP_BITS_1;
    pc_uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    pc_uart_config.rx_flow_ctrl_thresh = 122;
    pc_uart_config.source_clk = UART_SCLK_DEFAULT;
    uart_param_config(UART_NUM_0, &pc_uart_config);
    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);

    midi_uart_config.baud_rate = 31250;
    midi_uart_config.data_bits = UART_DATA_8_BITS;
    midi_uart_config.parity    = UART_PARITY_DISABLE;
    midi_uart_config.stop_bits = UART_STOP_BITS_1;
    midi_uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    midi_uart_config.rx_flow_ctrl_thresh = 122;
    midi_uart_config.source_clk = UART_SCLK_DEFAULT;
    uart_set_pin(UART_NUM_1, UART_PIN_NO_CHANGE, 34,UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_param_config(UART_NUM_1, &midi_uart_config);
    uart_driver_install(UART_NUM_1, 1024, 0, 0, NULL, 0);
    su_synth::timbre_manager::init();
    su_synth::synth_controller::init();

    size_t length = 0;
    size_t ret_size;
    //Infinite loop
    while(1){
        for(int i = 0;i < 48;i++){
            uart_get_buffered_data_len(midi_port, &length);
            if(length > 0){
                int rlen = (length > 10)?10:length;
                uart_read_bytes(midi_port,uart_buf,rlen,1);
                for(int n = 0;n < rlen;n++){
                    uart_midi.parse_byte(uart_buf[n]);
                }
            }
            su_synth::synth_controller::calc(&out_buf[i*2]);     
        }
        ret = sucodec_write(out_buf,48*2*4,&ret_size,1000);
        sucodec_set_amp_mute(sucodec_is_hp_detected());
    }
}