#ifndef H__PCACHEPROXY_SERVICESERVERREQUEST__H
#define H__PCACHEPROXY_SERVICESERVERREQUEST__H

#include <PCacheProxy/Service.h>

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace PCacheProxy {

class ServiceServerRequest : public Service
{
public:
	ServiceServerRequest(Cache::Ptr cache);

	virtual void serverRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse& response);
	void request(Request &request, Response &response);
};

}

#endif // H__PCACHEPROXY_SERVICESERVERREQUEST__H
