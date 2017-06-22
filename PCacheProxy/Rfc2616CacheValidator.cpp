#include "PCacheProxy/Rfc2616CacheValidator.h"
#include "PCacheProxy/Util.h"

#include <Poco/NumberParser.h>

#include <map>

namespace PCacheProxy {

Rfc2616CacheValidator::Rfc2616CacheValidator() :
	CacheValidator()
{
	clear();
}

bool Rfc2616CacheValidator::isCacheable(Request &request)
{
	clear();

	// only GET is cacheable
	if (request.request().getMethod() != Poco::Net::HTTPRequest::HTTP_GET)
		return false;

	return true;
}

bool Rfc2616CacheValidator::isStorable(Request &request, Response &response)
{
	if (!isCacheable(request))
		return false;

	// Only cache "OK" status
	if (response.response().getStatus() != Poco::Net::HTTPResponse::HTTP_OK)
		return false;

	// check HTTP 1.0 Pragma
	if (response.response().has("Pragma"))
	{
		if (response.response().get("Pragma") == "no-cache")
			return false;
	}

	// check RESPONSE Cache-Control header (RFC2616 #14.9)
	if (response.response().has("Cache-Control"))
	{
		std::map<std::string, std::string> cache_control;
		for (Poco::Net::NameValueCollection::ConstIterator cci = response.response().find("Cache-Control"); cci != response.response().end() && cci->first == "Cache-Control"; ++cci)
			Util::parseParams(cci->second, cache_control);

		if (cache_control.find("private") != cache_control.end())
			// private, do not cache
			return false;

		if (cache_control.find("no-cache") != cache_control.end())
			// no cache
			return false;

		if (cache_control.find("no-store") != cache_control.end())
			// no store
			return false;

		std::map<std::string, std::string>::iterator cc_maxage = cache_control.find("max-age");
		if (cc_maxage != cache_control.end())
		{
			int p_maxage;
			if (Poco::NumberParser::tryParse(cc_maxage->second, p_maxage))
			{
				_maxage = p_maxage;
				if (p_maxage <= 0)
					return false;
			}
		}
	}

	// check REQUEST Cache-Control header (RFC2616 #14.9.3)
	if (request.request().has("Cache-Control"))
	{
		std::map<std::string, std::string> cache_control;
		for (Poco::Net::NameValueCollection::ConstIterator cci = request.request().find("Cache-Control"); cci != request.request().end() && cci->first == "Cache-Control"; ++cci)
			Util::parseParams(cci->second, cache_control);

		std::map<std::string, std::string>::iterator cc_maxage = cache_control.find("max-age");
		if (cc_maxage != cache_control.end())
		{
			int p_maxage;
			if (Poco::NumberParser::tryParse(cc_maxage->second, p_maxage))
			{
				_reqmaxage = p_maxage;
			}
		}

		std::map<std::string, std::string>::iterator cc_minfresh = cache_control.find("min-fresh");
		if (cc_minfresh != cache_control.end())
		{
			int p_minfresh;
			if (Poco::NumberParser::tryParse(cc_minfresh->second, p_minfresh))
			{
				_reqminfresh = p_minfresh;
			}
		}

		std::map<std::string, std::string>::iterator cc_maxstale = cache_control.find("max-stale");
		if (cc_maxstale != cache_control.end())
		{
			int p_maxstale;
			if (Poco::NumberParser::tryParse(cc_maxstale->second, p_maxstale))
			{
				_reqmaxstale = p_maxstale;
			}
		}
	}

	return true;
}

void Rfc2616CacheValidator::clear()
{
	_maxage.clear();

	_reqmaxage.clear();
	_reqminfresh.clear();
	_reqmaxstale.clear();
}


}