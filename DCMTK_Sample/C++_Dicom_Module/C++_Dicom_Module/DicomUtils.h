#ifndef _DICOM_UTILS_H
#define _DICOM_UTILS_H

#include <ctime>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <time.h>

#include "DicomInfo.h"

__declspec(dllexport) std::string GenerateUniqueId(UID_Type type);
__declspec(dllexport) std::string GetDataVR(DataVR_Type type);
__declspec(dllexport) std::chrono::system_clock::time_point StringtoDate(std::string date);
__declspec(dllexport) std::chrono::system_clock::time_point StringtoDateTime(std::string date, std::string time);
__declspec(dllexport) std::string DatetoString(std::chrono::system_clock::time_point timePoint);
__declspec(dllexport) std::string TimetoString(std::chrono::system_clock::time_point timePoint);
__declspec(dllexport) std::string WeighttoString(float weight);
__declspec(dllexport) std::string AgetoString(int age, AgeUnit_Type type);
__declspec(dllexport) AgeUnit_Type StringtoAgeUnit(std::string type);
__declspec(dllexport) std::vector<std::string> GetMultiItem(const std::string &str);
__declspec(dllexport) void MultiItemtoString(std::string &str, const std::string &item);
__declspec(dllexport) double ScientificStringtoFloat(const char *str);
__declspec(dllexport) std::string GenerateImageName(std::string name, Image_Pattern pattern);

#endif