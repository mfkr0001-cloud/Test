#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr int kPanelTag = 0x474454;
    bool noclipEnabled() {
        return Mod::get()->getSettingValue<bool>("noclip-enabled");
    }
    void toggleNoclip() {
        const bool next = !noclipEnabled();
        Mod::get()->setSettingValue("noclip-enabled", next);
        Notification::create(next ? "No-Clip enabled (basic)" : "No-Clip disabled",
            NotificationIcon::Info)->show();
    }
}

class GDToolsPanelDelegate : public CCNode {
    CCLabelBMFont* m_label = nullptr;
public:
    static GDToolsPanelDelegate* create() {
        auto* ret = new GDToolsPanelDelegate();
        if (ret && ret->init()) { ret->autorelease(); return ret; }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    void setStatusLabel(CCLabelBMFont* label) { m_label = label; }
    void onToggle(CCObject*) {
        toggleNoclip();
        if (m_label) {
            m_label->setString(noclipEnabled() ? "GD Tools | NOCLIP: ON" : "GD Tools | NOCLIP: OFF");
            m_label->setColor(noclipEnabled() ? ccc3(80, 255, 130) : ccc3(255, 220, 220));
        }
    }
};

namespace {
    void createPanel(PlayLayer* layer) {
        if (!Mod::get()->getSettingValue<bool>("show-panel")) return;
        if (layer->getChildByTag(kPanelTag)) return;

        auto win = CCDirector::sharedDirector()->getWinSize();
        auto* delegate = GDToolsPanelDelegate::create();
        auto* menu = CCMenu::create();
        if (!delegate || !menu) return;

        menu->setID("gd-tools-floating-panel");
        menu->setTag(kPanelTag);
        menu->setPosition({win.width - 12.f, win.height - 12.f});
        menu->setZOrder(9999);
        menu->addChild(delegate);

        auto* label = CCLabelBMFont::create(
            noclipEnabled() ? "GD Tools | NOCLIP: ON" : "GD Tools | NOCLIP: OFF",
            "bigFont.fnt"
        );
        if (!label) return;
        label->setScale(0.38f);
        label->setAnchorPoint({1.f, 1.f});
        label->setColor(noclipEnabled() ? ccc3(80, 255, 130) : ccc3(255, 220, 220));
        delegate->setStatusLabel(label);

        auto* button = CCMenuItemLabel::create(
            label, delegate, menu_selector(GDToolsPanelDelegate::onToggle)
        );
        if (!button) return;
        button->setID("gd-tools-noclip-toggle");
        menu->addChild(button);
        button->setPosition({-label->getContentSize().width * 0.19f,
                             -label->getContentSize().height * 0.19f});
        layer->addChild(menu, 9999);
    }
}

class $modify(GDTools_PlayLayer, PlayLayer) {
    void onEnter() {
        PlayLayer::onEnter();
        createPanel(this);
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (noclipEnabled()) {
            // Basic no-death mode: bypasses the normal death handler only.
            return;
        }
        PlayLayer::destroyPlayer(player, object);
    }
};

$on_mod(Loaded) {
    log::info("GD Tools - Replay Lab loaded. Target GD Android version: 2.208.");
}
