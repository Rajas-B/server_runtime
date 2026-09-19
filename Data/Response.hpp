#pragma once
#include <memory>
#include <string>

#include "Headers.hpp"
#include "Body.hpp"

class Response {
    std::string method;
    std::string url;
    std::string version;

    std::unique_ptr<Headers> headers;
    std::unique_ptr<Body> body;
};