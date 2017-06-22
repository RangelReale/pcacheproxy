#include "PCacheProxy/ServiceServerRequest.h"
#include "PCacheProxy/RequestServer.h"
#include "PCacheProxy/ResponseServer.h"

#include <Poco/URI.h>
#include <Poco/Net/HTMLForm.h>

namespace PCacheProxy {

ServiceServerRequest::ServiceServerRequest(Cache::Ptr cache) :
	Service(cache)
{

}

void ServiceServerRequest::serverRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse& response)
{
	this->request(RequestServer(request), ResponseServer(response));
}

void ServiceServerRequest::request(Request &request, Response &response)
{
	Service::request(request, response);
}


}