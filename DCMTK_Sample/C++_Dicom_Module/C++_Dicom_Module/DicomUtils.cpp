#include "DicomUtils.h"
#include <boost/format.hpp>

#include "dcmtk/dcmdata/dcuid.h"

std::string GenerateUniqueId(UID_Type type)
{
	char uid[100];
	switch (type)
	{
	case UID_Study:
		dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
		break;
	case UID_Series:
		dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
		break;
	case UID_Image:
		dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
		break;
	default:
		std::cout << "The type of generate unique id that is not supported! type :" << type << std::endl;
		break;
	}
	return std::string(uid);
}

std::string GetDataVR(DataVR_Type type)
{
	std::string vr = "";
	switch (type)
	{
	case DataVRType_CS:
		vr = "CS";
		break;
	case DataVRType_SH:
		vr = "SH";
		break;
	case DataVRType_LO:
		vr = "LO";
		break;
	case DataVRType_ST:
		vr = "ST";
		break;
	case DataVRType_LT:
		vr = "LT";
		break;
	case DataVRType_UT:
		vr = "UT";
		break;
	case DataVRType_AE:
		vr = "AE";
		break;
	case DataVRType_PN:
		vr = "PN";
		break;
	case DataVRType_UI:
		vr = "UI";
		break;
	case DataVRType_DA:
		vr = "DA";
		break;
	case DataVRType_TM:
		vr = "TM";
		break;
	case DataVRType_DT:
		vr = "DT";
		break;
	case DataVRType_AS:
		vr = "AS";
		break;
	case DataVRType_IS:
		vr = "IS";
		break;
	case DataVRType_DS:
		vr = "DS";
		break;
	case DataVRType_SS:
		vr = "SS";
		break;
	case DataVRType_US:
		vr = "US";
		break;
	case DataVRType_SL:
		vr = "SL";
		break;
	case DataVRType_UL:
		vr = "UL";
		break;
	case DataVRType_AT:
		vr = "AT";
		break;
	case DataVRType_FL:
		vr = "FL";
		break;
	case DataVRType_FD:
		vr = "FD";
		break;
	case DataVRType_OB:
		vr = "OB";
		break;
	case DataVRType_OW:
		vr = "OW";
		break;
	case DataVRType_OF:
		vr = "OF";
		break;
	case DataVRType_SQ:
		vr = "SQ";
		break;
	case DataVRType_UN:
		vr = "UN";
		break;
	default:
		break;
	}
	return vr;
}


std::chrono::system_clock::time_point StringtoDate(std::string date)
{
	std::string year = date.substr(0, 4);
	std::string month = date.substr(4, 2);
	std::string day = date.substr(6, 2);
	tm tmObj;
	tmObj.tm_year = std::atoi(year.c_str()) - 1900;
	tmObj.tm_mon = std::atoi(month.c_str()) - 1;
	tmObj.tm_mday = std::atoi(day.c_str());
	tmObj.tm_hour = std::atoi("0");
	tmObj.tm_min = std::atoi("0");
	tmObj.tm_sec = std::atoi("0");

	//std::time_t tt = mktime(&tmObj);
	return std::chrono::system_clock::from_time_t(mktime(&tmObj));
}

std::chrono::system_clock::time_point StringtoDateTime(std::string date, std::string time)
{
	std::string year = date.substr(0, 4);
	std::string month = date.substr(4, 2);
	std::string day = date.substr(6, 2);
	std::string hour = time.substr(0, 2);
	std::string min = time.substr(2, 2);
	std::string sec = time.substr(4, 2);

	tm tmObj;
	tmObj.tm_year = std::atoi(year.c_str()) - 1900;
	tmObj.tm_mon = std::atoi(month.c_str()) - 1;
	tmObj.tm_mday = std::atoi(day.c_str());
	tmObj.tm_hour = std::atoi(hour.c_str());
	tmObj.tm_min = std::atoi(min.c_str());
	tmObj.tm_sec = std::atoi(sec.c_str());

	size_t pos = time.find(".", 0);
	if (pos == std::string::npos)
	{
		return std::chrono::system_clock::from_time_t(std::mktime(&tmObj));
	}
	else
	{
		std::string strMS = time.substr(pos + 1, time.length() - pos - 1);
		long long ms = std::atoll(strMS.c_str());
		return (std::chrono::system_clock::from_time_t(std::mktime(&tmObj)) + std::chrono::microseconds(ms));
	}
}


std::string DatetoString(std::chrono::system_clock::time_point timePoint)
{
	std::time_t tt = std::chrono::system_clock::to_time_t(timePoint);
	std::stringstream ss;
	ss << std::put_time(std::localtime(&tt), "%Y%m%d");
	return ss.str();
}

std::string TimetoString(std::chrono::system_clock::time_point timePoint)
{
	std::time_t tt = std::chrono::system_clock::to_time_t(timePoint);

	auto ms = timePoint.time_since_epoch();
	auto diff = std::chrono::duration_cast<std::chrono::microseconds>(ms).count();
	auto const mcsecs = diff % 1000000;

	std::stringstream ss;
	if (mcsecs == 0)
	{
		ss << std::put_time(std::localtime(&tt), "%H%M%S");
	}
	else
	{
		ss << std::put_time(std::localtime(&tt), "%H%M%S") << "." << mcsecs;
	}
	return ss.str();
}

std::string WeighttoString(float weight)
{
	std::string ss = str(boost::format("%f") % weight);
	return ss;
}

std::string AgetoString(int age, AgeUnit_Type type)
{

	std::string ss = str(boost::format("%03d") % age);
	switch (type)
	{
	case AgeUnit_Year:
		ss += "Y";
		break;
	case AgeUnit_Month:
		ss += "M";
		break;
	case AgeUnit_Week:
		ss += "W";
		break;
	case AgeUnit_Day:
		ss += "D";
		break;
	default:
		break;
	}

	return ss;
}

AgeUnit_Type StringtoAgeUnit(std::string type)
{
	if (type.compare("Y") == 0)
	{
		return AgeUnit_Year;
	}
	if (type.compare("M") == 0)
	{
		return AgeUnit_Month;
	}
	if (type.compare("W") == 0)
	{
		return AgeUnit_Week;
	}
	if (type.compare("D") == 0)
	{
		return AgeUnit_Day;
	}

	std::cout << "String to AgeUnit Error! " << "type: " << type << ", So Set Unit is Year" << std::endl;
	return AgeUnit_Year;
}

std::vector<std::string> GetMultiItem(const std::string &str)
{
	std::vector<std::string> itemList;

	if ("" == str)
	{
		return itemList;
	}

	std::string pattern = ";";
	std::string strs = str + pattern;

	size_t size = strs.size();
	size_t pos = strs.find(pattern);
	while (pos != std::string::npos)
	{
		std::string x = strs.substr(0, pos);
		itemList.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return itemList;
}

void MultiItemtoString(std::string &str, const std::string &item)
{
	std::string pattern = ";";
	str.append(item + pattern);
}

double ScientificStringtoFloat(const char *str)
{
	double result = 0.0;
	double base = 10.0;
	double digits = 10.0;
	int index = 0;
	bool indexPositive = true;
	bool subOperator = false;
	while (*str == ' ')
	{
		str++;
	}

	if (*str == '-')
	{
		subOperator = true;
		str++;
	}

	if (!(*str >= '0'&&*str <= '9')) //error
	{
		return result;
	}

	while (*str >= '0'&&*str <= '9'&&*str != '.')
	{
		result = result * base + *str - '0';
		str++;
	}

	if (*str == '.')
	{
		str++;
	}

	while (*str >= '0'&&*str <= '9')
	{
		result = result + (*str - '0') / digits;
		digits *= 10.0;
		str++;
	}

	//Scientific Counting
	if ((*str == 'e') || (*str == 'E'))
	{
		str++;
		if (*str == '-')
		{
			indexPositive = false;
		}
		str++;
		while (*str >= '0'&&*str <= '9')
		{
			index = index * 10 + *str - '0';
			str++;
		}
		while (index > 0)
		{
			if (indexPositive)
			{
				result *= 10;
			}
			else
			{
				result *= 10;
			}
			index--;
		}

		if (indexPositive)
		{
			while (index > 0)
			{
				result /= 10;
				index--;
			}
		}
	}

	return result * (subOperator ? -1.0 : 1.0);
}

std::string GenerateImageName(std::string name, Image_Pattern pattern)
{
	std::string prefix = "";
	switch (pattern)
	{
	case ImagePattern_Normal:
		break;
	case ImagePattern_Flip:
		prefix = "_flip";
		break;
	case ImagePattern_Rotate:
		prefix = "_rotate";
		break;
	case ImagePattern_Polarity:
		prefix = "_polarity";
		break;
	default:
		break;
	}


	auto last_pos = name.find_last_of(".");
	std::string srcName = name.substr(0, last_pos);
	std::cout << srcName << std::endl;
	std::string fileName = srcName + prefix + ".bmp";
	return fileName;
}
