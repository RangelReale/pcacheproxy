#include "PCacheProxy/MemoryCache.h"
#include "PCacheProxy/Util.h"
#include "PCacheProxy/Rfc2616CacheValidator.h"

#include <Poco/URI.h>
#include <Poco/MD5Engine.h>
#include <Poco/DigestStream.h>

#include <map>
#include <sstream>

namespace PCacheProxy {

//
// MemoryCacheGet
//
class MemoryCacheGet : public CacheGet
{
public:
	MemoryCacheGet(MemoryCache &cache, MemoryCacheInfo::Ptr info, Response &response) :
		CacheGet(), _cache(cache), _info(info), _response(response)
	{

	}

	bool mustCheckModified()
	{
		return false;
	}

	Poco::Nullable<Poco::Timestamp> lastModified()
	{
		return Poco::Nullable<Poco::Timestamp>();
	}

	Poco::Nullable<std::string> ETag()
	{
		return Poco::Nullable<std::string>();
	}

	void send()
	{
		// status
		_response.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_OK);

		// headers
		for (std::multimap<std::string, std::string>::const_iterator hi = _info->headers.cbegin(); hi != _info->headers.cend(); ++hi)
		{
			_response.response().add(hi->first, hi->second);
		}

		// get output stream
		std::ostream& ostr = _response.send();
		ostr << _info->data;
	}
private:
	MemoryCache &_cache;
	MemoryCacheInfo::Ptr _info;
	Response &_response;
};

//
// MemoryCacheStore
//
class MemoryCacheStore : public CacheStore
{
public:
	MemoryCacheStore(MemoryCache &cache, MemoryCacheInfo::Ptr info) :
		CacheStore(), _cache(cache), _info(info)
	{
	}

	std::ostream &stream()
	{
		return _stream;
	}

	void commit()
	{
		_info->data = _stream.str();
		_cache.internalStore(_info);
	}

	void rollback()
	{

	}
private:
	MemoryCache &_cache;
	MemoryCacheInfo::Ptr _info;
	std::stringstream _stream;
};

//
// MemoryCache
//

MemoryCache::MemoryCache() :
	Cache()
{

}

CacheGet::Ptr MemoryCache::get(Request &request, Response &response)
{
	Rfc2616CacheValidator cvalid;
	if (!cvalid.isCacheable(request))
		return CacheGet::Ptr();

	std::string key = buildKey(request);
	Poco::SharedPtr<MemoryCacheInfo::Ptr> find = _data.get(key);
	if (!find.isNull())
	{
		return new MemoryCacheGet(*this, *find, response);
	}
	return CacheGet::Ptr();
}

CacheStore::Ptr MemoryCache::store(Request &request, Response &response)
{
	Rfc2616CacheValidator cvalid;
	if (!cvalid.isStorable(request, response))
		return CacheStore::Ptr();

	std::string key = buildKey(request);

	MemoryCacheInfo::Ptr info(new MemoryCacheInfo);
	info->key = key;
	info->uri = request.request().getURI();
	for (Poco::Net::HTTPResponse::ConstIterator ri = response.response().begin(); ri != response.response().end(); ++ri)
	{
		info->headers.emplace(ri->first, ri->second);
	}

	return new MemoryCacheStore(*this, info);
}

void MemoryCache::internalStore(MemoryCacheInfo::Ptr info)
{
	_data.add(info->key, info);
}

std::string MemoryCache::buildKey(Request &request)
{
	// url hash + query parameters in alphabetical order hash
	// when multiple parameters with the same name, order by position in query string

	Poco::URI uri(request.request().getURI());
	if (uri.isRelative())
		return "";

	uri.setUserInfo("");
	uri.setFragment("");

	std::multimap<std::string, std::string> params;
	Poco::URI::QueryParameters qp(uri.getQueryParameters());
	for (Poco::URI::QueryParameters::const_iterator qi = qp.cbegin(); qi != qp.cend(); ++qi)
	{
		params.emplace(qi->first, qi->second);
	}

	uri.setRawQuery("");

	for (std::multimap<std::string, std::string>::const_iterator qmi = params.cbegin(); qmi != params.cend(); ++qmi)
	{
		uri.addQueryParameter(qmi->first, qmi->second);
	}

	Poco::MD5Engine uri_md5;
	uri_md5.update(uri.toString());

	return Poco::DigestEngine::digestToHex(uri_md5.digest());
}

}