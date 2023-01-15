#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include "./fm_lib/timbre_manager.h"

//Constants
const char program_file_path[] = "/spiffs/program.bin"; //Path for program data

//Functions
void load_all_timbre_from_file(su_synth::fm::timbre_manager *tm,const char *filename);

#endif