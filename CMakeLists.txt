cmake_minimum_required(VERSION 3.28)
project(forest_mq C)

set(CMAKE_C_STANDARD 11)

add_executable(forest_mq main.c
        consumer.c
        consumer.h
        provider.c
        provider.h
        tcp.c
        tcp.h
        queue.c
        queue.h
        exchange.c
        exchange.h
        config.c
        config.h
)

find_package(PkgConfig REQUIRED)


if (APPLE)
    target_include_directories(
            forest_mq
            PRIVATE
            /opt/homebrew/Cellar/yder/1.4.20/include
            /opt/homebrew/Cellar/orcania/2.3.3/include
            /opt/homebrew/Cellar/libmicrohttpd/1.0.1/include
            /opt/homebrew/Cellar/jansson/2.14/include
            /opt/homebrew/Cellar/gnutls/3.8.4/include
            /opt/homebrew/Cellar/ulfius/2.7.15/include
    )
elseif (UNIX)
    target_include_directories(
            forest_mq
            PRIVATE
    )
endif()

if (APPLE)
    target_link_libraries(
            forest_mq
            PRIVATE
            /opt/homebrew/Cellar/jansson/2.14/lib/libjansson.dylib
            /opt/homebrew/Cellar/ulfius/2.7.15/lib/libulfius.dylib
    )
elseif(UNIX)
    target_link_libraries(
            forest_mq
            PRIVATE
            /opt/homebrew/Cellar/jansson/2.14/lib/libjansson.dylib
            /opt/homebrew/Cellar/ulfius/2.7.15/lib/libulfius.dylib
    )
endif ()