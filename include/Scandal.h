#pragma once

#include <RE/Skyrim.h>
#include <SKSE\Interfaces.h>
using namespace RE::BSScript;
using namespace SKSE;
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
            int gossip;
            int tolerance = 0;
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
            map<RE::BGSLocation*, map<RE::TESGlobal*, fameData>> trackedLocations;
        };
        RE::TESForm* currentAlias;
        map<RE::TESGlobal*, fameInfo> fame;
        map<RE::TESForm*, fameAlias> alias;
        map<RE::BGSLocation*, locationInfo> locations;
        static Gossip* getSingleton() {
            static Gossip* container;
            if (!container) container = new Gossip;
            return container;
        }
        bool newFameAlias(RE::StaticFunctionTag*, RE::TESForm* fameAlias, RE::BSString aliasName);
        bool newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, RE::BSString fameName);
        RE::TESGlobal* getFameGlobal(RE::StaticFunctionTag*, RE::BSString globalName);
        RE::BSString getFameName(RE::StaticFunctionTag*, RE::TESGlobal* global);
        std::vector<RE::TESGlobal*> getAllFameGlobals(RE::StaticFunctionTag*);
        std::vector<RE::BSString> getAllFameNames(RE::StaticFunctionTag*);

        void setFameMin(RE::StaticFunctionTag*, RE::TESGlobal* global, int amt, RE::BGSLocation* newLoc);
        void setFameMax(RE::StaticFunctionTag*, RE::TESGlobal* global, int amt, RE::BGSLocation* newLoc);

        int setFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,
                    int amt);
        int addFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,
                    int amt);
        int removeFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                       RE::TESGlobal* fameGlobal, int amt);
        int getFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal);


        bool newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc, RE::BSString locName);
        std::vector<RE::BGSLocation*> getAllLocations(RE::StaticFunctionTag*);
        std::vector<RE::BSString> getAllLocationNames(RE::StaticFunctionTag*);

        
        int setGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
        int addGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
        int removeGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
        int getGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal);

        int setInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, int amt);
        int addInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, int amt);
        int removeInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, int amt);
        int getInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc);
        
        int setTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,  int amt);
        int getTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal);
        int addTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,  int amt);
        int removeTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);


        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    private:
    };

}