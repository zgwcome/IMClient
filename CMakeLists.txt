cmake_minimum_required(VERSION 3.5.0)
project(client VERSION 0.1.0 LANGUAGES C CXX)

LINK_DIRECTORIES(lib)

add_executable(client HttpServer.cpp GenerateTestUserSig.cpp IMServerImp.cpp jsoncpp/jsoncpp.cpp main.cpp)

TARGET_LINK_LIBRARIES(client ImSDK cryptopp z)