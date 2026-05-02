project(wfac)
add_executable(${PROJECT_NAME} wfac.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE wfac::src wfac::diag  wfac::lex wfac::parse wfac::env)
