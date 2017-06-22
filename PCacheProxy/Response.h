#ifndef H__PCACHEPROXY_RESPONSE__H
#define H__PCACHEPROXY_RESPONSE__H

#include <Poco/Net/HTTPResponse.h>

namespace PCacheProxy {

class Response
{
public:
	virtual ~Response() {}

	virtual Poco::Net::HTTPResponse &response() = 0;
	virtual std::ostream &send() = 0;
	virtual bool sent() const = 0;
};

}

#endif // H__PCACHEPROXY_RESPONSE__H
