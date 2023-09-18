#pragma once

#include <FameAlias.h>


using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::stl;



namespace gossip {
    class Gossip {
        
    public:
        std::vector<RE::BGSLocation*> trackedLocations;
        std::map<RE::TESGlobal*, fameInfo> fame;
        fameAlias* currentFameAlias;
        region* currentRegion;
        std::map<RE::TESFaction*, fameAlias> Alias{};
        std::map<RE::BGSLocation*, valueData> regionTolerance{};
        std::map<RE::TESObjectREFR*, fameProfile> npcProfile;
        
        static Gossip* getSingleton() {
            static Gossip* container;
            if (!container) container = new Gossip;
            return container;
        }
    
        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    private:
    };

}