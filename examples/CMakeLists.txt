# Examples are small executables that demonstrate usage.

add_executable(fix-decode-demo decode_demo.cpp)
target_link_libraries(fix-decode-demo PRIVATE fix-engine fix-protocol-fixt11 fix-core)
target_compile_features(fix-decode-demo PRIVATE cxx_std_23)

add_executable(fix-session-demo session_demo.cpp)
target_link_libraries(fix-session-demo PRIVATE fix-engine fix-protocol-fixt11 fix-core)
target_compile_features(fix-session-demo PRIVATE cxx_std_23)
