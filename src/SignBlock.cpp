#include "Global.h"

namespace SignBlockPAPI {

using namespace ll::chrono_literals;

std::set<std::pair<BlockPos, DimensionType>> mSignBlockActorMap;

ll::schedule::GameTickScheduler mScheduler;

std::shared_ptr<ll::schedule::task::Task<ll::chrono::GameTickClock>> mUpdateTask = nullptr;

bool mIsEnabled = false;

LL_TYPE_INSTANCE_HOOK(
    SignBlockTickHook,
    ll::memory::HookPriority::Normal,
    SignBlockActor,
    "?tick@SignBlockActor@@UEAAXAEAVBlockSource@@@Z",
    void,
    class BlockSource& region
) {
    auto pos   = getPosition();
    auto dimId = region.getDimensionId();
    mSignBlockActorMap.insert({pos, dimId});
    return origin(region);
}

void updateAllClientSignBlocks() {
    for (auto& data : mSignBlockActorMap) {
        auto bs  = GMLIB_Level::getLevel()->getBlockSource(data.second);
        auto pos = data.first;
        auto ba  = bs->getBlockEntity(pos);
        if (ba) {
            auto signblock = (SignBlockActor*)ba;
            auto backText  = signblock->getMessage(SignTextSide::Back);
            auto frontText = signblock->getMessage(SignTextSide::Front);
            auto nbt       = GMLIB_CompoundTag::getFromBlockActor(signblock)->clone();
            auto back      = nbt->getCompound("BackText");
            auto front     = nbt->getCompound("FrontText");
            auto pls       = GMLIB_Level::getLevel()->getAllPlayers();
            for (auto pl : pls) {
                if (!pl->isSimulated()) {
                    std::string backplaceHolder  = GMLIB::Server::PlaceholderAPI::translateString(backText, pl);
                    std::string frontplaceHolder = GMLIB::Server::PlaceholderAPI::translateString(frontText, pl);
                    back->putString("Text", backplaceHolder);
                    front->putString("Text", frontplaceHolder);
                    GMLIB_BinaryStream bs;
                    bs.writeBlockPos(pos);
                    bs.writeCompoundTag(*nbt);
                    GMLIB::Server::NetworkPacket<56> pkt(bs.getAndReleaseData());
                    pkt.sendTo(*pl);
                }
            }
        }
    }
}

void initSchedule() {
    mUpdateTask = mScheduler.add<ll::schedule::RepeatTask>(20_tick, [] {
        updateAllClientSignBlocks();
        mSignBlockActorMap.clear();
    });
}

void removeSchedule() {
    mScheduler.remove(mUpdateTask);
    mUpdateTask = nullptr;
}

std::unordered_map<SignBlockActor*, std::pair<std::string, std::string>> mSignTextCache;

LL_STATIC_HOOK(
    ChangeSignBlockHook,
    ll::memory::HookPriority::Normal,
    "?_canChangeSign@SignBlock@@CA_NAEAVSignBlockActor@@AEAVPlayer@@@Z",
    bool,
    class SignBlockActor& blockActor,
    class Player&         player
) {
    auto result = origin(blockActor, player);
    if (result) {
        std::string frontplaceHolder = blockActor.getMessage(SignTextSide::Front);
        std::string backplaceHolder  = blockActor.getMessage(SignTextSide::Back);
        GMLIB::Server::PlaceholderAPI::translate(frontplaceHolder, &player);
        GMLIB::Server::PlaceholderAPI::translate(backplaceHolder, &player);
        mSignTextCache.emplace(&blockActor, std::pair<std::string, std::string>{frontplaceHolder, backplaceHolder});
    }
    return result;
}

LL_TYPE_INSTANCE_HOOK(
    BlockActorDataPacketHook,
    ll::memory::HookPriority::Normal,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@V?$shared_ptr@VBlockActorDataPacket@@@std@@@Z",
    void,
    class NetworkIdentifier const&              source,
    std::shared_ptr<class BlockActorDataPacket> packet
) {
    auto& bs        = this->getServerPlayer(source, packet->mClientSubId)->getDimensionBlockSource();
    auto  pos       = (BlockPos)packet->mPos;
    auto  signblock = (SignBlockActor*)bs.getBlockEntity(pos);
    if (signblock) {
        auto it = mSignTextCache.find(signblock);
        if (it != mSignTextCache.end()) {
            auto SignBlockActorNbt = &packet->mData;
            auto front             = SignBlockActorNbt->getCompound("FrontText");
            if (!it->second.first.empty() && front->getString("Text") == it->second.first) {
                front->putString("Text", signblock->getMessage(SignTextSide::Front));
            }
            auto back = SignBlockActorNbt->getCompound("BackText");
            if (!it->second.second.empty() && back->getString("Text") == it->second.second) {
                back->putString("Text", signblock->getMessage(SignTextSide::Back));
            }
            mSignTextCache.erase(it);
        }
    }
    return origin(source, packet);
}

void enable() {
    ll::memory::HookRegistrar<SignBlockTickHook, ChangeSignBlockHook, BlockActorDataPacketHook>().hook();
    initSchedule();
    mIsEnabled = true;
}

void disable() {
    if (mIsEnabled) {
        ll::memory::HookRegistrar<SignBlockTickHook, ChangeSignBlockHook, BlockActorDataPacketHook>().unhook();
        removeSchedule();
    }
}

} // namespace SignBlockPAPI