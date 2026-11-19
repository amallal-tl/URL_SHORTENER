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
UrlService
  |
  +----------------------+
  |                      |
  v                      v
ShortCodeGenerator     Database
                         |
                         v
                     PostgreSQL
```

### Main Components

| Component | Responsibility |
|---|---|
| `main.cpp` | Application entry point |
| `url_shortener` | Initializes the server and registers HTTP routes |
| `url_service` | Handles URL shortening and resolution business logic |
| `short_code_generator` | Generates unique six-character short codes |
| `database` | Handles PostgreSQL connectivity and database operations |

## Project Structure

```text
URL_SHORTENER/
│
├── CMakeLists.txt
├── README.md
│
├── include/
│   └── url_shortener/
│       ├── database.h
│       ├── short_code_generator.h
│       ├── url_service.h
│       └── url_shortener.h
│
├── src/
│   ├── main.cpp
│   ├── database.cpp
│   ├── short_code_generator.cpp
│   ├── url_service.cpp
│   └── url_shortener.cpp
│
├── tests/
│   ├── CMakeLists.txt
│   ├── unit/
│   └── integration/
│
└── build/
```

The `build/` directory contains generated build files and should not be committed to Git.

## REST API Documentation

The application exposes the following HTTP endpoints.

### 1. Application Health Check

**Endpoint:** `GET /health`

Checks whether the application server is running.

```bash
curl -i http://localhost:8080/health
```

Expected result: HTTP 200 OK.

### 2. Database Health Check

**Endpoint:** `GET /checkDB`

Checks connectivity to the configured PostgreSQL database.

```bash
curl -i http://localhost:8080/checkDB
```

Expected result: HTTP 200 OK when the database connection succeeds.

### 3. Create a Short URL

**Endpoint:** `POST /urls`

Creates a shortened URL and stores its mapping in PostgreSQL.

Request:

```bash
curl -i -X POST http://localhost:8080/urls \
  -H "Content-Type: application/json" \
  -d '{"url":"https://www.google.com"}'
```

Expected response: HTTP 201 Created.

```json
{
  "short_code": "Pn6GnQ",
  "short_url": "http://localhost:8080/r/Pn6GnQ"
}
```

The generated short code will differ for each request.

### 4. Redirect to the Original URL

**Endpoint:** `GET /r/{shortCode}`

Resolves the short code and redirects the client to the original URL.

```bash
curl -i http://localhost:8080/r/Pn6GnQ
```

Expected result:

```text
HTTP/1.1 302 Found
Location: https://www.google.com
```

If the short code does not exist:

```text
HTTP/1.1 404 Not Found
```

### 5. Retrieve All URL Mappings

**Endpoint:** `GET /urls`

Returns all stored short URL mappings.

```bash
curl -i http://localhost:8080/urls
```

Example response:

```json
{
  "urls": [
    {
      "short_code": "Pn6GnQ",
      "original_url": "https://www.google.com"
    }
  ]
}
```

## Database Configuration

The application uses PostgreSQL for persistent storage.

### Database Schema

Create the following table:

```sql
CREATE TABLE shortened_urls (
    short_code VARCHAR(10) PRIMARY KEY,
    original_url TEXT NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);
```

### Create Database and User

Open the PostgreSQL shell:

```bash
sudo -u postgres psql
```

Create the application database:

```sql
CREATE DATABASE url_shortener;
```

Create the application user:

```sql
CREATE USER url_app WITH PASSWORD 'your_password';
```

Grant database privileges:

```sql
GRANT ALL PRIVILEGES ON DATABASE url_shortener TO url_app;
```

Connect to the application database:

```sql
\c url_shortener
```

Grant schema privileges and create the table:

```sql
GRANT ALL ON SCHEMA public TO url_app;

CREATE TABLE shortened_urls (
    short_code VARCHAR(10) PRIMARY KEY,
    original_url TEXT NOT NULL,
    created_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
);
```

Exit PostgreSQL:

```sql
\q
```

**Note:** Use your own secure password. Do not commit database credentials to Git.

## Prerequisites

The following tools and libraries are required:

- Ubuntu Linux or WSL2 with Ubuntu.
- GCC with C++17 support.
- CMake 3.20 or later.
- Git.
- PostgreSQL.
- libpqxx development package.
- pkg-config.

Crow and standalone Asio are fetched automatically through CMake FetchContent.

### Install Dependencies

```bash
sudo apt update

sudo apt install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    postgresql \
    postgresql-contrib \
    libpqxx-dev
```

Verify installed tools:

```bash
g++ --version
cmake --version
psql --version
pkg-config --modversion libpqxx
```

## Build Instructions

Clone the repository:

```bash
git clone <YOUR_GITHUB_REPOSITORY_URL>
```

Navigate into the project:

```bash
cd URL_SHORTENER
```

Configure the project:

```bash
cmake -S . -B build
```

Compile:

```bash
cmake --build build -j2
```

The executable will be generated at:

```text
build/url_shortener
```

## Running the Application

Start PostgreSQL:

```bash
sudo service postgresql start
```

Configure the database connection string:

```bash
export DATABASE_URL="host=localhost port=5432 dbname=url_shortener user=url_app password=your_password"
```

Start the application:

```bash
./build/url_shortener
```

The application listens on port 8080, provided the configured server port remains 8080.

Open another terminal and test:

```bash
curl -i http://localhost:8080/health
```

## Testing the Application

### Manual API Testing

You can test the application using:

- Terminal with `curl`.
- Postman.
- Any HTTP client that supports REST APIs.

Recommended testing sequence:

1. Start PostgreSQL.
2. Start the URL Shortener application.
3. Check application health using `/health`.
4. Check database connectivity using `/checkDB`.
5. Create a short URL using `POST /urls`.
6. Retrieve all mappings using `GET /urls`.
7. Open the generated short URL using `/r/{shortCode}`.
8. Try an invalid short code and verify the 404 response.

### Automated Testing

The project will use GoogleTest for C++ unit and integration tests and CTest to execute the test suite.

Planned test coverage:

| Component | Test coverage |
|---|---|
| ShortCodeGenerator | Code length and allowed characters |
| Database | Insert, retrieve, list and duplicate-code handling |
| UrlService | URL shortening and resolution |
| HTTP API | Endpoint responses, status codes, JSON and redirects |

A separate PostgreSQL test database will be used to isolate automated tests from development data.

Once the test configuration is implemented, the commands will be:

```bash
cmake -S . -B build

cmake --build build -j2

ctest --test-dir build --output-on-failure
```

**Testing status:** Automated test cases and their final commands should be marked complete only after implementation and successful execution.

## Deployment

### Local Linux / WSL2 Deployment

1. Install the required dependencies.
2. Configure PostgreSQL.
3. Create the application database and table.
4. Set `DATABASE_URL`.
5. Build the application using CMake.
6. Run the executable.
7. Verify the application through its health endpoint.

### Running in the Background

For temporary background execution:

```bash
nohup ./build/url_shortener > server.log 2>&1 &
```

Inspect the log:

```bash
tail -f server.log
```

Stop the process using its process ID:

```bash
pgrep -af url_shortener
kill <PID>
```

For production deployment, use a process supervisor such as systemd and configure appropriate database credentials, network access, HTTPS termination and monitoring.

## Design Considerations

- **Separation of Concerns:** HTTP handling, business logic, code generation and persistence are maintained in separate components.
- **Parameterized SQL:** Database queries use parameterized values to avoid SQL injection through URL data.
- **Persistent Storage:** PostgreSQL ensures URL mappings survive application restarts.
- **Collision Handling:** The service retries when a generated short code violates the database's unique constraint.
- **HTTP Redirects:** The application uses HTTP 302 responses to redirect users to their original URLs.
- **Configuration Through Environment Variables:** Database connection details are supplied through `DATABASE_URL`.

## Current Scope and Limitations

This project focuses on implementing the core URL-shortening workflow.

The current scope does not include:

- User authentication and authorization.
- URL expiration.
- Custom short codes.
- Rate limiting.
- Redis caching.
- Analytics and click tracking.
- Distributed deployment.
- Frontend web application.

These features can be considered future enhancements.

## Future Improvements

- Add URL expiration and deletion.
- Introduce click tracking and analytics.
- Add rate limiting.
- Add caching using Redis.
- Add Docker-based deployment.
- Introduce authentication.
- Add performance benchmarking and load testing.

## Learning Outcomes

This project demonstrates practical experience with:

- Modern C++17.
- Object-oriented design and modular architecture.
- REST API development using Crow.
- PostgreSQL database integration using libpqxx.
- SQL queries and transactions.
- Random code generation.
- Exception handling and database constraint management.
- CMake dependency management and project builds.
- Unit and integration testing.

## License

Add your preferred open-source license before publishing the repository.
