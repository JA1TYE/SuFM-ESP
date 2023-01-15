#include <cstdio>

#include "file_utils.h"
#include "esp_log.h"

#include "./fm_lib/timbre_manager.h"



void load_all_timbre_from_file(su_synth::fm::timbre_manager *tm,const char *filename){
    FILE* f = fopen(filename,"r");
    if (f == NULL) {
        ESP_LOGI("FS", "Failed to open program file");
        return;
    }

    std::uint8_t buf [256];
    for(int i = 0;i < MAX_TIMBRE_PROGRAMS;i++){
        fread(buf,1,sizeof(su_synth::fm::save_param_t),f);
        tm->import_timbre(i,(su_synth::fm::save_param_t*)buf);
    }
    ESP_LOGI("FS","Load done!");
    fclose(f);
}