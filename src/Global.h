#pragma once
#include <include_all.h>

#define PLUGIN_NAME "EverywherePAPI"

extern ll::Logger logger;

extern std::string tr(std::string key, std::vector<std::string> data = {});
extern std::string trp(std::string key, std::vector<std::string> data = {}, Player* pl = nullptr);

extern void initConfig();

namespace SignBlockPAPI {
extern void enable();
extern void disable();
} // namespace SignBlock