#ifndef H__PCACHEPROXY_REQUESTSTREAM__H
#define H__PCACHEPROXY_REQUESTSTREAM__H

#include <PCacheProxy/Request.h>


namespace PCacheProxy {

class RequestStream : public Request
{
public:
	RequestStream(Poco::Net::HTTPRequest &request) :
		Request(), _request(request), _stream(NULL)
	{

	}

	RequestStream(Poco::Net::HTTPRequest &request, std::istream &stream) :
		Request(), _request(request), _stream(&stream)
	{

	}

	Poco::Net::HTTPRequest &request()
	{
		return _request;
	}

	std::istream* stream()
	{
		return _stream;
	}
private:
	Poco::Net::HTTPRequest &_request;
	std::istream *_stream;
};

}

#endif // H__PCACHEPROXY_REQUESTSTREAM__H
