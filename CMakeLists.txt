cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 14)
project (libraryprojects)
set (BUILD_SHARED_LIBRARIES)
set(CMAKE_BUILD_TYPE Debug)

IF (CYGWIN OR WIN32)
    set(GCC_ADDITIONAL_COMPILE_FLAGS "")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_COMPILE_FLAGS}")
ELSE()
    set(GCC_ADDITIONAL_COMPILE_FLAGS "-fsanitize=undefined -frecord-gcc-switches")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_COMPILE_FLAGS}")
ENDIF()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/systemcommand/include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/crypto/include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/pythoncrypto/include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/generalutilities/include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/fileutilities/include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/datetime/include/"  
                    "${CMAKE_CURRENT_SOURCE_DIR}/mathutilities/include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/serialport/include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/eventtimer/include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/prettyprinter/include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/arduino/include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/templateobjects/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/common/")

set(SOURCE_BASE /opt/GitHub/tjlutils)

set (SYSTEMCOMMAND_SOURCES ${SOURCE_BASE}/systemcommand/src/systemcommand.cpp)
set (PYTHONCRYPTO_SOURCES ${SOURCE_BASE}/pythoncrypto/src/pythoncrypto.cpp)
set (CRYPTO_SOURCES ${SOURCE_BASE}/crypto/src/crypto.cpp)
set (GENERALUTILITIES_SOURCES ${SOURCE_BASE}/generalutilities/src/generalutilities.cpp)
set (FILEUTILITIES_SOURCES ${SOURCE_BASE}/fileutilities/src/fileutilities.cpp)
set (DATETIME_SOURCES ${SOURCE_BASE}/datetime/src/datetime.cpp)
set (MATHUTILITIES_SOURCES ${SOURCE_BASE}/mathutilities/src/mathutilities.cpp)
set (SERIALPORT_SOURCES ${SOURCE_BASE}/serialport/src/serialport.cpp)
set (EVENTTIMER_SOURCES ${SOURCE_BASE}/eventtimer/src/eventtimer.cpp)
set (PRETTYPRINTER_SOURCES ${SOURCE_BASE}/prettyprinter/src/prettyprinter.cpp)
set (ARDUINO_SOURCES ${SOURCE_BASE}/arduino/src/arduino.cpp)

add_library(tjlutils SHARED ${SYSTEMCOMMAND_SOURCES}
                            ${PYTHONCRYPTO_SOURCES}
                            ${CRYPTO_SOURCES}
                            ${GENERALUTILITIES_SOURCES}
                            ${FILEUTILITIES_SOURCES}
                            ${DATETIME_SOURCES}
                            ${MATHUTILITIES_SOURCES}
                            ${SERIALPORT_SOURCES}
                            ${EVENTTIMER_SOURCES}
                            ${PRETTYPRINTER_SOURCES}
                            ${ARDUINO_SOURCES})
                        
add_library(tjlutilsstatic STATIC ${SYSTEMCOMMAND_SOURCES}
                                  ${PYTHONCRYPTO_SOURCES}
                                  ${CRYPTO_SOURCES}
                                  ${GENERALUTILITIES_SOURCES}
                                  ${FILEUTILITIES_SOURCES}
                                  ${DATETIME_SOURCES}
                                  ${MATHUTILITIES_SOURCES}
                                  ${SERIALPORT_SOURCES}
                                  ${EVENTTIMER_SOURCES}
                                  ${PRETTYPRINTER_SOURCES}
                                  ${ARDUINO_SOURCES})

set_target_properties(tjlutilsstatic PROPERTIES OUTPUT_NAME tjlutils)
