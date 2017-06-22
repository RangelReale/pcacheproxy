#ifndef H__PCACHEPROXY_SERVICESERVERPARAMREQUEST__H
#define H__PCACHEPROXY_SERVICESERVERPARAMREQUEST__H

#include <PCacheProxy/ServiceServerRequest.h>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

namespace PCacheProxy {

class ServiceServerParamRequest : public ServiceServerRequest
{
public:
	ServiceServerParamRequest(Cache::Ptr cache, const std::string &urlParamName);

	void request(Request &request, Response &response);
private:
	std::string _urlparamname;
};

}

#endif // H__PCACHEPROXY_SERVICESERVERPARAMREQUEST__H
