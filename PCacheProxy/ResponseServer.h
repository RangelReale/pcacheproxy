#ifndef H__PCACHEPROXY_RESPONSESERVER__H
#define H__PCACHEPROXY_RESPONSESERVER__H

#include <PCacheProxy/Response.h>

#include <Poco/Net/HTTPServerResponse.h>

namespace PCacheProxy {

class ResponseServer : public Response
{
public:
	ResponseServer(Poco::Net::HTTPServerResponse &response) :
		Response(), _response(response)
	{

	}

	Poco::Net::HTTPResponse &response()
	{
		return _response;
	}

	std::ostream &send()
	{
		return _response.send();
	}

	bool sent() const
	{
		return _response.sent();
	}
private:
	Poco::Net::HTTPServerResponse &_response;
};

}

#endif // H__PCACHEPROXY_RESPONSESERVER__H
