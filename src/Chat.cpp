#include "Global.h"

namespace ChatPAPI {

ll::event::ListenerId mEventId;

bool mIsEnabled = false;

void enable() {
    auto listener = ll::event::EventBus::getInstance().emplaceListener<ll::event::player::PlayerChatEvent>(
        [](ll::event::player::PlayerChatEvent& ev) {
            ev.message() = GMLIB::Server::PlaceholderAPI::translateString(ev.message(), &ev.self());
        }
    );
    mEventId   = listener->getId();
    mIsEnabled = true;
}

void disable() {
    if (mIsEnabled) {
        ll::event::EventBus::getInstance().removeListener(mEventId);
    }
}

} // namespace ChatPAPI