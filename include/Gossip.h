
#include <FameAlias.h>
namespace gossip {

    extern SKSE::RegistrationSet<const RE::TESObjectREFR*> playerSightEvent;
    extern SKSE::RegistrationSet<const RE::TESObjectREFR*> playerSightLostEvent;
    extern SKSE::RegistrationSet<const RE::BGSLocation*, const RE::BGSLocation*> playerRegionChange; 
    struct tolerance : bound<std::uint16_t> {
        typedef bound<std::uint16_t> limit;
        std::uint16_t raw = 0;
        std::uint16_t val = 0;
        tolerance(){};
        tolerance(std::uint16_t amt, std::uint16_t min, std::uint16_t max)
            : raw(amt), limit(min, max), val(clamp(raw)) {}
        tolerance(SKSE::SerializationInterface* evt) : limit(evt) {
            evt->ReadRecordData(raw);
            evt->ReadRecordData(val);
        }
        tolerance(std::uint16_t amt){};
        void set(int data) { 
            val = clamp(raw = data);
        }
        void mod(int data) { 
            val = clamp(raw += data);
        }
        operator int() { return val; }
        void save(SKSE::SerializationInterface* evt) {
            limit::save(evt);
            evt->WriteRecordData(raw);
            evt->WriteRecordData(val);
        }
        void operator()(std::uint16_t min, std::uint16_t max) { limit(min, max); }
        std::uint16_t getRaw() { return raw; }
    };
    class Gossip : 
        public RE::BSTEventSink<RE::BGSActorCellEvent>,
        public RE::BSTEventSink<RE::TESActiveEffectApplyRemoveEvent> {
    public:
       


        std::string version = "0.0.0";
        
        bool isActive = false;
        std::vector<RE::BGSLocation*> trackedLocations;
        infoMap fame;
        std::map<RE::BGSLocation*, std::map<RE::TESGlobal*, tolerance>> regionTolerance{};
        RE::TESGlobal* interest = nullptr;
        RE::TESGlobal* recognition = nullptr;
        //Originally planned to allow for NPCs to have fame.
        //std::map<RE::TESObjectREFR*, fameProfile> profiles;
        fameProfile profile;
        float nextNotif = 0.0f;
        static Gossip& getSingleton() {
            
            return o_gossip;
        }
        Gossip();
        bool setup(RE::TESGlobal* inte, RE::TESGlobal* recog) { 
            if (!inte || !recog) return false;
            RE::PlayerCharacter::GetSingleton()->AsBGSActorCellEventSource()->AddEventSink(this);
            RE::ScriptEventSourceHolder::GetSingleton()->GetEventSource<RE::TESActiveEffectApplyRemoveEvent>()->AddEventSink(this);
            
            interest = inte;
            auto entry = fame.find(inte);
            if (entry != fame.end()) fame.erase(interest);
            fame.insert(std::make_pair(interest, fameInfo(interest, "Interest", 0, 100, std::vector<std::string>())));
            
            recognition = recog;
            entry = fame.find(recognition);
            if (entry != fame.end()) fame.erase(recognition);
            fame.insert(std::make_pair(recognition, fameInfo(recognition, "Recognition", 0, 100, std::vector<std::string>())));
            return true;
        }
        RE::TESFaction* currentFaction();
        RE::BGSLocation* currentLocation();
        RE::BGSLocation* checkLocation(RE::BGSLocation* checkLoc, bool CheckLocation = true);
        fameInfo* operator[](RE::TESGlobal* global);
        
        fameData* getFameObj(RE::TESGlobal* global, RE::TESFaction * faction, RE::BGSLocation * loc);

        region* getRegionObj(RE::TESFaction * fac, RE::BGSLocation * loc);

        fameAlias* getAliasObj(RE::TESFaction * fac);

        tolerance* getToleranceObj(RE::BGSLocation* loc, RE::TESGlobal* glob);

        void sendFameGainNotification() { 
            if (RE::Calendar::GetSingleton()->GetCurrentGameTime() < nextNotif) return;
            RE::DebugNotification("I can hear people whispering about me...");
            nextNotif += 0.044;
        }
        bool isReady() {
            
            return interest && recognition;
        }
        bool setActive(bool set) {
            return isReady() ? isActive = set : isActive = false;
        };
        RE::BSEventNotifyControl ProcessEvent(const RE::TESActiveEffectApplyRemoveEvent* evt,
            RE::BSTEventSource<RE::TESActiveEffectApplyRemoveEvent>*) {
            if (!evt) return RE::BSEventNotifyControl::kContinue;
            if (!evt->target) return RE::BSEventNotifyControl::kContinue;

           
            
            RE::EffectSetting* mef =
                RE::TESDataHandler::GetSingleton()->LookupForm<RE::EffectSetting>(0x081E, "Gossip.esp");
            RE::Actor* aActor = evt->target.get()->As<RE::Actor>();
            if (!aActor) return RE::BSEventNotifyControl::kContinue;
            RE::MagicTarget* magicTarget = aActor->GetMagicTarget(); 
            if (!magicTarget) {
                return RE::BSEventNotifyControl::kContinue;
            }
            auto effectList = magicTarget->GetActiveEffectList();
            
            for (auto effect : *effectList) {
                if (effect->usUniqueID != evt->activeEffectUniqueID) continue;
                std::string add = evt->isApplied ? "Removing" : "Adding";
               
                if (effect && effect->GetBaseObject()->GetFormID() == RE::TESDataHandler::GetSingleton()->LookupFormID(0x081E, "Gossip.esp")) {
                    if (evt->isApplied) {
                        logger::debug("{} looked away from the player", evt->target->GetBaseObject()->GetName());
                        playerSightLostEvent.SendEvent(evt->target.get());
                        return RE::BSEventNotifyControl::kContinue;
                    }
                    logger::debug("{}({}) saw the player", evt->target->GetBaseObject()->GetName(),
                                  evt->activeEffectUniqueID);
                    playerSightEvent.SendEvent(evt->target.get());
                    return RE::BSEventNotifyControl::kContinue;
                }
                
                return RE::BSEventNotifyControl::kContinue;
            }
            
            // logger::debug("spell active");

            return RE::BSEventNotifyControl::kContinue;
        }
        RE::BGSLocation* oldLoc = nullptr; 
        RE::BSEventNotifyControl ProcessEvent(const RE::BGSActorCellEvent* event,
            RE::BSTEventSource<RE::BGSActorCellEvent>*) {
            if (event && event->actor == RE::PlayerCharacter::GetSingleton()->GetHandle()) {
                
                
                if (stl::to_underlying(event->flags.get()) & stl::to_underlying(RE::BGSActorCellEvent::CellFlag::kLeave)) {
                    logger::debug("Leaving({}) cell {}", stl::to_underlying(event->flags.get()), RE::TESForm::LookupByID(event->cellID)->GetFormEditorID());
                    oldLoc = currentLocation(); 
                    if (oldLoc) 
                    {
                        region* reg = getRegionObj(nullptr, oldLoc);
                        if (reg) reg->timeSinceVisit = RE::Calendar::GetSingleton()->GetCurrentGameTime();
                    }
                } else {
                    logger::debug("Entering({}) cell {}", stl::to_underlying(event->flags.get()),
                                  RE::TESForm::LookupByID(event->cellID)->GetFormEditorID());
                    RE::BGSLocation* akLoc =
                        checkLocation(RE::TESForm::LookupByID<RE::TESObjectCELL>(event->cellID)->GetLocation());
                    if (akLoc) 
                    {
                        region* regObj = getRegionObj(nullptr, akLoc);
                        if (regObj) {
                            regObj->syncLocation();
                            logger::debug("Syncing fame globals to {}", regObj->tLoc->GetFormEditorID());
                        }
                        logger::debug("Sending new region");
                        playerRegionChange.SendEvent(akLoc, oldLoc);
                    }
                    
                }
                
            }
            return RE::BSEventNotifyControl::kContinue;
        }


    }extern o_gossip;
    
    void onGameSaved(SKSE::SerializationInterface* evt);
    void onGameLoad(SKSE::SerializationInterface* evt);
    void onRevert(SKSE::SerializationInterface* evt);
}