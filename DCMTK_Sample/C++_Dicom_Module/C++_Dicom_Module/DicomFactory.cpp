#include "DicomFactory.h"
#include "DicomProcessor.h"
#include <iostream>

void CreateDicomProcessor()
{
	if (nullptr == s_dicom)
	{
		s_dicom = new DicomProcessor();
	}
}

IDicom* GetDicomProcessor()
{
	return s_dicom;
}

void DeleteDicomProcessor()
{
	if (nullptr != s_dicom)
	{
		delete s_dicom;
		s_dicom = nullptr;
	}
}