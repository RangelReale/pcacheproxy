#ifndef H__PCACHEPROXY_CACHESTORE__H
#define H__PCACHEPROXY_CACHESTORE__H

#include <Poco/RefCountedObject.h>
#include <Poco/AutoPtr.h>

#include <iostream>

namespace PCacheProxy {

class CacheStore : public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<CacheStore> Ptr;

	virtual ~CacheStore() {}

	virtual std::ostream &stream() = 0;

	virtual void commit() = 0;
	virtual void rollback() = 0;
};

}

#endif // H__PCACHEPROXY_CACHESTORE__H
