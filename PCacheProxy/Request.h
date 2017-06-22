#ifndef H__PCACHEPROXY_REQUEST__H
#define H__PCACHEPROXY_REQUEST__H

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Nullable.h>

#include <iostream>

namespace PCacheProxy {

class Request
{
public:
	virtual ~Request() {};

	virtual Poco::Net::HTTPRequest &request() = 0;
	virtual std::istream* stream() = 0;
};

}

#endif // H__PCACHEPROXY_REQUEST__H
