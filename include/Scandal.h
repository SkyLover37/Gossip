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
        static bool newFameAlias(RE::StaticFunctionTag*, RE::TESFaction* Alias, std::string aliasName);
        static bool newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc, std::string locName);
        static bool newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, int min, int max,
                            std::vector<std::string> tags);
        static RE::TESGlobal* getFameGlobal(RE::StaticFunctionTag*, std::string globalName);
        static std::string getFameName(RE::StaticFunctionTag*, RE::TESGlobal* global);
        static std::vector<RE::TESGlobal*> getAllFameGlobals(RE::StaticFunctionTag*);
        static std::vector<std::string> getAllFameNames(RE::StaticFunctionTag*);


        static std::vector<RE::BGSLocation*> getAllLocations(RE::StaticFunctionTag*);
        std::vector<std::string> getAllLocationNames(RE::StaticFunctionTag*);

        
        
        
        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    private:
    };

}