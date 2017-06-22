#include "PCacheProxy/ServiceServerParamRequest.h"

#include <Poco/URI.h>
#include <Poco/Net/HTMLForm.h>

namespace PCacheProxy {

ServiceServerParamRequest::ServiceServerParamRequest(Cache::Ptr cache, const std::string &urlParamName) :
	ServiceServerRequest(cache), _urlparamname(urlParamName)
{

}

void ServiceServerParamRequest::request(Request &request, Response &response)
{
	// parse "url" param
	Poco::URI uri(request.request().getURI());
	Poco::Net::HTMLForm params;
	params.read(uri.getRawQuery());

	if (!params.has(_urlparamname))
	{
		response.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "Parameter 'url' not sent");
		std::ostream& ostr = response.send();
		ostr << "Url not sent";
		return;
	}

	// change request uri and host
	Poco::URI url(params[_urlparamname]);
	if (url.isRelative())
	{
		response.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST, "'Url' must be absolute");
		std::ostream& ostr = response.send();
		ostr << "'Url' must be absolute";
		return;
	}
	request.request().setURI(params[_urlparamname]);
	request.request().setHost(url.getHost());

	// do the request
	ServiceServerRequest::request(request, response);
}

}