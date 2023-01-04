file(GLOB_RECURSE csrc *.c)
file(GLOB_RECURSE cxxsrc *.cpp)
idf_component_register(SRCS ${csrc} ${cxxsrc}  
                       INCLUDE_DIRS ".")

spiffs_create_partition_image(storage ../tonedata FLASH_IN_PROJECT)