#pragma once

#include <Gossip.h>

namespace gossip {

    bool isReady();
    bool setup(RE::StaticFunctionTag*, RE::TESGlobal* interest, RE::TESGlobal* recognition);

    bool getActive(RE::StaticFunctionTag*);
    bool setActive(RE::StaticFunctionTag*);
    bool setInactive(RE::StaticFunctionTag*);

    float getVersion(RE::StaticFunctionTag*);
    std::string getVersionString(RE::StaticFunctionTag*);

    bool newFameAlias(RE::StaticFunctionTag*, RE::TESFaction* Alias, std::string aliasName);
    bool setCurrentAlias(RE::StaticFunctionTag*, RE::TESFaction* actorAlias);

    void newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc);
    void newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName,
                 int min, int max, bool force);
    void newFameWithTags(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, std::vector<std::string> tags, int min,
                        int max, bool force);
    void addFameTag(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, std::string valueTag);
    bool removeFameTag(RE::StaticFunctionTag*, RE::TESGlobal* valueKey, std::string valueTag);

    std::vector<std::string> getValueTags(RE::StaticFunctionTag*, RE::TESGlobal* valueGlobal);
    RE::TESGlobal* getFameGlobal(RE::StaticFunctionTag*, std::string globalName);
    std::string getFameName(RE::StaticFunctionTag*, RE::TESGlobal* global);
    std::vector<RE::TESGlobal*> getAllFameGlobals(RE::StaticFunctionTag*);
    std::vector<std::string> getAllFameNames(RE::StaticFunctionTag*);
    
    std::vector<std::string> getAllLocationNames(RE::StaticFunctionTag*);
    std::vector<RE::BGSLocation*> getAllLocations(RE::StaticFunctionTag*);

    bool isTrackedLocation(RE::StaticFunctionTag*, RE::BGSLocation* akLoc, bool checkParent);
    RE::BGSLocation* getCurrentLocation(RE::StaticFunctionTag*);
    RE::BGSLocation* resolveTrackedLocation(RE::StaticFunctionTag*, RE::BGSLocation* akLoc);
    namespace fame 
    {
        bool useGlobalFameLimit(RE::StaticFunctionTag* , RE::TESGlobal* Glob, RE::BGSLocation* akLoc,
                                RE::TESFaction* akFac);
        bool useLocalFameLimit(RE::StaticFunctionTag*, RE::TESGlobal* Glob, RE::BGSLocation* Loc, RE::TESFaction* Fac);
        bool isLimitLocal(RE::StaticFunctionTag*, RE::TESGlobal* Glob, RE::BGSLocation* akLoc, RE::TESFaction* akAlias);

        int getFameValue(RE::StaticFunctionTag*, RE::TESGlobal* glob, RE::BGSLocation* akLoc, RE::TESFaction* akAlias);
        int setFameValue(RE::StaticFunctionTag* ,RE::TESGlobal* glob, int amt, RE::BGSLocation* akLoc,
                         RE::TESFaction* akAlias, bool returnOldVal);
        int modFameValue(RE::StaticFunctionTag* , RE::TESGlobal* glob, int amt, RE::BGSLocation* akLoc,
                         RE::TESFaction* akAlias, bool returnOldVal);
        
        void setFameLimits(RE::StaticFunctionTag*, RE::TESGlobal* Glob, int min, int max);
        int getFameMin(RE::StaticFunctionTag*, RE::TESGlobal* Glob);
        int getFameMax(RE::StaticFunctionTag*, RE::TESGlobal* Glob);

        void setLocalFameLimits(RE::StaticFunctionTag*, RE::TESGlobal* Glob, int min, int max, RE::BGSLocation* akLoc,
                                RE::TESFaction* akAlias);
        int getLocalFameMin(RE::StaticFunctionTag*, RE::TESGlobal* Glob, RE::BGSLocation* akLoc,
                            RE::TESFaction* akAlias);
        int getLocalFameMax(RE::StaticFunctionTag*, RE::TESGlobal* Glob, RE::BGSLocation* akLoc,
                            RE::TESFaction* akAlias);
    }  // namespace fame
    namespace gossipVal {
        int getGossipValue(RE::StaticFunctionTag*, RE::TESGlobal* glob, RE::BGSLocation* akLoc,
                           RE::TESFaction* akAlias);
        int setGossipValue(RE::StaticFunctionTag*, RE::TESGlobal* glob, RE::BGSLocation* akLoc, RE::TESFaction* akAlias,
                           bool returnOldVal = false);
        int modGossipValue(RE::StaticFunctionTag*, RE::TESGlobal* glob, RE::BGSLocation* akLoc, RE::TESFaction* akAlias,
                           bool returnOldVal = false);
    }  // namespace gossipVal
    namespace ntolerance {
        int getToleranceValue(RE::StaticFunctionTag*, RE::TESGlobal* glob, RE::BGSLocation* akLoc);
        int setToleranceValue(RE::StaticFunctionTag*, RE::TESGlobal* glob, int amt, RE::BGSLocation* akLoc,
                              bool returnOldVal = false);
        int modToleranceValue(RE::StaticFunctionTag*, RE::TESGlobal* glob, int amt, RE::BGSLocation* akLoc,
                              bool returnOldVal = false);
        
        void setToleranceLimits(RE::StaticFunctionTag*, RE::TESGlobal* Glob, int min, int max, RE::BGSLocation* akLoc);
        int getToleranceMin(RE::StaticFunctionTag*, RE::TESGlobal* Glob, RE::BGSLocation* akLoc);
        int getToleranceMax(RE::StaticFunctionTag*, RE::TESGlobal* Glob, RE::BGSLocation* akLoc);
    }
    namespace ninterest {
        int getInterestValue(RE::StaticFunctionTag*, RE::BGSLocation* akLoc, RE::TESFaction* akAlias);
        int setInterestValue(RE::StaticFunctionTag*, int amt, RE::BGSLocation* akLoc, RE::TESFaction* akAlias,
                             bool returnOldVal = false);
        int modInterestValue(RE::StaticFunctionTag*, int amt, RE::BGSLocation* akLoc, RE::TESFaction* akAlias,
                             bool returnOldVal = false);
        
        void setInterestLimits(RE::StaticFunctionTag*, int min, int max, RE::TESFaction* akAlias);
        int getInterestMin(RE::StaticFunctionTag*, RE::TESFaction* akAlias);
        int getInterestMax(RE::StaticFunctionTag*, RE::TESFaction* akAlias);
    }  // namespace interest
    


    //bool setCurrentRegion(RE::StaticFunctionTag*, RE::BGSRE::BGSLocation** fameLoc);
   
    bool papyrusRegister(RE::BSScript::IVirtualMachine* Registry);
}  // namespace gossip