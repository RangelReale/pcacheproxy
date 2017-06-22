#ifndef H__PCACHEPROXY_UTIL__H
#define H__PCACHEPROXY_UTIL__H

#include <Poco/Timestamp.h>

#include <map>

namespace PCacheProxy {
namespace Util {

	std::string formatTimestamp(const Poco::Timestamp &value);
	/**
	 * Comma-separated name/values, name/values "=" separated, no repetition.
	 */
	void parseParams(const std::string &params, std::map<std::string, std::string> &ret);

} }

#endif // H__PCACHEPROXY_UTIL__H
