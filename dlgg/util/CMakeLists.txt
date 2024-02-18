set(LIBNAME util)
add_library(${LIBNAME} STATIC
        include/dlgg/util/include/util.h
        src/util.cpp
)

add_library(${PACKAGE_NAME}::${LIBNAME} ALIAS ${LIBNAME})
target_include_directories(${LIBNAME} PUBLIC ${PROJECT_SOURCE_DIR}/${PACKAGE_NAME}/${LIBNAME}/include)
message(${PROJECT_SOURCE_DIR}/${PACKAGE_NAME}/${LIBNAME}/include)

