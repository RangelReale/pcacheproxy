#ifndef H__PCACHEPROXY_CACHEGET__H
#define H__PCACHEPROXY_CACHEGET__H

#include <Poco/RefCountedObject.h>
#include <Poco/AutoPtr.h>
#include <Poco/Timestamp.h>
#include <Poco/Nullable.h>

#include <iostream>

namespace PCacheProxy {

class CacheGet : public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<CacheGet> Ptr;

	virtual ~CacheGet() {}

	virtual bool mustCheckModified() = 0;
	virtual Poco::Nullable<Poco::Timestamp> lastModified() = 0;
	virtual Poco::Nullable<std::string> ETag() = 0;
	virtual void send() = 0;
};

}

#endif // H__PCACHEPROXY_CACHEGET__H
