#include <Poco/URI.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/StreamCopier.h>

#include <PCacheProxy/ServiceServerParamRequest.h>
#include <PCacheProxy/RequestServer.h>
#include <PCacheProxy/ResponseServer.h>
#include <PCacheProxy/RequestStream.h>
#include <PCacheProxy/ResponseStream.h>
#include <PCacheProxy/MemoryCache.h>

#include <sstream>

class ProxyRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
	ProxyRequestHandler(PCacheProxy::Cache::Ptr cache) : _cache(cache) {};

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
	{
		try {
			//std::cout << "METHOD: " << request.getMethod() << std::endl;
			//std::cout << "URI: " << request.getURI() << std::endl;

			PCacheProxy::ServiceServerParamRequest proxy(_cache, "url");
			proxy.serverRequest(request, response);
		}
		catch (Poco::Exception &e)
		{
			std::cout << "ERROR: " << e.displayText() << std::endl;
			if (!response.sent())
			{
				std::ostream& ostr = response.send();
				ostr << "ERROR: " << e.displayText();
			}
		}
		catch (std::exception &e)
		{
			std::cout << "ERROR: " << e.what() << std::endl;
			if (!response.sent())
			{
				std::ostream& ostr = response.send();
				ostr << "ERROR: " << e.what();
			}
		}
	}
private:
	PCacheProxy::Cache::Ptr _cache;
};

class ProxyDumpHandler : public Poco::Net::HTTPRequestHandler
{
public:
	ProxyDumpHandler() {}

	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
	{
		try {
			std::cout << "================== DUMP BEGIN =================" << std::endl;
			std::cout << "METHOD: " << request.getMethod() << std::endl;
			std::cout << "URI: " << request.getURI() << std::endl;
			std::cout << "VERSION: " << request.getVersion() << std::endl;
			std::cout << "CONTENT-LENGTH: " << request.getContentLength() << std::endl;

			for (Poco::Net::HTTPResponse::ConstIterator ri = request.begin(); ri != request.end(); ++ri)
				std::cout << "REQUEST HEADER: " << ri->first << " == " << ri->second << std::endl;

			if (request.hasContentLength() && request.getContentLength() > 0)
			{
				std::cout << "************* CONTENT BEGIN ****************" << std::endl;
				Poco::StreamCopier::copyStream(request.stream(), std::cout);
				std::cout << std::endl << "************* CONTENT END ****************" << std::endl;
			}

			std::cout << "================== DUMP END =================" << std::endl;

			std::ostream& ostr = response.send();
			ostr << "OK";

		}
		catch (Poco::Exception &e)
		{
			std::cout << "ERROR: " << e.displayText() << std::endl;
			if (!response.sent())
			{
				std::ostream& ostr = response.send();
				ostr << "ERROR: " << e.displayText();
			}
		}
		catch (std::exception &e)
		{
			std::cout << "ERROR: " << e.what() << std::endl;
			if (!response.sent())
			{
				std::ostream& ostr = response.send();
				ostr << "ERROR: " << e.what();
			}
		}
	}
};

class ProxyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
	ProxyRequestHandlerFactory(PCacheProxy::Cache::Ptr cache) : _cache(cache) {}

	Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request)
	{
		Poco::URI uri(request.getURI());

		if (/*uri.getHost() == "127.0.0.1" &&*/ uri.getPath() == "/dump")
			return new ProxyDumpHandler;
		return new ProxyRequestHandler(_cache);
	}
private:
	PCacheProxy::Cache::Ptr _cache;
};

class App : public Poco::Util::ServerApplication
{
public:
	App() {}
protected:
	int main(const std::vector<std::string>& args)
	{
		try
		{
			//return manualRequest();

			PCacheProxy::Cache::Ptr cache(new PCacheProxy::MemoryCache);

			Poco::Net::ServerSocket svs(14500);
			Poco::Net::HTTPServer srv(new ProxyRequestHandlerFactory(cache), svs, new Poco::Net::HTTPServerParams);
			srv.start();

			//Poco::Thread::sleep(1000);
			//manualRequest();

			waitForTerminationRequest();
			srv.stop();
		}
		catch (Poco::Exception &e)
		{
			std::cout << "ERROR: " << e.displayText() << std::endl;
		}
		catch (std::exception &e)
		{
			std::cout << "ERROR: " << e.what() << std::endl;
		}
		return EXIT_OK;
	}

	int manualRequest()
	{
		PCacheProxy::Service proxy(new PCacheProxy::MemoryCache);

		//Poco::Net::HTTPRequest req("GET", "http://www.sibit.com.br", Poco::Net::HTTPRequest::HTTP_1_1);
		Poco::Net::HTTPRequest req("GET", "http://127.0.0.1:14500/dump", Poco::Net::HTTPRequest::HTTP_1_1);
		std::stringstream reqin;
		reqin << "{'u':'joao'}";
		req.setContentLength(reqin.tellp());

		Poco::Net::HTTPResponse resp;
		std::stringstream respout;

		proxy.request(PCacheProxy::RequestStream(req, reqin), PCacheProxy::ResponseStream(resp, respout));

		std::cout << "====================" << std::endl;

		std::cout << respout.str() << std::endl;

		std::cout << "====================" << std::endl;

		std::cin.ignore();

		return EXIT_OK;
	}
};

int main(int argc, char *argv[])
{
	App app;
	return app.run(argc, argv);
}
