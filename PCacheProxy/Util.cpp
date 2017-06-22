#include "PCacheProxy/Util.h"

#include <Poco/StringTokenizer.h>

namespace PCacheProxy {
namespace Util {

std::string formatTimestamp(const Poco::Timestamp &value)
{
	return "";
}

void parseParams(const std::string &params, std::map<std::string, std::string> &ret)
{
	Poco::StringTokenizer tok(params, ",", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
	for (Poco::StringTokenizer::Iterator i = tok.begin(); i != tok.end(); ++i)
	{
		Poco::StringTokenizer vok(*i, "=", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
		if (vok.count() > 1)
			ret.emplace(vok[0], vok[1]);
		else
			ret.emplace(vok[0], "");
	}
}


} }
