#ifndef H__PCACHEPROXY_CACHEVALIDATOR__H
#define H__PCACHEPROXY_CACHEVALIDATOR__H

#include <PCacheProxy/Request.h>
#include <PCacheProxy/Response.h>

namespace PCacheProxy {

class CacheValidator
{
public:
	virtual ~CacheValidator() {}

	virtual bool isCacheable(Request &request) = 0;
	virtual bool isStorable(Request &request, Response &response) = 0;
};

}

#endif // H__PCACHEPROXY_CACHEVALIDATOR__H
