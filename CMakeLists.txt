cmake_minimum_required(VERSION 2.8)

project(pgtools C)

find_package(libconfig REQUIRED)
find_package(PostgreSQL REQUIRED)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
  find_package(Git)
  if (GIT_FOUND)
        EXECUTE_PROCESS(
            COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            OUTPUT_VARIABLE "BUILD_VERSION"
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "Git version: ${BUILD_VERSION}")
  else(GIT_FOUND)
    set(BUILD_VERSION 0)
  endif(GIT_FOUND)
endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)

add_definitions(-DSHA1=${BUILD_VERSION})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe  -O -W -Wall -Wpointer-arith -Wno-unused-parameter -Werror -g -g -O2 -fPIE -fstack-protector-strong -Wformat -Werror=format-security -Wdate-time -Wl,-rpath,/usr/lib,-Bsymbolic-functions -fPIE -no-pie -Wl,-z,relro -Wl,-z,now -ldl -Wl,-E")

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/os/unix/pgt-errno.c
    ${PROJECT_SOURCE_DIR}/src/os/unix/pgt-alloc.c
    ${PROJECT_SOURCE_DIR}/src/os/unix/pgt-process.c
    ${PROJECT_SOURCE_DIR}/src/os/unix/pgt-time.c
    ${PROJECT_SOURCE_DIR}/src/core/pgt-string.c
    ${PROJECT_SOURCE_DIR}/src/core/pgt-times.c
    ${PROJECT_SOURCE_DIR}/src/core/pgt-log.c
    ${PROJECT_SOURCE_DIR}/src/core/pgt-connection.c
    ${PROJECT_SOURCE_DIR}/src/core/pgt.c
    ${PROJECT_SOURCE_DIR}/src/modules/pgt-modules.c
    ${PROJECT_SOURCE_DIR}/src/modules/transactions/pgt-modules-transactions-idle.c
)

add_executable(pgtools ${SOURCES})

target_include_directories(pgtools
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/os/unix
        ${PROJECT_SOURCE_DIR}/src/core
        ${PROJECT_SOURCE_DIR}/src/modules
        ${PROJECT_SOURCE_DIR}/src/modules/transactions
)

target_link_libraries(pgtools
    PRIVATE
        config
        PostgreSQL::PostgreSQL
)

install(TARGETS pgtools DESTINATION /usr/bin)