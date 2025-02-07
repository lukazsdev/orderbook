project(orderbook_src)

set(HEADER_FILES
        book.h
        order.h
        trade.h
        types.h)

set(SOURCE_FILES
        book.cpp
        order.cpp
        trade.cpp)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})