#include "Entry.h"
#include "Global.h"
#include "Language.h"

ll::Logger logger(PLUGIN_NAME);

namespace EverywherePAPI {

std::unique_ptr<Entry>& Entry::getInstance() {
    static std::unique_ptr<Entry> instance;
    return instance;
}

bool Entry::load() { return true; }

bool Entry::enable() {
    mConfig.emplace();
    if (!ll::config::loadConfig(*mConfig, getSelf().getConfigDir() / u8"config.json")) {
        ll::config::saveConfig(*mConfig, getSelf().getConfigDir() / u8"config.json");
    }
    mI18n.emplace(getSelf().getLangDir(), mConfig->language);
    mI18n->updateOrCreateLanguage("en_US", en_US);
    mI18n->updateOrCreateLanguage("zh_CN", zh_CN);
    mI18n->loadAllLanguages();
    RegisterCommand();
    if (mConfig->Translate.Chat) {
        ChatPAPI::enable();
    }
    if (mConfig->Translate.Sign) {
        SignBlockPAPI::enable();
    }
    logger.info(tr("info.loaded"));
    logger.info("{}: GroupMountain", tr("info.author"));
    logger.info("{}: https://github.com/GroupMountain/EverywherePAPI", tr("info.repository"));
    return true;
}

bool Entry::disable() {
    SignBlockPAPI::disable();
    ChatPAPI::disable();
    return true;
}

bool Entry::unload() { return true; }

Config& Entry::getConfig() { return mConfig.value(); }

LangI18n& Entry::getI18n() { return mI18n.value(); }

} // namespace EverywherePAPI

LL_REGISTER_PLUGIN(EverywherePAPI::Entry, EverywherePAPI::Entry::getInstance());

std::string tr(std::string const& key, std::vector<std::string> const& data) {
    return EverywherePAPI::Entry::getInstance()->getI18n().get(key, data);
}