#pragma once
#include "Global.h"

std::string defaultConfig = R"({
    "language": "en_US"
})";

std::string defaultStaticFile = R"([
    {
        "Text": "Example Static Text 1",
        "Position": {
            "x": -5,
            "y": 90,
            "z": -5,
            "dim": 0
        }
    },
    {
        "Text": "Example\nStatic\nText 2",
        "Position": {
            "x": -5,
            "y": 95,
            "z": -5,
            "dim": 0
        }
    }
])";

std::string defaultDynamicFile = R"([
    {
        "Text": "Player Name: %player_realname%\nServer Mspt: %server_mspt%",
        "UpdateRate": 1,
        "Position": {
            "x": 5,
            "y": 90,
            "z": 5,
            "dim": 0
        }
    },
    {
        "Text": "Player Ping: %player_ping%ms\nServer Tps: %server_tps%",
        "UpdateRate": 2,
        "Position": {
            "x": 5,
            "y": 95,
            "z": 5,
            "dim": 0
        }
    }
])";