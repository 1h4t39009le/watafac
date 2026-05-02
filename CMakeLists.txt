cmake_minimum_required(VERSION 3.18)
project(watafac)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WFAC_INCLUDE_DIR
    ${CMAKE_SOURCE_DIR}/include CACHE INTERNAL ""
)

macro(add_wfac_library name)
    cmake_parse_arguments(ARG "STATIC;SHARED" "" "LINK_LIBS;PUBLIC_INCLUDES;PRIVATE_INCLUDES" ${ARGN})
    if(ARG_SHARED)
        set(LIB_TYPE SHARED)
    elseif(ARG_STATIC)
        set(LIB_TYPE STATIC)
    else()
        set(LIB_TYPE STATIC) 
    endif()
    add_library(${name} ${LIB_TYPE} ${ARG_UNPARSED_ARGUMENTS})
    target_include_directories(${name} 
        PUBLIC 
            ${WFAC_INCLUDE_DIR} 
            ${ARG_PUBLIC_INCLUDES} 
        PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR} 
            ${ARG_PRIVATE_INCLUDES}
    )
    if(ARG_LINK_LIBS)
        target_link_libraries(${name} PRIVATE ${ARG_LINK_LIBS})
    endif()
    add_library(wfac::${name} ALIAS ${name})
endmacro()


add_subdirectory(lib/src)
add_subdirectory(lib/diag)
add_subdirectory(lib/env)
add_subdirectory(lib/lex)
add_subdirectory(lib/ast)
add_subdirectory(lib/parse)

add_subdirectory(tools/wfac)
