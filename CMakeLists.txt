project(PCacheProxy)
cmake_minimum_required(VERSION 2.8)

find_package (Threads)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
FIND_PACKAGE(Poco REQUIRED Zip Net Util Foundation)
#IF(Poco_FOUND)
  INCLUDE_DIRECTORIES(${Poco_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Poco_LIBRARY_DIR})
  add_definitions(-DPOCO_STATIC -DPOCO_NO_AUTOMATIC_LIBS)
#ENDIF(Poco_FOUND)


SET( TARGET_BASE_NAME pcacheproxy )

IF(ANDROID)
	add_definitions(-DPOCO_ANDROID)
ENDIF()

#-----------------------------------------------------------------------------

SET( EXE_NAME "${TARGET_BASE_NAME}" )

include_directories(.)


# Add any source files here.
SET( EXE_SRCS
	PCacheProxy/Service.cpp
	PCacheProxy/Exception.cpp
	PCacheProxy/ServiceServerRequest.cpp
	PCacheProxy/ServiceServerParamRequest.cpp
	PCacheProxy/Util.cpp
	PCacheProxy/Rfc2616CacheValidator.cpp
	
	PCacheProxy/MemoryCache.cpp
	PCacheProxy/MemoryCacheInfo.cpp
)

# Add any include files here.
SET( EXE_INCS
	PCacheProxy/Service.h
	PCacheProxy/Cache.h
	PCacheProxy/CacheGet.h
	PCacheProxy/CacheStore.h
	PCacheProxy/CacheValidator.h
	PCacheProxy/Rfc2616CacheValidator.h
	PCacheProxy/Exception.h
	PCacheProxy/Request.h
	PCacheProxy/Response.h
	PCacheProxy/ServiceServerRequest.h
	PCacheProxy/ServiceServerParamRequest.h
	PCacheProxy/Util.h

	PCacheProxy/RequestStream.h
	PCacheProxy/ResponseStream.h
	PCacheProxy/RequestServer.h
	PCacheProxy/ResponseServer.h
	
	PCacheProxy/MemoryCache.h
	PCacheProxy/MemoryCacheInfo.h
)

#-----------------------------------------------------------------------------

SET(BASE_SRCS
		pcacheproxy.cpp
)
SET(BASE_INCS
)

# The executable.
ADD_EXECUTABLE(${EXE_NAME} ${BASE_SRCS} ${BASE_INCS} ${EXE_SRCS} ${EXE_INCS})

TARGET_LINK_LIBRARIES ( ${EXE_NAME}
  ${Poco_LIBRARIES}
  
  ${CMAKE_THREAD_LIBS_INIT}
  ${CMAKE_DL_LIBS}
)
