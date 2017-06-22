#ifndef H__PCACHEPROXY_RFC2616CACHEVALIDATOR__H
#define H__PCACHEPROXY_RFC2616CACHEVALIDATOR__H

#include <PCacheProxy/CacheValidator.h>

#include <Poco/Nullable.h>

namespace PCacheProxy {

class Rfc2616CacheValidator : public CacheValidator
{
public:
	Rfc2616CacheValidator();

	bool isCacheable(Request &request);
	bool isStorable(Request &request, Response &response);

	Poco::Nullable<int> maxage() const { return _maxage;  }

	Poco::Nullable<int> reqMaxAge() const { return _reqmaxage; }
	Poco::Nullable<int> reqMinFresh() const { return _reqminfresh; }
	Poco::Nullable<int> reqMaxStale() const { return _reqmaxstale; }
protected:
	virtual void clear();
private:
	Poco::Nullable<int> _maxage;

	Poco::Nullable<int> _reqmaxage;
	Poco::Nullable<int> _reqminfresh;
	Poco::Nullable<int> _reqmaxstale;
};

}

#endif // H__PCACHEPROXY_RFC2616CACHEVALIDATOR__H
