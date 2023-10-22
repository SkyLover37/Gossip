
#include <FameAlias.h>
namespace gossip {
   
    class Gossip {
    public:
        
        std::string version = "0.0.0";
        bool active = false;
        std::vector<RE::BGSLocation*> trackedLocations;
        infoMap fame;
        std::map<RE::BGSLocation*, valueData<long long>> regionTolerance{};
        //std::map<RE::TESObjectREFR*, fameProfile> profiles;
        fameProfile profile;
        static Gossip* getSingleton() {
            // static Gossip* container;
            // if (!container) container = new Gossip;
            return &o_gossip;
        }
        Gossip();
        RE::TESFaction* currentFaction();
        RE::BGSLocation* currentLocation();
        RE::BGSLocation* checkLocation(RE::BGSLocation* checkLoc, bool CheckLocation = true);
        
        
        fameData* getFameObj(RE::TESFaction* faction, RE::BGSLocation* loc, RE::TESGlobal* global);

        bool setState(bool active);

        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    } static o_gossip;
    
}