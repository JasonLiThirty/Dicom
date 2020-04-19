// C++_DICOM_Sample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv.hpp>
#include "DicomIF.h"
#include "DicomFactory.h"
#include "DicomUtils.h"

#include <iostream>

void ReadDicomTags(std::string file)
{
	if (!GetDicomProcessor()->Read(file))
	{
		std::cout << "Read Dicom File Failed" << file.c_str() << std::endl;
		return;
	}

	GetDicomProcessor()->MetaInfoTags();
	GetDicomProcessor()->DataSetTags();
}

void ReadDicom(std::string file)
{
	PrivateData data;
	data.GroupTag = 0x4001;
	data.ElementTag = 0x1000;
	GetDicomProcessor()->PrivateDataInfo().push_back(data);

	if (!GetDicomProcessor()->Read(file))
	{
		std::cout << "Read Dicom File Failed" << file.c_str() << std::endl;
		return;
	}

	for (auto &item : GetDicomProcessor()->PrivateDataInfo())
	{
		std::ios::fmtflags fmt(std::cout.flags());
		std::cout << "Private Data:(0x" << std::hex << item.GroupTag << ",0x" << item.ElementTag << ")" << item.Value << std::endl;
		std::cout.flags(fmt);
	}

	/*std::cout << "Patient Name:" << GetDicomProcessor()->PatientInfo()->PatientsName << std::endl;
	std::cout << "Patient ID:" << GetDicomProcessor()->PatientInfo()->PatientID << std::endl;

	std::cout << "Study Instance UID:" << GetDicomProcessor()->StudyInfo()->StudyInstanceUID << std::endl;

	std::cout << "Serial Date:" << DatetoString(GetDicomProcessor()->SeriesInfo()->SeriesDateTime) << std::endl;
	std::cout << "Serial Time:" << TimetoString(GetDicomProcessor()->SeriesInfo()->SeriesDateTime) << std::endl;
	std::cout << "Body Part Examined:" << GetDicomProcessor()->SeriesInfo()->BodyPartExamined << std::endl;

	std::cout << "Rows:" << GetDicomProcessor()->ImageInfo()->Rows << std::endl;
	std::cout << "Columns:" << GetDicomProcessor()->ImageInfo()->Columns << std::endl;*/
}

void SaveDicom(std::string file)
{
	/*GetDicomProcessor()->PatientInfo()->PatientID = "88888";
	GetDicomProcessor()->StudyInfo()->StudyInstanceUID = GenerateUniqueId(UID_Study);
	GetDicomProcessor()->SeriesInfo()->SeriesDateTime = std::chrono::system_clock::now();*/

	PrivateData data;
	data.GroupTag = 0x4001;
	data.ElementTag = 0x1000;
	data.Name = "pravite1";
	data.VR = GetDataVR(DataVRType_UT);
	data.Value = "Private Test...";
	GetDicomProcessor()->PrivateDataInfo().push_back(data);


	if (!GetDicomProcessor()->Save(file))
	{
		std::cout << "Save Dicom File Failed: " << file.c_str() << std::endl;
		return;
	}
}

int main()
{
	CreateDicomProcessor();

	//std::string readFile = "D:\\214 - Advanced Technology\\DICOM\\Src\\Module_Sample\\brain_005.dcm";
	std::string readFile = "D:\\214 - Advanced Technology\\DICOM\\Src\\Module_Sample\\MRBRAIN.dcm";
	//std::string readFile = "D:\\Learning\\brain_005.dcm";
	std::string saveFile = "D:\\214 - Advanced Technology\\DICOM\\Src\\Module_Sample\\brain_005_New.dcm";

	//ReadDicom(readFile);
	//SaveDicom(saveFile);

	//if (!GetDicomProcessor()->ShowImage(readFile, ImagePattern_Flip))
	//if (!GetDicomProcessor()->ShowImage(readFile, ImagePattern_Rotate, 90));
	//if (!GetDicomProcessor()->ShowImage(readFile, ImagePattern_Polarity))
	//if (!GetDicomProcessor()->ShowImage(readFile))
	/*{
	std::cout << "Read Dicom Image Failed: " << readFile.c_str() << std::endl;
	return -1;
	}*/


	//
		//if (!GetDicomProcessor()->SavetoBMP(readFile, ImagePattern_Flip))
	if (!GetDicomProcessor()->SavetoBMP(readFile, ImagePattern_Rotate, 90))
	//if (!GetDicomProcessor()->SavetoBMP(readFile, ImagePattern_Polarity))
	//if (!GetDicomProcessor()->SavetoBMP(readFile))
	{
		std::cout << "Read Dicom Image Failed: " << readFile.c_str() << std::endl;
		return -1;
	}


	DeleteDicomProcessor();
    return 0;
}

