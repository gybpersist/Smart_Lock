# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/project_ruanjian/Espressif/frameworks/esp-idf-v5.3.1/components/bootloader/subproject"
  "E:/project_ruanjian/AAA_Workspace/AAA_ESP32_project/Smart_Lock/build/bootloader"
  "E:/project_ruanjian/AAA_Workspace/AAA_ESP32_project/Smart_Lock/build/bootloader-prefix"
  "E:/project_ruanjian/AAA_Workspace/AAA_ESP32_project/Smart_Lock/build/bootloader-prefix/tmp"
  "E:/project_ruanjian/AAA_Workspace/AAA_ESP32_project/Smart_Lock/build/bootloader-prefix/src/bootloader-stamp"
  "E:/project_ruanjian/AAA_Workspace/AAA_ESP32_project/Smart_Lock/build/bootloader-prefix/src"
  "E:/project_ruanjian/AAA_Workspace/AAA_ESP32_project/Smart_Lock/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/project_ruanjian/AAA_Workspace/AAA_ESP32_project/Smart_Lock/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/project_ruanjian/AAA_Workspace/AAA_ESP32_project/Smart_Lock/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
