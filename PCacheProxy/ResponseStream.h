#ifndef H__PCACHEPROXY_RESPONSESTREAM__H
#define H__PCACHEPROXY_RESPONSESTREAM__H

#include <PCacheProxy/Response.h>

#include <iostream>

namespace PCacheProxy {

class ResponseStream : public Response
{
public:
	ResponseStream(Poco::Net::HTTPResponse &response, std::ostream &stream) :
		Response(), _response(response), _stream(stream)
	{

	}

	Poco::Net::HTTPResponse &response()
	{
		return _response;
	}

	std::ostream &send()
	{
		return _stream;
	}

	bool sent() const
	{
		return false;
	}
private:
	Poco::Net::HTTPResponse &_response;
	std::ostream &_stream;
};

}

#endif // H__PCACHEPROXY_RESPONSESTREAM__H
