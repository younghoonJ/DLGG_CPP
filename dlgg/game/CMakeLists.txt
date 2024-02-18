set(LIBNAME game)
add_library(${LIBNAME} STATIC
        include/dlgg/game/include/zobrist.h
        src/gotypes.cpp
        include/dlgg/game/include/gotypes.h
        src/gostring.cpp
        include/dlgg/game/include/gostring.h
        include/dlgg/game/include/goboard.h
        src/goboard_impl.cpp
        include/dlgg/game/include/goboard_impl.h
        include/dlgg/game/include/gamestate.h
        src/gamestate_impl.cpp
        include/dlgg/game/include/gamestate_impl.h
)

add_library(${PACKAGE_NAME}::${LIBNAME} ALIAS ${LIBNAME})
target_include_directories(${LIBNAME} PUBLIC ${PROJECT_SOURCE_DIR}/${PACKAGE_NAME}/${LIBNAME}/include)
message(${PROJECT_SOURCE_DIR}/${PACKAGE_NAME}/${LIBNAME}/include)

