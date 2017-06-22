#ifndef H__PCACHEPROXY_SERVICE__H
#define H__PCACHEPROXY_SERVICE__H

#include <PCacheProxy/Request.h>
#include <PCacheProxy/Response.h>
#include <PCacheProxy/Cache.h>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

namespace PCacheProxy {

class Service
{
public:
	Service(Cache::Ptr cache);
	virtual ~Service() {}

	virtual void request(Request &request, Response &response);
private:
	Cache::Ptr _cache;
};

}

#endif // H__PCACHEPROXY_SERVICE__H
