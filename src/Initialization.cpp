#include "Config.h"
#include "Global.h"
#include "Language.h"

GMLIB::Files::JsonConfig*     Config   = nullptr;
GMLIB::Files::I18n::LangI18n* Language = nullptr;

void initConfig() {
    Config = new GMLIB::Files::JsonConfig("./plugins/EverywherePAPI/config/config.json", defaultConfig);
    Config->init();
    std::string langPath     = "./plugins/EverywherePAPI/language/";
    std::string languageCode = Config->getValue<std::string>({"language"}, "en_US");
    Language                 = new GMLIB::Files::I18n::LangI18n(langPath, languageCode);
    Language->updateOrCreateLanguage("en_US", en_US);
    Language->updateOrCreateLanguage("zh_CN", zh_CN);
    Language->loadAllLanguages();
    Language->chooseLanguage(languageCode);
}

std::string tr(std::string key, std::vector<std::string> data) { return Language->translate(key, data); }

std::string trp(std::string key, std::vector<std::string> data, Player* pl) {
    auto result = Language->translate(key, data);
    return GMLIB::Server::PlaceholderAPI::translateString(result, pl);
}