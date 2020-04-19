#ifndef _DICOM_PROCESSOR_H
#define _DICOM_PROCESSOR_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "DicomIF.h"

class __declspec(dllexport) DicomProcessor : public IDicom
{
public:
	DicomProcessor();
	~DicomProcessor();

	bool Read(std::string filePath);
	bool Save(std::string filePath);
	void Reset();
	bool ShowImage(std::string filePath, Image_Pattern pattern = ImagePattern_Normal, signed int degree = 0);
	bool SavetoBMP(std::string filePath, Image_Pattern pattern = ImagePattern_Normal, signed int degree = 0);
	

	inline std::shared_ptr<Patient> PatientInfo()
	{
		return m_patient;
	}

	inline std::shared_ptr<Study> StudyInfo()
	{
		return m_study;
	}

	inline std::shared_ptr<Series> SeriesInfo()
	{
		return m_series;
	}

	inline std::shared_ptr<Image> ImageInfo()
	{
		return m_image;
	}

	inline std::list<PrivateData> &PrivateDataInfo()
	{
		return m_privateData;
	}

	void MetaInfoTags();
	void DataSetTags();

private:
	void GetPatient();
	void GetStudy();
	void GetSeries();
	void GetImage();
	void GetPrivateData();

	void FillPatient();
	void FillStudy();
	void FillSeries();
	void FillNormalizeImage(unsigned char* data);
	void FillImage();
	void FillPrivateData();
	


	void GetInfo(DcmTagKey tagKey, std::string &info);
	void GetInfo(DcmTagKey tagKey, unsigned int &info);
	void GetInfo(DcmTagKey tagKey, float &info);
	void GetDateTimeInfo(DcmTagKey tagKey, DcmTagKey tagTimeKey, std::chrono::system_clock::time_point &info);
	void FillInfo(DcmTagKey tagKey, std::string &info);

	void RegisterPrivateTags(PrivateData data);
	void FreePixelData();
	void PatternConversion(DicomImage* &image, Image_Pattern pattern, signed int degree = 0);

	std::string					m_fileName;
	DcmFileFormat				m_fileformat;
	std::shared_ptr<Patient>	m_patient;
	std::shared_ptr<Study>		m_study;
	std::shared_ptr<Series>     m_series;
	std::shared_ptr<Image>		m_image;
	std::list<PrivateData>      m_privateData;
};

#endif


