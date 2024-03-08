#pragma once
#include <include_all.h>

#define PLUGIN_NAME "EverywherePAPI"

extern ll::Logger logger;

extern std::string tr(std::string key, std::vector<std::string> data = {});

extern void initConfig();
extern void enablePlugin();
extern void disablePlugin();
extern void reloadPlugin();
extern void RegisterCommand();

namespace SignBlockPAPI {
extern void enable();
extern void disable();
} // namespace SignBlockPAPI

namespace ChatPAPI {
extern void enable();
extern void disable();
} // namespace ChatPAPI