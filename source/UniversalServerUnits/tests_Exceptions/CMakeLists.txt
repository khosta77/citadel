set(TEST_NAME Exceptions_tests)
set(COVERAGE_FLAGS --coverage -g -O0)

if(APPLE)
    set(GTEST_ROOT "/opt/homebrew/opt/googletest")
    include_directories(${GTEST_ROOT}/include)
    include_directories(/opt/homebrew/opt/boost/include)
endif()

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(${TEST_NAME})
target_sources(${TEST_NAME} PUBLIC ${TEST_NAME}.cpp ../Exceptions.cpp)

target_include_directories(${TEST_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_options(${TEST_NAME} PUBLIC ${COVERAGE_FLAGS})

target_link_libraries(${TEST_NAME} PRIVATE
    ${GTEST_ROOT}/lib/libgtest.a
    ${GTEST_ROOT}/lib/libgmock.a
    ${GTEST_ROOT}/lib/libgtest_main.a)
target_link_libraries(${TEST_NAME} PRIVATE Threads::Threads)
target_link_libraries(${TEST_NAME} PRIVATE -fprofile-generate)

# Добавляем тесты в CTest
include(GoogleTest)
gtest_discover_tests(${TEST_NAME})

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
