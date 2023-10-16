
#include <FameAlias.h>
namespace gossip {
    class Gossip {
    public:
        
        std::string version = "0.0.0";
        bool active = false;
        std::vector<RE::BGSLocation*> trackedLocations;
        infoMap fame;
        std::map<RE::TESFaction*, fameAlias> Alias{};
        std::map<RE::BGSLocation*, valueData<long long>> regionTolerance{};
        std::map<RE::TESObjectREFR*, fameProfile> profiles;

        static Gossip* getSingleton() {
            // static Gossip* container;
            // if (!container) container = new Gossip;
            return &o_gossip;
        }

        RE::BGSLocation* checkLocation(RE::BGSLocation* checkLoc, bool CheckLocation = true);
        
        valueData* getValueObj(RE::TESGlobal* global);
        valueData* getValueObj(RE::BGSLocation* loc, RE::TESGlobal* global);
        valueData* getValueObj(RE::TESFaction* fac, RE::BGSLocation* Loc, RE::TESGlobal* global);

        bool setState(bool active);

        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    private:
    } static o_gossip;

}