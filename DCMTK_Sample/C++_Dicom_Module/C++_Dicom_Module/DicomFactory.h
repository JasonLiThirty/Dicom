#ifndef DICOM_FACTORY_H
#define DICOM_FACTORY_H

#include "DicomIF.h"

class IDicom;

static IDicom* s_dicom = nullptr;

__declspec(dllexport) void CreateDicomProcessor();

__declspec(dllexport) IDicom* GetDicomProcessor();

__declspec(dllexport) void DeleteDicomProcessor();

#endif
