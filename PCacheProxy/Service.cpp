#include "PCacheProxy/Service.h"
#include "PCacheProxy/Exception.h"
#include "PCacheProxy/Util.h"

#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/TeeStream.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace PCacheProxy {

Service::Service(Cache::Ptr cache) :
	_cache(cache)
{

}

void checkResponseStream(std::istream &stream)
{
	if (stream.bad())
		throw Exception("Stream error reading from connection");
	else if (stream.eof())
		return; // is ok
	else if (stream.fail())
		throw Exception("Stream failed reading from connection");
}

void Service::request(Request &request, Response &response)
{
	std::cout << "REQUEST URL: " << request.request().getURI() << std::endl;

	Poco::URI url(request.request().getURI());

	for (Poco::Net::HTTPResponse::ConstIterator ri = request.request().begin(); ri != request.request().end(); ++ri)
	{
		std::cout << "REQUEST HEADER: " << ri->first << " == " << ri->second << std::endl;
	}

	// check the cache database
	CacheGet::Ptr cacheget;
	if (!_cache.isNull())
		cacheget = _cache->get(request, response);

	if (!cacheget.isNull() && !cacheget->mustCheckModified())
	{
		std::cout << "CACHE: USED" << std::endl;
		cacheget->send();
		return;
	}

	// read REMOTE request
	Poco::Net::HTTPClientSession client(url.getHost(), url.getPort());

	if (!cacheget.isNull() && cacheget->mustCheckModified())
	{
		// send not modified header
		if (!cacheget->lastModified().isNull())
			request.request().add("If-Modified-Since", Util::formatTimestamp(cacheget->lastModified()));
		if (!cacheget->ETag().isNull())
			request.request().add("If-None-Match", cacheget->ETag());
	}

	std::ostream& dsend = client.sendRequest(request.request());
	if (request.stream() != NULL)
		Poco::StreamCopier::copyStream(*request.stream(), dsend);

	// read REMOTE response
	Poco::Net::HTTPResponse dresponse;
	std::istream& rs = client.receiveResponse(dresponse);

	// check if not modified
	if (!cacheget.isNull() && response.response().getStatus() == Poco::Net::HTTPResponse::HTTP_NOT_MODIFIED)
	{
		std::cout << "CACHE: USED [NOT_MODIFIED]" << std::endl;
		cacheget->send();
		return;
	}
	std::cout << "CACHE: NOT USED" << std::endl;

	// set LOCAL status and headers
	response.response().setStatusAndReason(dresponse.getStatus(), dresponse.getReason());
	for (Poco::Net::HTTPResponse::ConstIterator ri = dresponse.begin(); ri != dresponse.end(); ++ri)
	{
		std::cout << "RESPONSE HEADER: " << ri->first << " == " << ri->second << std::endl;
		response.response().add(ri->first, ri->second);
	}

	CacheStore::Ptr store;
	try
	{
		// request database storage
		if (!_cache.isNull())
		{
			store = _cache->store(request, response);
			if (!store.isNull())
				std::cout << "CACHE WILL STORE" << std::endl;
			else
				std::cout << "CACHE WON'T STORE" << std::endl;
		}

		// get output stream
		std::ostream& ostr = response.send();

		// create stream that will write to multiple streams
		Poco::TeeOutputStream output(ostr);
		if (!store.isNull())
			output.addStream(store->stream());

		// copy body from REMOTE to LOCAL
		Poco::StreamCopier::copyStream(rs, output);

		// check response stream
		checkResponseStream(rs);

		// commit store
		if (!store.isNull())
			store->commit();
	}
	catch (...)
	{
		if (!store.isNull())
			store->rollback();
		throw;
	}
}

}