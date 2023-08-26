#pragma once

#include <RE/Skyrim.h>
#include <SKSE\Interfaces.h>
#include <Region.h>
#include <Fame.h>
#include <FameAlias.h>
#include <FameOrigin.h>
using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::stl;


void writeString(SKSE::SerializationInterface* evt, std::string string) {
    std::size_t size = string.length() + 1;
    evt->WriteRecordData(size);
    evt->WriteRecordData(string.data(), static_cast<std::uint32_t>(size));
}
std::string readString(SKSE::SerializationInterface* evt) {
    std::string name;
    std::size_t strings;
    evt->ReadRecordData(strings);
    name.reserve(strings);
    evt->ReadRecordData(name.data(), static_cast<std::uint32_t>(strings));
    return name;
}
RE::TESForm* readForm(SKSE::SerializationInterface* evt){
    RE::FormID oldForm;
    RE::FormID newForm = 0;
    evt->ReadRecordData(oldForm);
    evt->ResolveFormID(oldForm, newForm);
    logger::info("{:x},{:x}", oldForm, newForm);
    return RE::TESForm::LookupByID(newForm);
};
namespace gossip {
    class Gossip {
        
    public:
        std::vector<RE::BGSLocation*> trackedLocations;
        std::map<RE::TESGlobal*, fameInfo> fame;
        fameAlias* currentFameAlias = nullptr;
        region* currentRegion = nullptr;
        std::map<RE::TESForm*, fameAlias> Alias{};
        std::map<RE::BGSLocation*, int> regionTolerance{};
        
        
        
        static Gossip* getSingleton() {
            static Gossip* container;
            if (!container) container = new Gossip;
            return container;
        }
        static bool newFameAlias(RE::StaticFunctionTag*, RE::TESForm* Alias, std::string aliasName);
        static bool newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, int min, int max,
                            std::vector<std::string> tags);
        static RE::TESGlobal* getFameGlobal(RE::StaticFunctionTag*, std::string globalName);
        static std::string getFameName(RE::StaticFunctionTag*, RE::TESGlobal* global);
        static std::vector<RE::TESGlobal*> getAllFameGlobals(RE::StaticFunctionTag*);
        static std::vector<std::string> getAllFameNames(RE::StaticFunctionTag*);

        static void setFameMin(RE::StaticFunctionTag*, RE::TESGlobal* global, int amt, RE::BGSLocation* newLoc);
        static void setFameMax(RE::StaticFunctionTag*, RE::TESGlobal* global, int amt, RE::BGSLocation* newLoc);

        static int setFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,
                    int amt);
        static int addFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal,
                    int amt);
        static int removeFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                       RE::TESGlobal* fameGlobal, int amt);
        static int getFame(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal);


        static bool newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc, std::string locName);
        static std::vector<RE::BGSLocation*> getAllLocations(RE::StaticFunctionTag*);
        static std::vector<std::string> getAllLocationNames(RE::StaticFunctionTag*);

        
        static int setGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                             RE::TESGlobal* fameGlobal, int amt);
        static int addGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                             RE::TESGlobal* fameGlobal, int amt);
        static int removeGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                                RE::TESGlobal* fameGlobal, int amt);
        static int getGossip(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                             RE::TESGlobal* fameGlobal);

        static int setInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, int amt);
        static int addInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, int amt);
        static int removeInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, int amt);
        static int getInterest(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc);
        
        static int setTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                                RE::TESGlobal* fameGlobal, int amt);
        static int getTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                                RE::TESGlobal* fameGlobal);
        static int addTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                                RE::TESGlobal* fameGlobal, int amt);
        static int removeTolerance(RE::StaticFunctionTag*, RE::BGSKeyword* alias, RE::BGSLocation* fameLoc,
                                   RE::TESGlobal* fameGlobal, int amt);

        static bool papyrusRegister(RE::BSScript::IVirtualMachine* Registry) {
            Registry->RegisterFunction("newFame", "GIP_SKSE", newFame);
            return true;
        }
        static void onGameSaved(SKSE::SerializationInterface* evt);
        static void onGameLoad(SKSE::SerializationInterface* evt);
        static void onRevert(SKSE::SerializationInterface* evt);

    private:
    };

}