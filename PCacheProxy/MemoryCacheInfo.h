#ifndef H__PCACHEPROXY_MEMORYCACHEINFO__H
#define H__PCACHEPROXY_MEMORYCACHEINFO__H

#include <Poco/RefCountedObject.h>
#include <Poco/AutoPtr.h>

#include <map>

namespace PCacheProxy {

class MemoryCacheInfo : public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<MemoryCacheInfo> Ptr;

	MemoryCacheInfo();

	std::string key;
	std::string uri;
	std::string data;
	std::multimap<std::string, std::string> headers;
};

}

#endif // H__PCACHEPROXY_MEMORYCACHEINFO__H
