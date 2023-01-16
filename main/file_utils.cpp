#include <cstdio>

#include "file_utils.h"
#include "esp_log.h"
#include "esp_spiffs.h"

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

void fs_init(void){
    esp_vfs_spiffs_conf_t fs_config;
    const char fs_base_path[] = "/spiffs"; //Base path of configuration files
    fs_config.base_path = fs_base_path,
    fs_config.partition_label = NULL;
    fs_config.max_files = 5;
    fs_config.format_if_mount_failed = false;

    esp_err_t ret = esp_vfs_spiffs_register(&fs_config);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE("FS", "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE("FS", "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE("FS", "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
    
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE("FS", "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI("FS", "Partition size: total: %d, used: %d", total, used);
    }
}

void fs_deinit(void){
    esp_vfs_spiffs_unregister(NULL);
}