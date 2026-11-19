# C++ URL Shortener

## About

Build a high-performance URL Shortener application using C++17, Crow and PostgreSQL that converts long URLs into compact, unique short URLs. The application provides RESTful APIs to create short URLs, retrieve stored URL mappings, check application and database health, and redirect users to their original destinations.

It uses a modular C++ architecture with dedicated components for short-code generation, URL business logic, database operations and HTTP request handling, supported by automated unit and integration tests.

## Features

- **Short URL Generation:** Generate six-character short codes using uppercase letters, lowercase letters and digits.
- **URL Redirection:** Redirect users to the original URL using HTTP 302.
- **Persistent Storage:** Store URL mappings in PostgreSQL.
- **REST API:** Expose endpoints to create and retrieve shortened URLs.
- **Database Connectivity:** Check database connectivity through a dedicated endpoint.
- **Health Check:** Verify that the application is running.
- **Duplicate Code Handling:** Retry code generation if a generated code already exists in the database.
- **Automated Testing:** Unit and integration tests using GoogleTest and CTest (test implementation and coverage to be documented once completed).

## Technology Stack

| Technology | Version / Standard |
|---|---|
| Programming Language | C++17 |
| Build System | CMake 3.20 or later |
| HTTP Framework | Crow v1.2.1 |
| Networking Library | Standalone Asio 1.30.2 |
| Database | PostgreSQL 18.x (verify installed version) |
| PostgreSQL C++ Library | libpqxx 7.x (verify installed version) |
| Compiler | GCC (verify installed version) |
| Testing Framework | GoogleTest (to be integrated) |
| Test Runner | CTest |

## Project Architecture

The application follows a modular architecture that separates HTTP handling, business logic, short-code generation and database operations.

```text
Client
  |
  | HTTP Request
  v
Crow HTTP Server
  |
  v
UrlShortener
  |
  v
Url
