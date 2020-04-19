#ifndef _DICOM_INFO_H
#define _DICOM_INFO_H


#include <string>
#include <chrono>
#include <list>
#include <stdint.h>
#include <vector>


#define PRIVATE_GROUP_NAME "Private Group"


enum UID_Type
{
	UID_Study = 1,
	UID_Series = 2,
	UID_Image = 3,
	UID_Other = 4,
};


enum AgeUnit_Type
{
	AgeUnit_Year = 1,
	AgeUnit_Month = 2,
	AgeUnit_Week = 3,
	AgeUnit_Day = 4
};


enum DataVR_Type
{
	DataVRType_CS = 1,
	DataVRType_SH,
	DataVRType_LO,
	DataVRType_ST,
	DataVRType_LT,
	DataVRType_UT,
	DataVRType_AE,
	DataVRType_PN,
	DataVRType_UI,
	DataVRType_DA,
	DataVRType_TM,
	DataVRType_DT,
	DataVRType_AS,
	DataVRType_IS,
	DataVRType_DS,
	DataVRType_SS,
	DataVRType_US,
	DataVRType_SL,
	DataVRType_UL,
	DataVRType_AT,
	DataVRType_FL,
	DataVRType_FD,
	DataVRType_OB,
	DataVRType_OW,
	DataVRType_OF,
	DataVRType_SQ,
	DataVRType_UN
};

enum Image_Pattern
{
	ImagePattern_Normal = 1,
	ImagePattern_Flip,
	ImagePattern_Rotate,
	ImagePattern_Polarity
};


struct Patient
{
	std::string								PatientsName;
	std::string								PatientID;
	std::chrono::system_clock::time_point	PatientsBirthDate;
	std::string								PatientsSex;
	std::string                             OtherPatientNames;
	std::string								PatientsWeight;
	std::string								PatientComments;


	Patient()
	{
		PatientsName.clear();
		PatientID.clear();
		PatientsBirthDate = std::chrono::system_clock::now();
		PatientsSex.clear();
		OtherPatientNames.clear();
		PatientsWeight.clear();
		PatientComments.clear();
	}


	void SetData(Patient data)
	{
		PatientsName = data.PatientsName;
		PatientID = data.PatientID;
		PatientsBirthDate = data.PatientsBirthDate;
		PatientsSex = data.PatientsSex;
		OtherPatientNames = data.OtherPatientNames;
		PatientsWeight = data.PatientsWeight;
		PatientComments = data.PatientComments;
	}
};

struct Study
{
	std::string                             StudyInstanceUID;
	std::string								StudyID;
	std::chrono::system_clock::time_point	StudyDateTime;
	std::string								AccessionNumber;
	std::string								StudyDescription;

	Study()
	{
		StudyInstanceUID.clear();
		StudyID.clear();
		StudyDateTime = std::chrono::system_clock::now();
		AccessionNumber.clear();
		StudyDescription.clear();
	}


	void SetData(Study data)
	{
		StudyInstanceUID = data.StudyInstanceUID;
		StudyID = data.StudyID;
		StudyDateTime = data.StudyDateTime;
		AccessionNumber = data.AccessionNumber;
		StudyDescription = data.StudyDescription;
	}
};

struct Series
{
	std::string								SeriesInstanceUID;
	std::string								SeriesNumber;
	std::chrono::system_clock::time_point   SeriesDateTime;
	std::string								PerformingPhysicianName;
	std::string								ProtocolName;
	std::string								BodyPartExamined;

	Series()
	{
		SeriesInstanceUID = "";
		SeriesNumber.clear();
		SeriesDateTime = std::chrono::system_clock::now();
		PerformingPhysicianName.clear();
		ProtocolName.clear();
		BodyPartExamined.clear();
	}

	void SetData(Series data)
	{
		SeriesInstanceUID = data.SeriesInstanceUID;
		SeriesNumber = data.SeriesNumber;
		SeriesDateTime = data.SeriesDateTime;
		PerformingPhysicianName = data.PerformingPhysicianName;
		ProtocolName = data.ProtocolName;
		BodyPartExamined = data.BodyPartExamined;
	}
};

struct Image
{
	unsigned int							SamplesPerPixel;
	std::string								PhotometricInterpretation;
	unsigned int							Rows;
	unsigned int							Columns;
	unsigned int							BitsAllocated;
	unsigned int							BitsStored;
	unsigned int							HighBit;
	unsigned int							PixelRepresentation;
	unsigned short*						    PixelData;
	float                                   RescaleIntercept;
	float                                   RescaleSlope;
	float                                   WindowCenter;
	float                                   WindowWidth;

	Image()
	{
		SamplesPerPixel = 0;
		PhotometricInterpretation.clear();
		Rows = 0;
		Columns = 0;
		BitsAllocated = 0;
		BitsStored = 0;
		HighBit = 0;
		PixelRepresentation = 0;
		PixelData = NULL;
		RescaleIntercept = 0.0;
		RescaleSlope = 0.0;
		WindowCenter = 0.0;
		WindowWidth = 0.0;
	}

	void SetData(Image data)
	{
		SamplesPerPixel = data.SamplesPerPixel;
		PhotometricInterpretation = data.PhotometricInterpretation;
		Rows = data.Rows;
		Columns = data.Columns;
		BitsAllocated = data.BitsAllocated;
		BitsStored = data.BitsStored;
		HighBit = data.HighBit;
		PixelRepresentation = data.PixelRepresentation;
		PixelData = data.PixelData;
		RescaleIntercept = data.RescaleIntercept;
		RescaleSlope = data.RescaleSlope;
		WindowCenter = data.WindowCenter;
		WindowWidth = data.WindowWidth;
	}
};

struct PrivateData
{
	uint16_t GroupTag;
	uint16_t ElementTag;
	std::string Name;
	std::string VR;
	std::string Value;

	PrivateData()
	{
		GroupTag = 0x0000;
		ElementTag = 0x0000;
		Name = "";
		VR = "";
		Value = "";
	}

	void SetData(PrivateData data)
	{
		GroupTag = data.GroupTag;
		ElementTag = data.ElementTag;
		Name = data.Name;
		VR = data.VR;
		Value = data.Value;
	}
};

#endif