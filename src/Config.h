#pragma once
#include <iostream>

struct Config {
    int version = 1;

    std::string language = "en_US";

    struct translate {
        bool Chat = true;
        bool Sign = true;
    } Translate;
};