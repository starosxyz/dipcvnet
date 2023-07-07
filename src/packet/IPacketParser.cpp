/***********************************************************************
*    版权所有 (C) 2018-2021 南京北星极网络科技有限公司
**********************************************************************/
#include <packet/IPacketParser.h>
#include <inttypes.h>
#include <stdio.h>
#include <starbase/CAWMacAddress.h>
#include <sstream>
#ifdef CAW_WIN32
#include <webrtc/base/win32.h>
using namespace rtc;
#endif


IPacketParser::IPacketParser()
{
}

Json::Value IPacketParser::ValueToJson(uint8_t value)
{
    char buffer[1024]={0};
    snprintf(buffer,1024,"%d", value);

    return Json::Value(buffer);
}

Json::Value IPacketParser::ValueToJson(uint16_t value)
{
    char buffer[1024]={0};
    snprintf(buffer,1024,"%d", value);

    return Json::Value(buffer);

}

Json::Value IPacketParser::ValueToJson(uint32_t value)
{
    CAWString str = U32ToStr(value);
    return Json::Value(str.c_str());
}

Json::Value IPacketParser::ValueToJson(uint64_t value)
{
    CAWString str = U64ToStr(value);
    return Json::Value(str.c_str());
}

Json::Value IPacketParser::ValueToJson(const CAWString &value)
{
    return Json::Value(value.c_str());
}
/**********************************************************************
* 函数名称：ValueToJson
* 功能描述：将数据值转换成json形式
* 访问的表：无
* 修改的表：无
* 输入参数：bool boolvalue:布尔值
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/

Json::Value IPacketParser::ValueToJson(bool boolvalue)
{
    if (boolvalue)
    {
       return Json::Value("true");
    }
    else 
    {
        return Json::Value("false");
    }
}
/**********************************************************************
* 函数名称：IPv4ToJson
* 功能描述：将ipv4数据转换成json形式
* 访问的表：无
* 修改的表：无
* 输入参数：uint32_t ipv4addr：ipv4地址
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/

Json::Value IPacketParser::IPv4ToJson(uint32_t ipv4addr)
{
    CAWInetAddr addr;
    addr.SetIpAddrBy4Bytes(ipv4addr, FALSE);
    CAWString ipaddrstring = addr.GetIpDisplayName();
    return Json::Value(ipaddrstring.c_str());
}
/**********************************************************************
* 函数名称：IPv6ToJson
* 功能描述：将ipv6数据转换成json形式
* 访问的表：无
* 修改的表：无
* 输入参数：uint8_t ipv6[128]：ipv6地址
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/

Json::Value IPacketParser::IPv6ToJson(uint8_t ipv6[128])
{
    char ipv6buffer1[1024]={0};
#ifndef CAW_WIN32
    ::inet_ntop(AF_INET6,ipv6,ipv6buffer1,sizeof(ipv6buffer1));
#else
	win32_inet_ntop(AF_INET6, ipv6, ipv6buffer1, sizeof(ipv6buffer1));
#endif
    return Json::Value(ipv6buffer1);
}
/**********************************************************************
* 函数名称：MacAddressToJson
* 功能描述：将物理地址数据转换成json形式
* 访问的表：无
* 修改的表：无
* 输入参数：uint8_t mac[6]：物理地址
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/

Json::Value IPacketParser::MacAddressToJson(uint8_t mac[6])
{
    CAWMacAddress addr;
    addr.SetMacAddr((char *)mac);

    return Json::Value(addr.ToString());
}
/**********************************************************************
* 函数名称：BufferToJson
* 功能描述：将字符缓冲区数据转换成json形式
* 访问的表：无
* 修改的表：无
* 输入参数：uint8_t *buffer:字符区指针size_t buffersize:缓冲区大小
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/

Json::Value IPacketParser::BufferToJson(uint8_t *buffer, size_t buffersize)
{
    std::string strbuffer((char *)buffer, buffersize);
    return Json::Value(strbuffer.c_str());
}
/**********************************************************************
* 函数名称：ByteArrayToJson
* 功能描述：将字节数组转换成json形式
* 访问的表：无
* 修改的表：无
* 输入参数：char *byte:字节指针 size_t bytesize:字节大小
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/

Json::Value IPacketParser::ByteArrayToJson(char *byte, size_t bytesize)
{
    if ((byte == NULL) || (bytesize == 0))
    {
        Json::Value nullValue;
        return nullValue;
    }
    std::stringstream stream;  
    for(uint32_t i=0;i<bytesize;i++)
    {
        stream<<byte[i];
    }
    return Json::Value(stream.str().c_str());
}
/**********************************************************************
* 函数名称：ValueFromJson
* 功能描述：将json形式转换为基本数据值
* 访问的表：无
* 修改的表：无
* 输入参数：Json::Value &jsonvalue：数据地址 const CAWString &name:键值 uint8_t &outvalue：数据值
* 输出参数：无
* 返 回 	  : 无
* 其它说明：
* 修改日期      版本号  修改人      修改内容
* ---------------------------------------------------------------------
* 2019/4/17  V1.0    夏章辉     创建
************************************************************************/

void IPacketParser::ValueFromJson(Json::Value &jsonvalue, const CAWString &name, uint8_t &outvalue)
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            uint32_t retvalue= 0;
            CAWString str= jsonvalue[name.c_str()].asString().c_str();
            StrToU32(str, retvalue);
            outvalue=(uint8_t)retvalue;

        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }

}

void IPacketParser::ValueFromJson(Json::Value &jsonvalue, const CAWString &name, uint16_t  &outvalue)
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            uint32_t retvalue= 0;
            CAWString str= jsonvalue[name.c_str()].asString().c_str();
            StrToU32(str, retvalue);
            outvalue=(uint16_t)retvalue;

        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }

}

void IPacketParser::ValueFromJson(Json::Value &jsonvalue, const CAWString &name, uint32_t &outvalue)
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            uint32_t retvalue= 0;
            CAWString str= jsonvalue[name.c_str()].asString().c_str();
            StrToU32(str, retvalue);
            outvalue = retvalue;
        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }

}

void IPacketParser::ValueFromJson(Json::Value &jsonvalue, const CAWString &name, uint64_t &outvalue)
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            uint64_t retvalue= 0;
            CAWString str= jsonvalue[name.c_str()].asString().c_str();
            StrToU64(str, retvalue);
            outvalue = retvalue;
        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }

}

void IPacketParser::ValueFromJson(Json::Value &jsonvalue, const CAWString &name, CAWString &output)
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            output=jsonvalue[name.c_str()].asString().c_str();
        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }
}


void IPacketParser::ValueFromJson(Json::Value &jsonvalue, const CAWString &name, bool &output)
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            std::string booloutput=jsonvalue[name.c_str()].asString().c_str();
            if ((booloutput == "true") || (booloutput == "TRUE") || (booloutput == "1"))
            {
                output=true;
            }
            else if ((booloutput == "false") || (booloutput == "FALSE") || (booloutput == "0"))
            {
                output=false;
            }
            else 
            {
                output=false;
            }
        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }
}

void IPacketParser::IPv4FromJson(Json::Value &jsonvalue, const CAWString &name, uint32_t &ipv4output)
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            std::string booloutput=jsonvalue[name.c_str()].asString().c_str();
            CAWInetAddr addr(booloutput.c_str(), 0);
            ipv4output = ntohl(addr.GetIpAddrIn4Bytes());
        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }
}

void IPacketParser::IPv6FromJson(Json::Value &jsonvalue, const CAWString &name, uint8_t ipv6output[128])
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            std::string booloutput=jsonvalue[name.c_str()].asString().c_str();
#ifndef CAW_WIN32
            inet_pton(AF_INET6, booloutput.c_str(), ipv6output);
#else
			win32_inet_pton(AF_INET6, booloutput.c_str(), ipv6output);
#endif
        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }
}

void IPacketParser::MacAddressFromJson(Json::Value &jsonvalue, const CAWString &name, uint8_t macaddr[6])
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            std::string booloutput=jsonvalue[name.c_str()].asString().c_str();
            CAWMacAddress addr;
            addr.ParseAddress(booloutput.c_str());
            addr.ToOctet((char *)macaddr);
        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }
}

void IPacketParser::BufferFromJson(Json::Value &jsonvalue, const CAWString &name, uint8_t *buffer, size_t buffersize)
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            std::string booloutput=jsonvalue[name.c_str()].asString().c_str();
            BufferStringCopy((char *)buffer, buffersize, booloutput.c_str());
        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }
}

void IPacketParser::ByteArrayFromJson(Json::Value &jsonvalue, const CAWString &name, char *buffer, size_t buffersize)
{
    if ((buffer == NULL) || (buffersize == 0))
    {
        return;
    }

    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            std::string booloutput=jsonvalue[name.c_str()].asString().c_str();
            std::stringstream ss;
            ss.str(booloutput);
            size_t index=0;
            while (1)
            {
                ss >> buffer[index];
                if ( ss.fail() ) break;
                index++;
                if (index>=buffersize)
                {
                    break;
                }
            }
            //BufferStringCopy((char *)buffer, buffersize, booloutput.c_str());
        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    }
}


void IPacketParser::ValueFromJson_S(Json::Value &jsonvalue, const CAWString &name, uint16_t &outvalue)
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            uint32_t retvalue= 0;
            std::string str= jsonvalue[name.c_str()].asString().c_str();
            StrToU32(CAWString(str.c_str()), retvalue);
            outvalue=(uint16_t)retvalue;

        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    } 
}

void IPacketParser::ValueFromJson_S(Json::Value &jsonvalue, const CAWString &name, uint8_t &outvalue)
{
    if (!jsonvalue[name.c_str()].isNull())
    {
        if (jsonvalue[name.c_str()].type() == Json::stringValue)
        {
            uint32_t retvalue= 0;
            std::string str= jsonvalue[name.c_str()].asString().c_str();
            StrToU32(CAWString(str.c_str()), retvalue);
            outvalue=(uint8_t)retvalue;

        }
        else 
        {
            return;
        }
    }
    else 
    {
        return;
    } 
}


