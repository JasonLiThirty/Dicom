#include "DicomProcessor.h"
#include "DicomUtils.h"
#include <opencv.hpp>


DicomProcessor::DicomProcessor()
{
	Reset();
}


DicomProcessor::~DicomProcessor()
{
	FreePixelData();
}

void DicomProcessor::Reset()
{
	m_patient.reset(new Patient());
	m_study.reset(new Study());
	m_series.reset(new Series());
	m_image.reset(new Image());
	m_privateData.clear();
	FreePixelData();
}

bool DicomProcessor::ShowImage(std::string filePath, Image_Pattern pattern, signed int degree)
{
	if (!Read(filePath))
	{
		std::cout << "Read Dicom File Failed: " << filePath.c_str() << std::endl;
		return false;
	}


	E_TransferSyntax tfs = m_fileformat.getDataset()->getOriginalXfer();
	DicomImage *pImage = new DicomImage(&m_fileformat, tfs);
	pImage->setWindow(m_image->WindowCenter, m_image->WindowWidth);


	PatternConversion(pImage, pattern, degree);


	Uint16 *pixelData = (Uint16*)(pImage->getOutputData(m_image->BitsAllocated));
	if (pixelData == NULL)
	{
		std::cout << "Get Image Data Failed: " << filePath.c_str() << std::endl;
		return false;
	}


	cv::Mat dst(pImage->getWidth(), pImage->getHeight(), CV_16U, pixelData);
	cv::imshow(m_patient->PatientsName, dst);
	cv::waitKey(0);


	return true;
}


bool DicomProcessor::SavetoBMP(std::string filePath, Image_Pattern pattern, signed int degree)
{
	if (!Read(filePath))
	{
		std::cout << "Read Dicom File Failed: " << filePath.c_str() << std::endl;
		return false;
	}


	E_TransferSyntax tfs = m_fileformat.getDataset()->getOriginalXfer();
	DicomImage *pImage = new DicomImage(&m_fileformat, tfs);
	pImage->setWindow(m_image->WindowCenter, m_image->WindowWidth);


	PatternConversion(pImage, pattern, degree);


	if (!pImage->writeBMP(GenerateImageName(filePath, pattern).c_str(), 8))
	{
		std::cout << "Read Dicom File Failed: " << filePath.c_str() << std::endl;
		return false;
	}
	return true;
}

void DicomProcessor::PatternConversion(DicomImage* &image, Image_Pattern pattern, signed int degree)
{
	switch (pattern)
	{
	case ImagePattern_Normal:
		break;
	case ImagePattern_Flip:
		image->flipImage();
		break;
	case ImagePattern_Rotate:
		image->rotateImage(degree);
		break;
	case ImagePattern_Polarity:
		image->setPolarity(EPP_Reverse);
		break;
	default:
		break;
	}
}


bool DicomProcessor::Read(std::string filePath)
{
	OFCondition status = m_fileformat.loadFile(filePath.c_str());
	if (!status.good())
	{
		std::cout << "Read Dicom file error£º" << status.text() << ",file: " << filePath.c_str() << std::endl;
		return false;
	}
	try
	{
		GetPatient();
		GetStudy();
		GetSeries();
		GetImage();
		GetPrivateData();
	}
	catch (...)
	{
		std::cout << "Get Dicom info error!" << std::endl;
		FreePixelData();
		return false;
	}
	std::cout << "Read Dicom file succeed£¡" << std::endl;
	return true;
}

bool DicomProcessor::Save(std::string filePath)
{
	try
	{
		FillPatient();
		FillStudy();
		FillSeries();
		FillImage();
		FillPrivateData();
	}
	catch (const std::exception&)
	{
		std::cout << "Fill Dicom info error!" << std::endl;
		return false;
	}

	OFCondition status = m_fileformat.saveFile(filePath.c_str());
	if (!status.good())
	{
		std::cout << "Save Dicom file error£º"<< status.text() << ",file: " << filePath << std::endl;
		return false;
	}

	std::cout << "Save Dicom file succeed£¡" << std::endl;
	return true;
}

void DicomProcessor::MetaInfoTags()
{
	std::cout << "Meta Tag-----------------------------Meta Tag" << std::endl;
	DcmObject* item = m_fileformat.getMetaInfo()->nextInContainer(NULL);
	while (item)
	{
		DcmVR valueVR(item->getVR());
		DcmTag tag(item->getTag());

		std::cout << item->getTag().toString().c_str() << "    " << valueVR.getVRName() << "    " << tag.getTagName() << std::endl;
		item = m_fileformat.getMetaInfo()->nextInContainer(item);
	}
}

void DicomProcessor::DataSetTags()
{
	std::cout << "DataSet Tag------------------------------DataSet Tag" << std::endl;
	DcmObject* item = m_fileformat.getDataset()->nextInContainer(NULL);
	while (item)
	{
		DcmVR valueVR(item->getVR());
		DcmTag tag(item->getTag());
		std::cout << item->getTag().toString().c_str() << "    " << valueVR.getVRName() << "    " << tag.getTagName() << std::endl;
		item = m_fileformat.getDataset()->nextInContainer(item);
	}
}


void DicomProcessor::GetPatient()
{
	GetInfo(DCM_PatientID, m_patient->PatientID);
	GetInfo(DCM_PatientName, m_patient->PatientsName);
}

void DicomProcessor::GetStudy()
{
	GetInfo(DCM_StudyInstanceUID, m_study->StudyInstanceUID);
	if (m_study->StudyInstanceUID.empty())
	{
		std::cout << "Get Tag DCM_StudyInstanceUID Error£¡" << std::endl;
	}
}

void DicomProcessor::GetSeries()
{
	GetDateTimeInfo(DCM_SeriesDate, DCM_SeriesTime, m_series->SeriesDateTime);
	GetInfo(DCM_BodyPartExamined, m_series->BodyPartExamined);
}

void DicomProcessor::GetImage()
{
	GetInfo(DCM_SamplesPerPixel, m_image->SamplesPerPixel);
	GetInfo(DCM_PhotometricInterpretation, m_image->PhotometricInterpretation);
	GetInfo(DCM_Rows, m_image->Rows);
	GetInfo(DCM_Columns, m_image->Columns);
	GetInfo(DCM_BitsAllocated, m_image->BitsAllocated);
	GetInfo(DCM_BitsStored, m_image->BitsStored);
	GetInfo(DCM_HighBit, m_image->HighBit);
	GetInfo(DCM_PixelRepresentation, m_image->PixelRepresentation);
	GetInfo(DCM_WindowCenter, m_image->WindowCenter);
	GetInfo(DCM_WindowWidth, m_image->WindowWidth);

	uint pixelByteCount = (m_image->BitsAllocated <= 8) ? 1 : 2;
	ulong dataLength = m_image->Rows * m_image->Columns * pixelByteCount * m_image->SamplesPerPixel;

	DcmElement* element = NULL;
	OFCondition status = m_fileformat.getDataset()->findAndGetElement(DCM_PixelData, element);
	if (!status.good())
	{
		std::cout << "Get pixel data element error:" << status.text() << std::endl;
	}
	else
	{
		//std::cout << "Pixel data element's length:" << element->getLength() << std::endl;

		unsigned short*	pData = NULL;
		status = element->getUint16Array(pData);
		if (!status.good())
		{
			std::cout << "Get pixel data array error:" << status.text() << std::endl;
			return;
		}
		FreePixelData();
		m_image->PixelData = (uint16_t *)malloc(dataLength);
		memcpy(m_image->PixelData, pData, dataLength);
	}
}

void DicomProcessor::GetPrivateData()
{
	DcmTagKey tagKey;
	OFString value;
	DcmElement* element = NULL;

	for (auto &data : m_privateData)
	{
		tagKey.set((Uint16)data.GroupTag, (Uint16)data.ElementTag);
		value.clear();
		element = NULL;
		OFCondition status = m_fileformat.getDataset()->findAndGetElement(tagKey, element);

		if (!status.good())
		{
			std::ios::fmtflags fmt(std::cout.flags());
			std::cout << "Get private Tag(" << std::hex << data.GroupTag << "," << data.ElementTag << ") error:" << status.text() << std::endl;
			std::cout.flags(fmt);
		}
		else
		{
			status = element->getOFString(value, 0);
			if (!status.good())
			{
				std::ios::fmtflags fmt(std::cout.flags());
				std::cout << "Get private Tag(" << std::hex << data.GroupTag << "," << data.ElementTag << ") error:" << status.text() << std::endl;
				std::cout.flags(fmt);
			}
			else
			{
				data.Value = std::string(value.c_str());
				DcmVR valueVR(element->getVR());
				data.VR = std::string(valueVR.getVRName());
			}
		}
	}
}


void DicomProcessor::FillPatient()
{
	FillInfo(DCM_PatientID, m_patient->PatientID);
	FillInfo(DCM_PatientName, m_patient->PatientsName);
}

void DicomProcessor::FillStudy()
{
	if (m_study->StudyInstanceUID.empty())
	{
		std::cout << "Study instanceUID is empty!" << std::endl;
	}

	FillInfo(DCM_StudyInstanceUID, m_study->StudyInstanceUID);
}

void DicomProcessor::FillSeries()
{
	FillInfo(DCM_SeriesDate, DatetoString(m_series->SeriesDateTime));
	FillInfo(DCM_SeriesTime, TimetoString(m_series->SeriesDateTime));
}


void DicomProcessor::FillNormalizeImage(unsigned char* data)
{
	//m_image->SamplesPerPixel = 1;
	//m_image->BitsAllocated = 16;
	//m_image->BitsStored = 12;
	//m_image->HighBit = 11;


	FillInfo(DCM_SamplesPerPixel, std::to_string(m_image->SamplesPerPixel));
	FillInfo(DCM_PhotometricInterpretation, m_image->PhotometricInterpretation);
	FillInfo(DCM_Rows, std::to_string(m_image->Rows));
	FillInfo(DCM_Columns, std::to_string(m_image->Columns));
	FillInfo(DCM_BitsAllocated, std::to_string(m_image->BitsAllocated));
	FillInfo(DCM_BitsStored, std::to_string(m_image->BitsStored));
	FillInfo(DCM_HighBit, std::to_string(m_image->HighBit));
	FillInfo(DCM_PixelRepresentation, std::to_string(m_image->PixelRepresentation));


	//number of image pixels
	int64_t dataNumber = m_image->Rows * m_image->Columns;


	std::shared_ptr<float> pData(new float[dataNumber]);
	float minData = 0.0;
	float maxData = 0.0;


	//Get max * min data(float32)
	for (int64_t i = 0, j = 0; i < dataNumber; i++, j += 4)
	{
		memcpy(pData.get() + i, data + j, 4);
		minData = (*(pData.get() + i) < minData) ? *(pData.get() + i) : minData;
		maxData = (*(pData.get() + i) > maxData) ? *(pData.get() + i) : maxData;
	}


	//Get the range of normalization
	uint32_t range = (2 << (m_image->BitsStored - 1)) - 1;	//BitsStored = 12 -> range = 4095


															//Get Rescale Slope and Rescale Intercept
	m_image->RescaleIntercept = minData;
	m_image->RescaleSlope = (maxData - minData) / range;

	//Get Window Center and Window Width
	m_image->WindowCenter = maxData / 2;
	m_image->WindowWidth = maxData;

	//Normalization(Float32 to Uint16), BitsAllocated = 16 -> Create unsigned int16 array
	std::shared_ptr<uint16_t> pNormalizedData(new uint16_t[dataNumber]());
	for (int64_t i = 0; i < dataNumber; i++)
	{
		//normalizedData = (srcData - rescale intercept) / rescale slope
		pNormalizedData.get()[i] = (uint16_t)((pData.get()[i] - m_image->RescaleIntercept) / m_image->RescaleSlope);
	}


	//Save normalized data
	OFCondition status = m_fileformat.getDataset()->putAndInsertUint16Array(DCM_PixelData, pNormalizedData.get(), dataNumber);
	if (!status.good())
	{
		std::cout << "Fill pixel data error:" << status.text() << std::endl;
	}


	//Save Fields invloved
	FillInfo(DCM_RescaleIntercept, std::to_string(m_image->RescaleIntercept));
	FillInfo(DCM_RescaleSlope, std::to_string(m_image->RescaleSlope));
	FillInfo(DCM_WindowCenter, std::to_string(m_image->WindowCenter));
	FillInfo(DCM_WindowWidth, std::to_string(m_image->WindowWidth));


	//uint pixelByteCount = (m_image->BitsAllocated <= 8) ? 1 : 2;
	//ulong dataLength = m_image->Rows * m_image->Columns * pixelByteCount * m_image->SamplesPerPixel;
}

void DicomProcessor::FillImage()
{
	FillInfo(DCM_SamplesPerPixel, std::to_string(m_image->SamplesPerPixel));
	if (m_image->PhotometricInterpretation.empty())
	{
		std::cout << "No PhotometricInterpretation Info in Fill image Pixel" << std::endl;
	}

	FillInfo(DCM_PhotometricInterpretation, m_image->PhotometricInterpretation);
	FillInfo(DCM_Rows, std::to_string(m_image->Rows));
	FillInfo(DCM_Columns, std::to_string(m_image->Columns));
	FillInfo(DCM_BitsAllocated, std::to_string(m_image->BitsAllocated));
	FillInfo(DCM_BitsStored, std::to_string(m_image->BitsStored));
	FillInfo(DCM_HighBit, std::to_string(m_image->HighBit));
	FillInfo(DCM_PixelRepresentation, std::to_string(m_image->PixelRepresentation));

	uint pixelByteCount = (m_image->BitsAllocated <= 8) ? 1 : 2;
	ulong dataLength = m_image->Rows * m_image->Columns * pixelByteCount * m_image->SamplesPerPixel;

	if (m_image->PixelData != NULL)
	{
		OFCondition status = m_fileformat.getDataset()->putAndInsertUint16Array(DCM_PixelData, m_image->PixelData, dataLength / pixelByteCount);
		if (!status.good())
		{
			std::cout << "Fill pixel data error:" << status.text() << std::endl;
		}
	}
}

void DicomProcessor::FillPrivateData()
{
	for (auto &data : m_privateData)
	{
		if (!data.VR.empty())
		{
			RegisterPrivateTags(data);
			OFCondition status = m_fileformat.getDataset()->putAndInsertString(DcmTag((Uint16)data.GroupTag, (Uint16)data.ElementTag, PRIVATE_GROUP_NAME), data.Value.c_str());
			if (!status.good())
			{
				std::cout << "Fill private Tag(" << data.GroupTag << "," << data.ElementTag << ") error:" << status.text();
			}
		}
	}
	m_privateData.clear();
}

void DicomProcessor::RegisterPrivateTags(PrivateData data)
{
	DcmDataDictionary &dict = dcmDataDict.wrlock();

	if ((data.VR.compare("OB") == 0) || (data.VR.compare("FL") == 0) || (data.VR.compare("FD") == 0))
	{
		dict.addEntry(new DcmDictEntry((Uint16)data.GroupTag, (Uint16)data.ElementTag, EVR_UT, data.Name.c_str(), 1, 1, "private", OFTrue, PRIVATE_GROUP_NAME));
	}
	else
	{
		dict.addEntry(new DcmDictEntry((Uint16)data.GroupTag, (Uint16)data.ElementTag, DcmVR(data.VR.c_str()), data.Name.c_str(), 1, 1, "private", OFTrue, PRIVATE_GROUP_NAME));
	}
	dcmDataDict.wrunlock();
}


void DicomProcessor::GetInfo(DcmTagKey tagKey, std::string &info)
{
	OFString ofData;
	OFCondition status = m_fileformat.getDataset()->findAndGetOFString(tagKey, ofData);
	if (!status.good())
	{
		std::cout << "Get Tag(" << tagKey.toString().c_str() << ") error: " << status.text() << std::endl;
	}
	info = ofData.c_str();
}

void DicomProcessor::GetInfo(DcmTagKey tagKey, unsigned int &info)
{
	std::string strInfo;
	GetInfo(tagKey, strInfo);
	try
	{
		info = std::atoi(strInfo.c_str());
	}
	catch (...)
	{
		std::cout << "Get Tag(" << tagKey.toString().c_str() << ") error(String to UInt)£¡" << std::endl;
	}
}

void DicomProcessor::GetInfo(DcmTagKey tagKey, float &info)
{
	std::string strInfo;
	GetInfo(tagKey, strInfo);
	try
	{
		info = std::atof(strInfo.c_str());
	}
	catch (...)
	{
		std::cout << "Get Tag(" << tagKey.toString().c_str() << ") error(String to UInt)£¡" << std::endl;
	}
}

void DicomProcessor::GetDateTimeInfo(DcmTagKey tagDateKey, DcmTagKey tagTimeKey, std::chrono::system_clock::time_point &info)
{
	std::string date;
	std::string time;
	GetInfo(tagDateKey, date);
	GetInfo(tagTimeKey, time);
	info = StringtoDateTime(date, time);
}

void DicomProcessor::FillInfo(DcmTagKey tagKey, std::string &info)
{
	OFCondition status = m_fileformat.getDataset()->putAndInsertString(tagKey, info.c_str());
	if (!status.good())
	{
		std::cout << "Fill Tag(" << tagKey.toString().c_str() << ") error: " << status.text() << std::endl;
	}
}

void DicomProcessor::FreePixelData()
{
	if (m_image->PixelData != NULL)
	{
		free(m_image->PixelData);
		m_image->PixelData = NULL;
	}
}
