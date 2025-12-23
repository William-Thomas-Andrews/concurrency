include(FetchContent)

# Fetch Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Test executable
add_executable(hashtable_tests
    test_main.cpp
    HashTableBasicTests.cpp
    HashTableInsertTests.cpp
    HashTableRemoveTests.cpp
    HashTableEdgeCaseTests.cpp
)

target_link_libraries(hashtable_tests
    hashtable_lib
    GTest::gtest
    GTest::gtest_main
)

# Discover tests for CTest
include(GoogleTest)
gtest_discover_tests(hashtable_tests)