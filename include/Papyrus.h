#pragma once

#include <Scandal.h>

namespace gossip {
    
    bool newFameAlias(RE::StaticFunctionTag*, RE::TESFaction* Alias, std::string aliasName);
    void newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc);
    void newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, int min, int max,
                        std::vector<std::string> tags, bool force);
    RE::TESGlobal* getFameGlobal(RE::StaticFunctionTag*, std::string globalName);

    void addFameTag(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, std::string valueTag);
    bool removeFameTag(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, std::string valueTag);
    std::vector<std::string> getValueTags(RE::StaticFunctionTag*, RE::TESGlobal* valueGlobal);
    std::string getFameName(RE::StaticFunctionTag*, RE::TESGlobal* global);
    std::vector<RE::TESGlobal*> getAllFameGlobals(RE::StaticFunctionTag*);
    std::vector<std::string> getAllFameNames(RE::StaticFunctionTag*);

    std::vector<RE::BGSLocation*> getAllLocations(RE::StaticFunctionTag*);

    int getGossipValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt, RE::TESFaction* fameAlias);
    int setGossipValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                       bool returnOldValue,
                       RE::TESFaction* fameAlias);  
    int removeGossipValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                          bool returnOldValue,
                          RE::TESFaction* fameAlias);  
    int addGossipValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                       bool returnOldValue,
                       RE::TESFaction* fameAlias);
    int modGossipValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                       bool returnOldValue, RE::TESFaction* fameAlias);



    int getFameValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                     RE::TESFaction* fameAlias);  
    int setFameValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                     bool returnOldValue,
                     RE::TESFaction* fameAlias);
    int removeFameValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                        bool returnOldValue,
                        RE::TESFaction* fameAlias);   
    int addFameValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                     bool returnOldValue,
                     RE::TESFaction* fameAlias);
    int modFameValue(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* valueKey, int amt,
                       bool returnOldValue, RE::TESFaction* fameAlias);
    //std::vector<std::string> getAllLocationNames(RE::StaticFunctionTag*);
    int setInterest(RE::StaticFunctionTag*, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc, int amt);
    int addInterest(RE::StaticFunctionTag*, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc, int amt);
    int removeInterest(RE::StaticFunctionTag*, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc, int amt);
    int getInterest(RE::StaticFunctionTag*, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc);

    int setTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
    int getTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal);
    int addTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
    int removeTolerance(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);

    bool setCurrentRegion(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc);
    bool setCurrentAlias(RE::StaticFunctionTag*, RE::TESFaction* actorAlias);

    bool papyrusRegister(RE::BSScript::IVirtualMachine* Registry);
}  // namespace gossip