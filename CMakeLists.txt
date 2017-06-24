cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 14)
project (tjlutils)
set (BUILD_SHARED_LIBRARIES)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_COMPILER /usr/bin/g++)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    set (X86 TRUE)
else()
    set (X86 FALSE)
endif()

if ((CYGWIN OR WIN32) OR (NOT X86))
    set(GCC_ADDITIONAL_COMPILE_FLAGS "")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_COMPILE_FLAGS}")
else()
    set(GCC_ADDITIONAL_COMPILE_FLAGS "-fsanitize=undefined")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_COMPILE_FLAGS}")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/systemcommand/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/pythoncrypto/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/generalutilities/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/fileutilities/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/datetime/"  
                    "${CMAKE_CURRENT_SOURCE_DIR}/mathutilities/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/serialport/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/prettyprinter/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/udpduplex/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/ibytestream/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/stringformat/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/templateobjects/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/eventtimer/"
		            "${CMAKE_CURRENT_SOURCE_DIR}/bitset/")

set(SOURCE_BASE ${CMAKE_CURRENT_SOURCE_DIR})

set (SYSTEMCOMMAND_SOURCES "${SOURCE_BASE}/systemcommand/systemcommand.cpp")
set (PYTHONCRYPTO_SOURCES "${SOURCE_BASE}/pythoncrypto/pythoncrypto.cpp")
set (GENERALUTILITIES_SOURCES "${SOURCE_BASE}/generalutilities/generalutilities.cpp")
set (FILEUTILITIES_SOURCES "${SOURCE_BASE}/fileutilities/fileutilities.cpp")
set (DATETIME_SOURCES "${SOURCE_BASE}/datetime/datetime.cpp")
set (MATHUTILITIES_SOURCES "${SOURCE_BASE}/mathutilities/mathutilities.cpp")
set (SERIALPORT_SOURCES "${SOURCE_BASE}/serialport/serialport.cpp")
set (PRETTYPRINTER_SOURCES "${SOURCE_BASE}/prettyprinter/prettyprinter.cpp")
set (UDPDUPLEX_SOURCES "${SOURCE_BASE}/udpduplex/udpduplex.cpp")
set (STRINGFORMAT_SOURCES "${SOURCE_BASE}/stringformat/stringformat.cpp")
set (ibytestream_SOURCES "${SOURCE_BASE}/ibytestream/tscriptexecutor.cpp" 
                     "${SOURCE_BASE}/ibytestream/tscriptreader.cpp")


add_library(tjlutils SHARED "${SYSTEMCOMMAND_SOURCES}"
                            "${PYTHONCRYPTO_SOURCES}"
                            "${GENERALUTILITIES_SOURCES}"
                            "${FILEUTILITIES_SOURCES}"
                            "${DATETIME_SOURCES}"
                            "${MATHUTILITIES_SOURCES}"
                            "${SERIALPORT_SOURCES}"
                            "${PRETTYPRINTER_SOURCES}"
                            "${UDPSERVER_SOURCES}"
                            "${UDPCLIENT_SOURCES}"
                            "${UDPDUPLEX_SOURCES}"
                            "${TCPSERVER_SOURCES}"
                            "${TCPCLIENT_SOURCES}"
                            "${TCPDUPLEX_SOURCES}"
                            "${STRINGFORMAT_SOURCES}"
                            "${ibytestream_SOURCES}")
                        
add_library(tjlutilsstatic STATIC "${SYSTEMCOMMAND_SOURCES}"
                                  "${PYTHONCRYPTO_SOURCES}"
                                  "${GENERALUTILITIES_SOURCES}"
                                  "${FILEUTILITIES_SOURCES}"
                                  "${DATETIME_SOURCES}"
                                  "${MATHUTILITIES_SOURCES}"
                                  "${SERIALPORT_SOURCES}"
                                  "${PRETTYPRINTER_SOURCES}"
                                  "${UDPSERVER_SOURCES}"
                                  "${UDPCLIENT_SOURCES}"
                                  "${UDPDUPLEX_SOURCES}"
                                  "${TCPSERVER_SOURCES}"
                                  "${TCPCLIENT_SOURCES}"
                                  "${TCPDUPLEX_SOURCES}"
                                  "${STRINGFORMAT_SOURCES}"
                                  "${ibytestream_SOURCES}")

set_target_properties(tjlutilsstatic PROPERTIES OUTPUT_NAME tjlutils)
