# PCacheProxy

This is a prototype of a C++ HTTP 1.1 caching proxy using the [Poco](http://pocoproject.org) C++ library.

Currently implemented is a non-cache proxy mode, and a simple cache-to-memory mode.

### Standalone sample
```c++
	// use a memory cache
	PCacheProxy::Service proxy(new PCacheProxy::MemoryCache);

	// create an http request
	Poco::Net::HTTPRequest req("GET", "http://www.sibit.com.br", Poco::Net::HTTPRequest::HTTP_1_1);

	Poco::Net::HTTPResponse resp;
	std::stringstream respout;

	// request using the proxy, may return/store requests previously cached
	proxy.request(PCacheProxy::RequestStream(req), PCacheProxy::ResponseStream(resp, respout));

	std::cout << "==================== REQUEST OUTPUT ====================" << std::endl;
	std::cout << respout.str() << std::endl;
	std::cout << "====================" << std::endl;
```

# Author
Rangel Reale (rangelspam@gmail.com)
