set(LIBNAME agent
)
add_library(${LIBNAME} STATIC
        include/dlgg/agent/include/agent.h
        src/agent.cpp
        include/dlgg/agent/include/randombot.h
)

add_library(${PACKAGE_NAME}::${LIBNAME} ALIAS ${LIBNAME})
target_include_directories(${LIBNAME} PUBLIC ${PROJECT_SOURCE_DIR}/${PACKAGE_NAME}/${LIBNAME}/include)
message(${PROJECT_SOURCE_DIR}/${PACKAGE_NAME}/${LIBNAME}/include)

target_link_libraries(${LIBNAME} dlgg::game)

