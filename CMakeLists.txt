cmake_minimum_required(VERSION 3.20)

project(URLSHORTNER LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Download standalone Asio, which Crow needs for networking.
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-30-2
)

FetchContent_GetProperties(asio)

if(NOT asio_POPULATED)
    FetchContent_Populate(asio)
endif()

# Tell Crow where standalone Asio's headers are located.
set(ASIO_INCLUDE_DIR "${asio_SOURCE_DIR}/asio/include" CACHE PATH "" FORCE)
set(ASIO_INCLUDE_DIRS "${ASIO_INCLUDE_DIR}")

FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.2.1
)

FetchContent_MakeAvailable(Crow)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)

add_executable(url_shortener
    src/main.cpp
    src/url_shortener.cpp
    src/short_code_generator.cpp
    src/url_service.cpp
    src/database.cpp
)

target_include_directories(url_shortener
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
    url_shortener 
    PRIVATE 
        Crow::Crow
        PkgConfig::PQXX
)