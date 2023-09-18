#pragma once

#include <RE/Skyrim.h>
#include <SKSE\Interfaces.h>
#include <Region.h>
#include <Fame.h>
#include <FameAlias.h>
#include <FameOrigin.h>
#include <Papyrus.h>
using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::stl;



namespace gossip {
    class Gossip {
        
    public:
        std::vector<RE::BGSLocation*> trackedLocations;
        std::map<RE::TESGlobal*, fameInfo> fame;
        fameAlias* currentFameAlias = nullptr;
        region* currentRegion = nullptr;
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