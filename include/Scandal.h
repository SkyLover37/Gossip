#pragma once

#include <RE/Skyrim.h>
using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

template<typename T, typename Z>
using map = std::unordered_map<struct T, struct Z>;
namespace gossip {
    class Gossip {
        
    public:
        
        struct fameInfo {
            std::string name = "";
            RE::TESGlobal* fameGlobal = nullptr;
            int max = 100;
            int min = 0;
        };
        struct fameData {
            int fameValue;
            int tolerance;
            int max = 100;
            int min = 0;
        };
       
        struct fameProfile {
            RE::TESObjectREFR* akActor;
            float viewingTime;
            int interest;
            int relation;
        };
        struct locationInfo {
            std::string name = "";
            RE::BGSLocation* location = nullptr;
        };

        struct fameAlias {
            std::string name = "";
            RE::FormType type;
            RE::TESForm* formAlias;
            map<RE::BGSLocation*, map<fameInfo, fameData>> trackedLocations;
        };
        map<RE::TESGlobal*, fameInfo> fame;
        map<RE::TESForm*, fameAlias> alias;
        
        static Gossip* getSingleton() {
            static Gossip* container;
            if (!container) container = new Gossip;
            return container;
        }

        bool newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, RE::BSString fameName);
        bool newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc, RE::BSString locName);
        bool newFameAlias(RE::StaticFunctionTag*, RE::TESForm* fameAlias, RE::BSString aliasName);

        int setFame(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt, RE::BGSKeyword* alias);
        int addFame(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt, bool buffer, RE::BGSKeyword* alias);
        int removeFame(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt, bool buffer, RE::BGSKeyword* alias);
        int modFame(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt, RE::BGSKeyword* alias);
        int getFame(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, RE::BGSKeyword* alias);

        int setInterest(RE::StaticFunctionTag*, int amt);
        int addInterest(RE::StaticFunctionTag*, int amt);
        int removeInterest(RE::StaticFunctionTag*, int amt);
        int modInterest(RE::StaticFunctionTag*, int amt);
        int getInterest(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, RE::BGSKeyword* alias);
        
        int setTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, RE::BGSKeyword* alias, int amt);
        int getTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, RE::BGSKeyword* alias);
        int addTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, RE::BGSKeyword* alias, int amt);
        int removeTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,
                            RE::BGSKeyword* alias, int amt);
        int modTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,
                         RE::BGSKeyword* alias, int amt);


        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    private:
    };

}