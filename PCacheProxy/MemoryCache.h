#ifndef H__PCACHEPROXY_MEMORYCACHE__H
#define H__PCACHEPROXY_MEMORYCACHE__H

#include <PCacheProxy/Cache.h>
#include <PCacheProxy/MemoryCacheInfo.h>

#include <Poco/AccessExpireLRUCache.h>

namespace PCacheProxy {

class MemoryCache : public Cache
{
public:
	MemoryCache();

	CacheGet::Ptr get(Request &request, Response &response);
	CacheStore::Ptr store(Request &request, Response &response);
protected:
	std::string buildKey(Request &request);
private:
	friend class MemoryCacheStore;

	void internalStore(MemoryCacheInfo::Ptr info);

	Poco::AccessExpireLRUCache<std::string, MemoryCacheInfo::Ptr> _data;
};

}

#endif // H__PCACHEPROXY_MEMORYCACHE__H
