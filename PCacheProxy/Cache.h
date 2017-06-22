#ifndef H__PCACHEPROXY_CACHE__H
#define H__PCACHEPROXY_CACHE__H

#include <PCacheProxy/Request.h>
#include <PCacheProxy/Response.h>
#include <PCacheProxy/CacheGet.h>
#include <PCacheProxy/CacheStore.h>

#include <Poco/RefCountedObject.h>
#include <Poco/AutoPtr.h>

namespace PCacheProxy {

class Cache : public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<Cache> Ptr;

	virtual ~Cache() {}

	enum {
		CHK_NOTFOUND,
		CHK_FOUND,
	} check_t;

	virtual CacheGet::Ptr get(Request &request, Response &response) = 0;
	virtual CacheStore::Ptr store(Request &request, Response &response) = 0;
};

}

#endif // H__PCACHEPROXY_CACHE__H