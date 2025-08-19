# Test helper lib (builds tiny frame builder for tests)
add_library(fix-test-helpers
  helpers/FrameBuilder.cpp
)

# Expose test helper headers and project includes
target_include_directories(fix-test-helpers
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}                       # tests/helpers/*
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>    # project headers
)
target_link_libraries(fix-test-helpers PUBLIC fix-core)
target_compile_features(fix-test-helpers PUBLIC cxx_std_23)

# -------- Core parser tests --------
add_executable(test-core-parser core/ParserTests.cpp)
target_link_libraries(test-core-parser PRIVATE fix-core fix-test-helpers)
target_compile_features(test-core-parser PRIVATE cxx_std_23)
add_test(NAME core-parser COMMAND test-core-parser)

# -------- Protocol header validation tests --------
add_executable(test-protocol-header protocol/HeaderRulesTests.cpp)
target_link_libraries(test-protocol-header PRIVATE fix-protocol-fixt11 fix-core fix-test-helpers)
target_compile_features(test-protocol-header PRIVATE cxx_std_23)
add_test(NAME protocol-header COMMAND test-protocol-header)

# -------- Application message views tests --------
add_executable(test-application-messages protocol/ApplicationMessageTests.cpp)
target_link_libraries(test-application-messages PRIVATE fix-protocol-fix50 fix-core fix-test-helpers)
target_compile_features(test-application-messages PRIVATE cxx_std_23)
add_test(NAME application-messages COMMAND test-application-messages)

# -------- Engine session sequence tests --------
add_executable(test-engine-session engine/SessionSeqTests.cpp)
target_link_libraries(test-engine-session PRIVATE fix-engine fix-protocol-fixt11 fix-core fix-test-helpers)
target_compile_features(test-engine-session PRIVATE cxx_std_23)
add_test(NAME engine-session COMMAND test-engine-session)
