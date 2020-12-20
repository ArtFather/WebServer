#ifndef __CONFIG_H_
#define __CONFIG_H_ 1

#include "Pass.h"
#include <ArduinoJson.h>
#include <FileSystem.cpp>

const char *filename = "/config.txt";

void saveConfiguration(const char *filename, const Config &config);
void loadConfiguration(const char *filename, Config &config);
void printFile(const char *filename);

#endif