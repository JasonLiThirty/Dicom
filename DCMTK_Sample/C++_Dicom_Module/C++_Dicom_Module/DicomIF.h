#pragma warning( disable: 4251 )

#ifndef _DICOM_IF_H
#define _DICOM_IF_H

#include "DicomInfo.h"
#include <memory>
#include <list>

class __declspec(dllexport) IDicom
{
public:
	virtual ~IDicom() {}
	virtual bool Read(std::string filePath) = 0;
	virtual bool Save(std::string filePath) = 0;
	virtual void Reset() = 0;
	virtual bool ShowImage(std::string filePath, Image_Pattern pattern = ImagePattern_Normal, signed int degree = 0) = 0;
	virtual bool SavetoBMP(std::string filePath, Image_Pattern pattern = ImagePattern_Normal, signed int degree = 0) = 0;

	virtual void MetaInfoTags() = 0;
	virtual void DataSetTags() = 0;

	virtual std::shared_ptr<Patient>	PatientInfo() = 0;
	virtual std::shared_ptr<Study>		StudyInfo() = 0;
	virtual std::shared_ptr<Series>		SeriesInfo() = 0;
	virtual std::shared_ptr<Image>		ImageInfo() = 0;
	virtual std::list<PrivateData>&		PrivateDataInfo() = 0;

};

#endif