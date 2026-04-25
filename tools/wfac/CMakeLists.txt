project(wfac)
add_executable(${PROJECT_NAME} wfac.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE wfac::source wfac::lex wfac::parse)
