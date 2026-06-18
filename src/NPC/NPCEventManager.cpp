#include "NPC/NPCEventManager.hpp"
#include "Shop/ShopEventManager.hpp"

#include "Util/Logger.hpp"

namespace {
    const char* kFairyPortrait =    RESOURCE_DIR "/Image/Character/NPC/Fairy1.png";
    const char* kElderPortrait =    RESOURCE_DIR "/Image/Character/NPC/Elder1.png";
    const char* kMerchantPortrait = RESOURCE_DIR "/Image/Character/NPC/Merchant1.png";
    const char* kThiefPortrait =    RESOURCE_DIR "/Image/Character/NPC/Thief1.png";
    const char* kPrincessPortrait = RESOURCE_DIR "/Image/Character/NPC/Princess1.png";
    const char* kHeroPortrait =     RESOURCE_DIR "/Image/Character/Player/player_d1.png";
    const char* kShopBossPortrait = RESOURCE_DIR "/Image/Character/Shop/ShopA2.png";
    const char* kShopBossPortrait2 = RESOURCE_DIR "/Image/Character/Shop/ShopB2.png";
}

void NPCEventManager::DispatchEvent(
    NPC::Type type,
    Player& player,
    Map& map,
    std::shared_ptr<NPC> npc,
    NPCDialog& npcDialog,
    ShopScene& shopScene,
    RewardMessage& rewardMessage
) {
    switch (type) {
        case NPC::Type::FAIRY1:
            HandleFairy1Event(player, map, npc, npcDialog);
            break;
        case NPC::Type::FAIRY2:
            HandleFairy2Event(player, map, npc, npcDialog);
            break;
        case NPC::Type::ELDER1:
            HandleElder1Event(player, map, npc, npcDialog, rewardMessage);
            break;
        case NPC::Type::ELDER2:
            HandleElder2Event(player, map, npc, npcDialog, rewardMessage);
            break;
        case NPC::Type::ELDER3:
            HandleElder3Event(player, map, npc, npcDialog, rewardMessage);
            break;
        case NPC::Type::MERCHANT1:
            HandleMerchant1Event(player, map, npc, npcDialog, rewardMessage);
            break;
        case NPC::Type::MERCHANT2:
            HandleMerchant2Event(player, map, npc, npcDialog, rewardMessage);
            break;
        case NPC::Type::THIEF:
            HandleThiefEvent(player, map, npc, npcDialog, rewardMessage);
            break;
        case NPC::Type::PRINCESS:
            HandlePrincessEvent(player, map, npcDialog, rewardMessage);
            break;
        case NPC::Type::SHOP_1:
            HandleShop1IntroEvent(player, npcDialog, shopScene);
            break;
        case NPC::Type::SHOP_2:
            HandleShop2Event(player, shopScene);
            break;
        case NPC::Type::ELDER_SHOP_1:
            HandleElderShop1Event(player, shopScene);
            break;
        case NPC::Type::ELDER_SHOP_2:
            HandleElderShop2Event(player, shopScene);
            break;
        case NPC::Type::MERCHANT_SHOP_1:
            HandleMerchantShop1Event(player, shopScene);
            break;
        case NPC::Type::MERCHANT_SHOP_2:
            HandleMerchantShop2Event(player, shopScene);
            break;
        default:
            LOG_INFO("This NPC has no event.");
            break;
    }
}

void NPCEventManager::HandleFairy1Event(Player& player,Map& map,std::shared_ptr<NPC> npc,NPCDialog& npcDialog) {
    if (player.m_Cross) {
        npcDialog.Start({
            {"勇士", "仙子，我已經將那個十字架找到了。", kHeroPortrait},
            {"仙子", "你做得很好。那麼，現在我就開始授予你更強的力量！", kFairyPortrait},
            {"仙子", "咪啦哆咪嘩......", kFairyPortrait},
            {"仙子", "好了，我已經將你現在的能力提升了！", kFairyPortrait},
            {"仙子", "記住：如果你沒有足夠實力的話，不要去第二十一層！", kFairyPortrait},
            {"仙子", "在那一層裡，你所有寶物的法力都會失去作用！", kFairyPortrait},
            {"仙子", "快走吧，殺死魔王後，來第二十二層上找我！", kFairyPortrait}
        }, [&player, &map, npc]() {
            map.RemoveNPC(npc);
            player.m_Hp = std::ceil(player.m_Hp * 4 / 3);
            player.m_Atk = std::ceil(player.m_Atk * 4 / 3);
            player.m_Def = std::ceil(player.m_Def * 4 / 3);
            player.m_Cross = false;
            map.SetTileAtLevel(20, 7, 5, 5);
        });
        return;
    }

    if (player.m_BlueVeri) {
        npcDialog.Start({
            {"仙子", "嗯？！你手裡的那個東西是什麼？", kFairyPortrait},
            {"勇士", "這個？這是一個老人交給我的，是他叫我帶它來找你的。", kHeroPortrait},
            {"勇士", "他說你知道它的來歷和作用。", kHeroPortrait},
            {"仙子", "這個東西是仙界的聖物名叫'靈之杖'，是很久以前的一個聖者留下的，它們一共有三個，分別鑲著紅、綠、藍三種顏色的寶石。", kFairyPortrait},
            {"仙子", "你現在拿著的是鑲有一顆藍寶石的'冰之靈杖'，應該還有一個鑲有綠寶石'心之靈杖'和鑲有紅寶石的'炎之靈杖'。", kFairyPortrait},
            {"仙子", "在這座塔下面，封印著一支魔界的世獸，名叫'血影'。", kFairyPortrait},
            {"仙子", "這三把'靈之杖'就是封印的鑰匙。", kFairyPortrait},
            {"勇士", "封印鑰匙？", kHeroPortrait},
            {"仙子", "每一個'靈之杖'裡面都有著很強大的魔法力量，如果被惡魔得到了將會使它的力量倍增。", kFairyPortrait},
            {"仙子", "如果被惡魔將它們三個找齊的話，那麼'血影'的封印便會解除！", kFairyPortrait},
            {"仙子", "勇士，你還是快去把我要的東西找來吧，等我恢復了魔力，我就可以幫你將'靈之杖'中的魔力都開放出來！", kFairyPortrait}
        });
        return;
    }

    npcDialog.Start({
        {"勇士", "......", kHeroPortrait},
        {"仙子", "你醒了！", kFairyPortrait},
        {"勇士", "你是誰，我在哪裡？", kHeroPortrait},
        {"仙子", "我是這裡的仙子，剛才你被這裡的小怪打昏了", kFairyPortrait},
        {"勇士", "劍！劍！我的劍呢？", kHeroPortrait},
        {"仙子", "你的劍被他們搶走了，我只來得及將你救出來而已", kFairyPortrait},
        {"勇士", "那公主呢?我是來救公主的", kHeroPortrait},
        {"仙子", "公主還在裡面，你這樣進去是打不過裡面的小怪的", kFairyPortrait},
        {"勇士", "哪我怎麼辦，我答應了國王一定要把公主救出來的，那我現在應該怎麼辦呀？", kHeroPortrait},
        {"仙子", "放心吧，我把我的力量借給你，你就可以打贏那些小怪了。不過，你得先幫我去找一樣東西，找到了再來這裡找我", kFairyPortrait},
        {"勇士", "找東西？什麼東西？", kHeroPortrait},
        {"仙子", "是一個十字架，中間有一顆紅色的寶石", kFairyPortrait},
        {"勇士", "那個東西有什麼用嗎", kHeroPortrait},
        {"仙子", "我本是這塔的守護者，可不久之前，從北方來了一批惡魔，他們佔領了這座塔，並將我的魔力封在了這個十字架裡面", kFairyPortrait},
        {"仙子", "如果你能將它帶出來，那我的魔力便會慢慢的恢復，到時候我就可以把力量借給你去救出公主了", kFairyPortrait},
        {"仙子", "要記住，只有用我的魔力才可以打開21層的門", kFairyPortrait},
        {"勇士", "好吧，我試試看", kHeroPortrait},
        {"仙子", "剛才我去看過了，你的劍被放在三樓，你的盾在五樓，而那個十字架被放在七樓，要到七樓，你得先取回你的劍和盾", kFairyPortrait},
        {"仙子", "另外，在塔裡的其他樓層，還有一些存放了好幾百年的寶劍和寶物，如果得到它們，對於你將會有很大的幫助", kFairyPortrait},
        {"仙子", "我這裡有三把鑰匙，你先拿去，在塔裡還有很多這樣的鑰匙，你一定要珍惜使用", kFairyPortrait},
        {"仙子", "勇敢地去吧，勇士！", kFairyPortrait},
    }, [&player, &map, npc]() {

        map.MoveNPC(npc, -1, 0);

        player.m_YellowKeys += 1;
        player.m_BlueKeys += 1;
        player.m_RedKeys += 1;
    });
}

void NPCEventManager::HandleFairy2Event(Player& player,Map& map,std::shared_ptr<NPC> npc,NPCDialog& npcDialog) {
    if (player.m_BlueVeri && player.m_RedVeri && player.m_GreenVeri) {
        npcDialog.Start({
            {"勇士", "快看，我全部都找到了，我找齊所有了！", kHeroPortrait},
            {"仙子", "嗯，不錯，現在我們可以接觸這裡面的封印了！", kFairyPortrait},
            {"仙子", "那就讓我們開始吧！神之'靈杖'呀！", kFairyPortrait},
            {"仙子", "放射出你們的魔力吧！哈哩咪哆唏咪啦～～～～", kFairyPortrait},
            {"勇士", "......（又來了！）", kHeroPortrait},
            {"仙子", "好了，我已經將他們三個'靈之杖'的魔力都開放出來了！", kFairyPortrait},
            {"仙子", "公主就由我來救出去，你快去最底層殺了那個大魔頭吧！", kFairyPortrait},
            {"仙子", "要記住，如果沒有萬分的把握，一定不要去開最後那扇花形門！", kFairyPortrait},
            {"仙子", "一旦進去了，你將不能再回來！", kFairyPortrait}
        }, [&map, npc]() {
            map.SetFinalSealReleased(true);
            map.RemoveNPC(npc);
        });
        return;

    }
    npcDialog.Start({
        {"仙子", "做得很好。現在你已經將那個可惡的冥靈魔王給殺了，快去找另外的兩個'靈杖'吧，找齊了以後再來找我！", kFairyPortrait},
        {"仙子", "要記住，如果我不把封印解開的話，最底層的怪物你是殺不了的！", kFairyPortrait},
    });
}

void NPCEventManager::HandleElder1Event(Player& player, Map& map, std::shared_ptr<NPC> npc, NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    npcDialog.Start({
        {"勇士", "您已經得救了！", kHeroPortrait},
        {"神秘老人", "喔，我的孩子，真是太感謝你了！", kElderPortrait},
        {"神秘老人", "這個地方又髒又壞，我真的是快待不下去了。", kElderPortrait},
        {"勇士", "快走吧，我還得救被關在這裡的公主。", kHeroPortrait},
        {"神秘老人", "這個東西就送給你吧，這還是我年輕的時候用過的。", kElderPortrait},
    }, [&player, &map, npc, &rewardMessage]() {
        map.RemoveNPC(npc);
        player.m_Atk += 30;
        rewardMessage.ShowItem("獲得鋼劍! 攻擊力 +30");
    });
}

void NPCEventManager::HandleElder2Event(Player& player, Map& map, std::shared_ptr<NPC> npc, NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    npcDialog.Start({
        {"神秘老人", "你好，勇敢的孩子，你終於來到這裡了。", kElderPortrait},
        {"神秘老人", "我將給你一個非常好的寶物，它可以使你的攻擊力提升 120 點，但這必須用你的 500 點經驗來交換，考慮一下吧！", kElderPortrait},
        {"勇士", "好吧，那就將那把劍給我吧！", kHeroPortrait},
        {"神秘老人", "那好吧，這把劍就交給你了！", kElderPortrait}
    }, [&player, &map, npc, &rewardMessage]() {
        const int costExp = 500;

        if (player.m_Exp < costExp) {
            rewardMessage.ShowItem("經驗不足，需要 500 點經驗!");
            return;
        }

        map.RemoveNPC(npc);
        player.m_Exp -= costExp;
        player.m_Atk += 120;
        rewardMessage.ShowItem("獲得聖光劍! 攻擊力 +120");
    });
}

void NPCEventManager::HandleElder3Event(Player& player, Map& map, std::shared_ptr<NPC> npc, NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    npcDialog.Start({
        {"神秘老人", "年輕人，你終於來了！", kElderPortrait},
        {"勇士", "您怎麼了？", kHeroPortrait},
        {"神秘老人", "我已經快封不住它了，請你將這個東西交給彩蝶仙子，她會告訴你這是什麼東西，有什麼用的！", kElderPortrait},
        {"神秘老人", "快去吧，再遲就來不及了！", kElderPortrait}
    }, [&player, &map, npc, &rewardMessage]() {
        map.RemoveNPC(npc);
        player.m_BlueVeri = true;
        rewardMessage.ShowItem("獲得神秘寶物！");
    });
}

void NPCEventManager::HandleMerchant1Event(Player& player, Map& map, std::shared_ptr<NPC> npc, NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    npcDialog.Start({
        {"勇士", "您已經得救了！", kHeroPortrait},
        {"商人", "喔，是嘛，真是太感謝你了！", kMerchantPortrait},
        {"商人", "我是個商人，不知為什麼被抓到這裡來了。", kMerchantPortrait},
        {"勇士", "快走吧，現在你已經自由了。", kHeroPortrait},
        {"商人", "那這個東西就給你吧，本來我是準備賣錢的。", kMerchantPortrait}
    }, [&player, &map, npc, &rewardMessage]() {
        map.RemoveNPC(npc);
        player.m_Def += 30;
        rewardMessage.ShowItem("獲得鋼盾! 防禦力 +30");
    });
}

void NPCEventManager::HandleMerchant2Event(Player& player, Map& map, std::shared_ptr<NPC> npc, NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    npcDialog.Start({
        {"商人", "啊哈，歡迎你的到來！我這裡有一件對你來說非常好的寶物，只要你出得起錢，我就賣給你。", kMerchantPortrait},
        {"勇士", "什麼寶物？要多少錢？", kHeroPortrait},
        {"商人", "是這個遊戲最好的盾牌，防禦力可以可以增加 120 點，而你只要出 500 個金幣就可以買下。", kMerchantPortrait},
        {"商人", "怎麼樣？你有 500 個金幣嗎？", kMerchantPortrait},
        {"勇士", "我有 500 個金幣。", kHeroPortrait},
        {"商人", "好，成交！", kMerchantPortrait},
    }, [&player, &map, npc, &rewardMessage]() {
        const int costCoin = 500;

        if (player.m_Coin < costCoin) {
            rewardMessage.ShowItem("金幣不足，需要 500 個金幣!");
            return;
        }

        map.RemoveNPC(npc);
        player.m_Coin -= costCoin;
        player.m_Def += 120;
        rewardMessage.ShowItem("獲得星光盾! 防禦力 +120");
    });
}

void NPCEventManager::HandleThiefEvent(Player& player, Map& map, std::shared_ptr<NPC> npc, NPCDialog& npcDialog,RewardMessage& rewardMessage) {
    if (player.m_HasGemDigger) {
        npcDialog.Start({
            {"勇士", "哈，快看，我找到了什麼！", kHeroPortrait},
            {"小偷", "太好了！這個東西果然是在這裡。", kThiefPortrait},
            {"小偷", "好吧，我這就去幫你修好第十八層的路面。", kThiefPortrait},
        }, [&player, &map, npc]() {
            player.m_HasGemDigger = false;

            map.SetTileAtLevel(18, 8, 5, 0);
            map.SetTileAtLevel(18, 9, 5, 0);
            map.RemoveNPC(npc);
        });
        return;
    }

    if (player.m_ThiefOpenedSecondFloorDoor) {
        npcDialog.Start({
            {"小偷", "如果你找到嵌了紅寶石的鐵榔頭，記得拿來給我。", kThiefPortrait},
            {"小偷", "有了它，我就能幫你打通第十八層的路。", kThiefPortrait}
        });
        return;
    }

    npcDialog.Start({
        {"勇士", "您已經得救了！", kHeroPortrait},
        {"小偷", "啊，那真是太好了，我又可以在這裡面尋寶了！", kThiefPortrait},
        {"小偷", "喔，還沒有自我介紹，我叫杰克，是這附近有名的小偷，什麼金銀財寶我樣樣都偷過。", kThiefPortrait},
        {"勇士", "快走吧，外面還有很多怪物，我可能顧不上你。", kHeroPortrait},
        {"小偷", "不，不，不會有事的。快說吧，叫我做什麼？", kThiefPortrait},
        {"勇士", "你會開門嗎？", kHeroPortrait},
        {"小偷", "那當然。", kThiefPortrait},
        {"勇士", "那就請你幫我打開第二層的門吧！", kHeroPortrait},
        {"小偷", "那個簡單，不過，如果你能幫我找到一把嵌了紅寶石的鐵榔頭，我還能幫你打通第十八層的路。", kThiefPortrait},
        {"勇士", "嵌了紅寶石的鐵榔頭？好吧，我幫你找找。", kHeroPortrait},
        {"小偷", "非常地感謝。一會我便會將第二層的門打開。", kThiefPortrait},
    }, [&player, &map]() {
        player.m_ThiefOpenedSecondFloorDoor = true;
        map.RemoveTileAtLevel(2, 6, 1);
    });
}

void NPCEventManager::HandlePrincessEvent(Player& player, Map& map, NPCDialog& npcDialog, RewardMessage& rewardMessage) {
    (void)player;

    npcDialog.Start({
        {"勇士", "公主！你得救了！", kHeroPortrait},
        {"公主", "啊，你是來救我的嗎？", kPrincessPortrait},
        {"勇士", "是的，我奉國王的命令來救你的", kHeroPortrait},
        {"勇士", "請你快隨我出去吧！", kHeroPortrait},
        {"公主", "不，我還不想走", kPrincessPortrait},
        {"勇士", "為什麼？這裡到處都是惡魔。", kHeroPortrait},
        {"公主", "正是因為這裡到處都是惡魔，所以才不可以就這樣出去，我要看著那個惡魔被殺死！", kPrincessPortrait},
        {"公主", "英勇的勇士，如果你能夠將那個大惡魔殺死，我就和你一起出去！", kPrincessPortrait},
        {"勇士", "大惡魔？我已經殺死了一個魔王！", kHeroPortrait},
        {"公主", "大惡魔在這座塔的最頂層，你殺死的可能只是一個小隊長之類的惡魔", kPrincessPortrait},
        {"勇士", "好，那你等著，等我殺了那個惡魔再來這裡找你！", kHeroPortrait},
        {"公主", "大惡魔比你剛才殺死的那個厲害多了，而且他還會變身，變身後的魔王他的攻擊力跟防禦力都會提生至少一半以上，你得小心！", kPrincessPortrait},
        {"公主", "請一定要殺死大魔王！", kPrincessPortrait}
    }, [&map]() {
        map.SetTileAtLevel(18, 10, 10, 5);
    });
}

void NPCEventManager::HandleShop1IntroEvent(
    Player& player,
    NPCDialog& npcDialog,
    ShopScene& shopScene
) {
    npcDialog.Start({
        {"商店老闆", "嗨，你好，英雄的勇士，這裡是怪物商店，告訴你一些操作方法： 使用上下鍵來選擇項目，按 Space或 Enter 可以用來確認你的選擇！", kShopBossPortrait, kShopBossPortrait2},
        {"商店老闆", "同時在商人或神秘老人處進行交易也是一樣的操作方法!知道了嗎？勇士！", kShopBossPortrait, kShopBossPortrait2},
    }, [&player, &shopScene]() {
        ShopEventManager::OpenShop(NPC::Type::SHOP_1, player, shopScene);
    });
}

void NPCEventManager::HandleShop2Event(Player& player, ShopScene& shopScene) {
    ShopEventManager::OpenShop(NPC::Type::SHOP_2, player, shopScene);
}

void NPCEventManager::HandleElderShop1Event(Player& player, ShopScene& shopScene) {
    ShopEventManager::OpenShop(NPC::Type::ELDER_SHOP_1, player, shopScene);
}

void NPCEventManager::HandleElderShop2Event(Player& player, ShopScene& shopScene) {
    ShopEventManager::OpenShop(NPC::Type::ELDER_SHOP_2, player, shopScene);
}

void NPCEventManager::HandleMerchantShop1Event(Player& player, ShopScene& shopScene) {
    ShopEventManager::OpenShop(NPC::Type::MERCHANT_SHOP_1, player, shopScene);
}

void NPCEventManager::HandleMerchantShop2Event(Player& player, ShopScene& shopScene) {
    ShopEventManager::OpenShop(NPC::Type::MERCHANT_SHOP_2, player, shopScene);
}
