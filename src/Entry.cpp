#include "Entry.h"
#include "Global.h"

ll::Logger logger(PLUGIN_NAME);

namespace EverywherePAPI {

std::unique_ptr<Entry>& Entry::getInstance() {
    static std::unique_ptr<Entry> instance;
    return instance;
}

bool Entry::load() {
    initConfig();
    return true;
}

bool Entry::enable() {
    enablePlugin();
    RegisterCommand();
    logger.info(tr("info.loaded"));
    logger.info("{}: GroupMountain", tr("info.author"));
    logger.info("{}: https://github.com/GroupMountain/EverywherePAPI", tr("info.repository"));
    return true;
}

bool Entry::disable() {
    disablePlugin();
    return true;
}

} // namespace EverywherePAPI

LL_REGISTER_PLUGIN(EverywherePAPI::Entry, EverywherePAPI::Entry::getInstance());