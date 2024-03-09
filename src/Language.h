#pragma once
#include "Global.h"

std::string en_US = R"(
    info.repository=Repository
    info.author=Author
    info.loaded=EverywherePAPI Loaded!
    papi.command.description=PlaceholderAPI command system
    papi.command.list=List of loaded PlaceholderAPI extension plugins:
    papi.command.reload=Successfully reloaded EverywherePAPI
    papi.command.info=%s's placeholder list:
    papi.command.infoall=List of all placeholder
    papi.command.unregister.success=Successfully unregister %s
    papi.command.unregister.fail=Unable to unregister %s. May not be registered or may have been cancelled
)";

std::string zh_CN = R"(
    info.repository=Github链接
    info.author=插件作者
    info.loaded=EverywherePAPI 已成功加载！
    papi.command.description=PlaceholderAPI 命令系统
    papi.command.list=已加载的 PlaceholderAPI 拓展插件列表：
    papi.command.reload=已经成功重载 EverywherePAPI
    papi.command.info=%s 的变量列表
    papi.command.infoall=所有变量列表
    papi.command.unregister.success=成功取消注册 %s
    papi.command.unregister.fail=无法取消注册 %s，可能没有被注册或者已经被取消
)";