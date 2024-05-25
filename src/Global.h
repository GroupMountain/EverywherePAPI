#pragma once
#include <include_all.h>

#define PLUGIN_NAME "EverywherePAPI"

extern ll::Logger logger;

extern std::string tr(std::string const& key, std::vector<std::string> const& data = {});

extern void RegisterCommand();

namespace SignBlockPAPI {
extern void enable();
extern void disable();
} // namespace SignBlockPAPI

namespace ChatPAPI {
extern void enable();
extern void disable();
} // namespace ChatPAPI