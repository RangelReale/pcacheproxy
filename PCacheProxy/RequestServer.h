#ifndef H__PCACHEPROXY_REQUESTSERVER__H
#define H__PCACHEPROXY_REQUESTSERVER__H

#include <PCacheProxy/Request.h>

#include <Poco/Net/HTTPServerRequest.h>

namespace PCacheProxy {

class RequestServer : public Request
{
public:
	RequestServer(Poco::Net::HTTPServerRequest &request) :
		Request(), _request(request)
	{

	}

	Poco::Net::HTTPRequest &request()
	{
		return _request;
	}

	std::istream* stream()
	{
		return &_request.stream();
	}
private:
	Poco::Net::HTTPServerRequest &_request;
};

}

#endif // H__PCACHEPROXY_REQUESTSERVER__H
