add_library(lib_cornichon)

file(GLOB_RECURSE LIB_CORNICHON_SOURCES *.h *.c *.hpp *.cpp)

target_sources(
    lib_cornichon
    PRIVATE
        ${LIB_CORNICHON_SOURCES}
)

target_include_directories(
    lib_cornichon
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/cornichon>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/cornichon>
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

#target_link_libraries(
#    lib_cornichon
#    PUBLIC
#    PRIVATE
#)

set_target_properties(
    lib_cornichon
    PROPERTIES
    OUTPUT_NAME cornichon
)

install(TARGETS lib_cornichon)
