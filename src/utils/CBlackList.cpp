#include "utils/CBlackList.h"
#include "utils/dipcvnetutils.h"
#include "main/CDIPCVNetConfig.h"
CBlackList::CBlackList()
{

}
CBlackList::~CBlackList()
{

}
CAWResult CBlackList::Init()
{
	CAWString projectspath = GetDIPCVNETHome();
	CAWString configvalue = CDipcVnetConfig::Instance()->GetBlackJsonFile();
#ifndef CAW_WIN32
	configvalue = projectspath + "/conf/" + configvalue;
#else 
	configvalue = projectspath + "\\conf\\" + configvalue;
#endif
	char* m_pcontent = NULL;
	size_t m_contentsize = 0;
	//printf("CDIPCClusterManager::LoadClusterNode,configvalue=%s\n",configvalue.c_str());

	if (WFFileGetFileContent(configvalue.c_str(), &m_pcontent, m_contentsize) == -1)
	{
		CAW_FATAL_TRACE("CBlackList::Init Start Process ,can not find the file,the filename is " << configvalue);
		return CAW_ERROR_FAILURE;
	}
	std::string struser = std::string(m_pcontent, m_contentsize);
	WFFileFreeFileContent(m_pcontent);

	Json::Value value;
	Json::Reader reader;
	if (reader.parse(struser.c_str(), value) == false)
	{
		CAW_FATAL_TRACE("CBlackList::Init Start Process ,can not parser apps.json ,the filename is " << configvalue);
		return CAW_ERROR_FAILURE;
	}
	if (value["blacklist"].isNull())
	{
		CAW_FATAL_TRACE("CBlackList::Init error, can not find json apps value");
		return CAW_ERROR_FAILURE;
	}
	if (!value["blacklist"].isArray())
	{
		CAW_FATAL_TRACE("CBlackList::Init error, json apps is not array");
		return CAW_ERROR_FAILURE;
	}
	Json::Value appservervalue = value["blacklist"];

	for (unsigned int i = 0; i < appservervalue.size(); i++)
	{
		int jno=0;
		Json::Value oneapps = appservervalue[i];
		if (!oneapps["jno"].isNull())
		{
			if (!oneapps["jno"].isInt())
			{
				return CAW_ERROR_FAILURE;
			}
			jno = oneapps["jno"].asInt();
		}
		if (jno != 0)
		{
			AddList((uint16_t)jno);
		}
	}
	return CAW_OK;
}


CAWResult CBlackList::Veryfy(uint16_t unit)
{
	if (m_black_list.empty())
	{
		return VNET_IGNORE;
	}
	CBLACKLISTType::iterator it = m_black_list.begin();
	while (it != m_black_list.end())
	{
		if (*it == unit)
		{
			return VNET_BL_FIND_OK;
		}
		else 
		{
			it++;
		}
	}
	return VNET_BL_FIND_FAILURE;
}

CAWResult CBlackList::AddList(uint16_t unit)
{
	CBLACKLISTType::iterator it = m_black_list.begin();
	while (it != m_black_list.end())
	{
		if (*it == unit)
		{
			return CAW_ERROR_FAILURE;
		}
		else
		{
			it++;
		}
	}
	m_black_list.push_back(unit);
	return CAW_OK;
}

CAWResult CBlackList::RemoveList(uint16_t unit)
{
	if (m_black_list.empty())
	{
		return CAW_ERROR_FAILURE;
	}

	CBLACKLISTType::iterator it = m_black_list.begin();
	while (it != m_black_list.end())
	{
		if (*it == unit)
		{
			it = m_black_list.erase(it);
			return CAW_OK;
		}
		else
		{
			it++;
		}
	}
	return CAW_ERROR_FAILURE;
}

