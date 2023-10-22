#pragma once

#include <Gossip.h>

namespace gossip {
    bool getActive(RE::StaticFunctionTag*);
    bool setActive(RE::StaticFunctionTag*);
    bool setInactive(RE::StaticFunctionTag*);

    float getVersion(RE::StaticFunctionTag*);
    std::string getVersionString(RE::StaticFunctionTag*);

    bool newFameAlias(RE::StaticFunctionTag*, RE::TESFaction* Alias, std::string aliasName);
    void newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc);
    void newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, std::vector<std::string> tags, int min,
                        int max, bool force);
    RE::TESGlobal* getFameGlobal(RE::StaticFunctionTag*, std::string globalName);

    void addFameTag(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, std::string valueTag);
    bool removeFameTag(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, std::string valueTag);
    std::vector<std::string> getValueTags(RE::StaticFunctionTag*, RE::TESGlobal* valueGlobal);
    std::string getFameName(RE::StaticFunctionTag*, RE::TESGlobal* global);
    std::vector<RE::TESGlobal*> getAllFameGlobals(RE::StaticFunctionTag*);
    std::vector<std::string> getAllFameNames(RE::StaticFunctionTag*);
    std::vector<std::string> getAllLocationNames(RE::StaticFunctionTag*);
    std::vector<RE::BGSLocation*> getAllLocations(RE::StaticFunctionTag*);

    int getGossipValue(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, RE::BGSLocation* fameLoc, RE::TESFaction* fameAlias);
    int setGossipValue(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, int amt, bool returnOldValue,
                       RE::BGSLocation* fameLoc,
                       RE::TESFaction* fameAlias);  
    int removeGossipValue(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, int amt, bool returnOldValue,
                          RE::BGSLocation* fameLoc,
                          RE::TESFaction* fameAlias);  
    int addGossipValue(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, int amt, bool returnOldValue,
                       RE::BGSLocation* fameLoc,
                       RE::TESFaction* fameAlias);
    int modGossipValue(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, int amt, bool returnOldValue,
                       RE::BGSLocation* fameLoc, RE::TESFaction* fameAlias);
    void setGossipValueMin(RE::StaticFunctionTag*, RE::TESGlobal* fameGlobal, int amt, RE::TESFaction* actorAlias, RE::BGSLocation* akLoc);
    void setGossipValueMax(RE::StaticFunctionTag*, RE::TESGlobal* fameGlobal, int amt, RE::TESFaction* actorAlias, RE::BGSLocation* akLoc);


    int getFameValue(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, RE::BGSLocation* fameLoc, RE::TESFaction* fameAlias);  
    int setFameValue(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, int amt, bool returnOldValue,
                     RE::BGSLocation* fameLoc,
                     RE::TESFaction* fameAlias);
    int removeFameValue(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, int amt, bool returnOldValue,
                        RE::BGSLocation* fameLoc,
                        RE::TESFaction* fameAlias);   
    int addFameValue(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, int amt, bool returnOldValue,
                     RE::BGSLocation* fameLoc,
                     RE::TESFaction* fameAlias);
    int modFameValue(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, int amt, bool returnOldValue,
                       RE::BGSLocation* fameLoc, RE::TESFaction* fameAlias);
    void setFameValueMin(RE::StaticFunctionTag*, RE::TESGlobal* fameGlobal, int amt, RE::TESFaction* actorAlias,
                         RE::BGSLocation* akLoc);
    void setFameValueMax(RE::StaticFunctionTag*, RE::TESGlobal* fameGlobal, int amt, RE::TESFaction* actorAlias,
                         RE::BGSLocation* akLoc);
    //std::vector<std::string> getAllLocationNames(RE::StaticFunctionTag*);
    int setInterest(RE::StaticFunctionTag*, int amt, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc);
    int addInterest(RE::StaticFunctionTag*, int amt, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc);
    int removeInterest(RE::StaticFunctionTag*, int amt, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc);
    int getInterest(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc, RE::TESFaction* actorAlias);
    int modInterest(RE::StaticFunctionTag*, int amt, RE::TESFaction* actorAlias, RE::BGSLocation* fameLoc);
    void setInterestMin(RE::StaticFunctionTag*, int amt, RE::BGSLocation* akLoc, RE::TESFaction* actorAlias);
    void setInterestMax(RE::StaticFunctionTag*, int amt, RE::BGSLocation* akLoc, RE::TESFaction* actorAlias);

    int setTolerance(RE::StaticFunctionTag*, RE::TESGlobal* fameGlobal, int amt, RE::BGSLocation* fameLoc, RE::TESFaction* alias);
    int getTolerance(RE::StaticFunctionTag*, RE::TESGlobal* fameGlobal, RE::BGSLocation* fameLoc, RE::TESFaction* alias);
    int addTolerance(RE::StaticFunctionTag*, RE::TESGlobal* fameGlobal, int amt, RE::BGSLocation* fameLoc, RE::TESFaction* alias);
    int removeTolerance(RE::StaticFunctionTag*, RE::TESGlobal* fameGlobal, int amt, RE::BGSLocation* fameLoc, RE::TESFaction* alias);
    int modTolerance(RE::StaticFunctionTag*, RE::TESGlobal* fameGlobal, int amt, RE::BGSLocation* fameLoc, RE::TESFaction* alias);
    void setToleranceMin(RE::StaticFunctionTag*, RE::TESGlobal* glob, int amt, RE::BGSLocation* akLoc);
    void setToleranceMax(RE::StaticFunctionTag*, RE::TESGlobal* glob, int amt, RE::BGSLocation* akLoc);

    bool setCurrentRegion(RE::StaticFunctionTag*, RE::BGSLocation* fameLoc);
    bool setCurrentAlias(RE::StaticFunctionTag*, RE::TESFaction* actorAlias);
    bool isTrackedLocation(RE::StaticFunctionTag*, RE::BGSLocation* akLoc, bool checkParent);
    RE::BGSLocation* resolveTrackedLocation(RE::StaticFunctionTag*, RE::BGSLocation* akLoc);
    bool papyrusRegister(RE::BSScript::IVirtualMachine* Registry);
}  // namespace gossip