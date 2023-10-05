#pragma once

#include <FameAlias.h>


using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::stl;



namespace gossip {
    class Gossip {
        
    public:
        std::string version = "0.0.0";
        bool active = false;
        std::vector<RE::BGSLocation*> trackedLocations;
        std::map<RE::TESGlobal*, fameInfo> fame;
        fameAlias* currentFameAlias{};
        region* currentRegion{};
        std::map<RE::TESFaction*, fameAlias> Alias{};
        std::map<RE::BGSLocation*, valueData> regionTolerance{};
        std::map<RE::TESObjectREFR*, fameProfile> npcProfile;
        
        static Gossip* getSingleton() {
            //static Gossip* container;
            //if (!container) container = new Gossip;
            return &o_gossip;
        }
        
        RE::BGSLocation* checkLocation(RE::BGSLocation* checkLoc, bool CheckLocation = true);
        valueData* getInterest();
        valueData* getInterest(RE::BGSLocation* akLoc, RE::TESFaction* alias);
        valueData* getInterest(RE::BGSLocation* akLoc);
        valueData* getValueObj(valueType valueType, RE::TESGlobal* global);
        valueData* getValueObj(valueType valueType, RE::BGSLocation* loc, RE::TESGlobal* global);
        valueData* getValueObj(valueType valueType, RE::TESFaction* fac, RE::BGSLocation* Loc, RE::TESGlobal* global);
        
        bool setState(bool active);

        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    private:
    } static o_gossip;

}