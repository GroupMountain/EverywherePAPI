#include "Global.h"
#include <magic_enum.hpp>

struct PapiParam {
    enum class PAPIEnumList { list, reload } list;
    enum class PAPIEnumInfo { info } info;
    enum class PAPIEnumReg { unregister } unregister;
    ll::command::Optional<std::string> plugin;
    std::string                        papi;
};


void RegisterCommand() {
    auto& cmd = ll::command::CommandRegistrar::getInstance().getOrCreateCommand(
        "placeholder",
        tr("papi.command.description"),
        CommandPermissionLevel::GameDirectors
    );
    ll::service::getCommandRegistry()->registerAlias("placeholder", "papi");
    cmd.overload<PapiParam>()
        .required("list")
        .execute<[](CommandOrigin const& ori, CommandOutput& output, PapiParam const& param, ::Command const&) {
            switch (param.list) {
            case PapiParam::PAPIEnumList::list: {
                auto PAPIList = GMLIB::Server::PlaceholderAPI::getPAPIPluginsList();
                output.success(tr("papi.command.list"));
                int i = 0;
                for (auto& papi : PAPIList) {
                    std::string out = std::to_string(++i) + ".§a" + papi + "§r";
                    output.success(out);
                }
                return;
            }
            case PapiParam::PAPIEnumList::reload: {
                reloadPlugin();
                return output.success(tr("papi.command.reload"));
            }
            default:
                break;
            }
        }>();
    cmd.overload<PapiParam>()
        .required("info")
        .optional("plugin")
        .execute<[](CommandOrigin const& ori, CommandOutput& output, PapiParam const& param, ::Command const&) {
            auto list = GMLIB::Server::PlaceholderAPI::getPAPIInfoList();
            if (param.plugin.has_value()) {
                auto PAPIName = param.plugin.value();
                output.success(tr("papi.command.info", {PAPIName}));
                int i = 0;
                for (auto& papi : list) {
                    if (papi.getPluginName() == PAPIName) {
                        output.success(std::to_string(++i) + ".§a%" + papi.getName() + "%§r");
                    }
                }
            } else {
                output.success(tr("papi.command.infoall"));
                int i = 0;
                for (auto& papi : list) {
                    output.success(std::to_string(++i) + ".§a%" + papi.getName() + "%§r");
                }
            }
        }>();
    cmd.overload<PapiParam>()
        .required("unregister")
        .required("papi")
        .execute<[](CommandOrigin const& ori, CommandOutput& output, PapiParam const& param, ::Command const&) {
            auto result = GMLIB::Server::PlaceholderAPI::unRegisterPlaceholder(param.papi);
            if (result) {
                return output.success(tr("papi.command.unregister.success", {param.papi}));
            } else {
                return output.error(tr("papi.command.unregister.fail", {param.papi}));
            }
        }>();
}