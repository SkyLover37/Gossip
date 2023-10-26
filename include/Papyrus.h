#pragma once

#include <Gossip.h>

namespace gossip {
    bool getActive(RE::StaticFunctionTag*);
    bool setActive(RE::StaticFunctionTag*);
    bool setInactive(RE::StaticFunctionTag*);

    float getVersion(RE::StaticFunctionTag*);
    std::string getVersionString(RE::StaticFunctionTag*);

    bool newFameAlias(RE::StaticFunctionTag*, RE::TESFaction* Alias, std::string aliasName);
    bool setCurrentAlias(RE::StaticFunctionTag*, RE::TESFaction* actorAlias);

    void newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc);
    void newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, std::vector<std::string> tags, int min,
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
        bool switchFameLimit(bool local, RE::TESGlobal* Glob, RE::BGSLocation* Loc, RE::TESFaction* Fac);
        bool isLimitLocal(RE::TESGlobal* Glob, RE::BGSLocation* akLoc, RE::TESFaction* akAlias);

        int getFameValue(RE::TESGlobal* glob, RE::BGSLocation* akLoc, RE::TESFaction* akAlias);
        int setFameValue(RE::TESGlobal* glob, int amt, RE::BGSLocation* akLoc, RE::TESFaction* akAlias, bool returnOldVal);
        int modFameValue(RE::TESGlobal* glob, int amt, RE::BGSLocation* akLoc, RE::TESFaction* akAlias, bool returnOldVal);
        
        void setFameLimits(RE::TESGlobal* Glob, int min, int max, RE::BGSLocation* akLoc, RE::TESFaction* akFac);
        int getFameMin(RE::TESGlobal* Glob);
        int getFameMax(RE::TESGlobal* Glob);
    }  // namespace fame
    namespace gossipVal {
        int getGossipValue(RE::TESGlobal* glob, RE::BGSLocation* akLoc, RE::TESFaction* akAlias);
        int setGossipValue(RE::TESGlobal* glob, RE::BGSLocation* akLoc, RE::TESFaction* akAlias,
                           bool returnOldVal = false);
        int modGossipValue(RE::TESGlobal* glob, RE::BGSLocation* akLoc, RE::TESFaction* akAlias,
                           bool returnOldVal = false);
    }  // namespace gossipVal
    namespace tolerance {
        int getToleranceValue(RE::TESGlobal* glob, RE::BGSLocation* akLoc, RE::TESFaction* akAlias);
        int setToleranceValue(RE::TESGlobal* glob, int amt, RE::BGSLocation* akLoc, RE::TESFaction* akAlias,
                              bool returnOldVal = false);
        int modToleranceValue(RE::TESGlobal* glob, int amt, RE::BGSLocation* akLoc, RE::TESFaction* akAlias,
                              bool returnOldVal = false);
        
        int setToleranceLimits(RE::TESGlobal* Glob, int min, int max, bool local = false);
        int getToleranceMin(RE::TESGlobal* Glob);
        int getToleranceMax(RE::TESGlobal* Glob);
    }
    namespace ninterest {
        int getInterestValue(RE::BGSLocation* akLoc, RE::TESFaction* akAlias);
        int setInterestValue(int amt, RE::BGSLocation* akLoc, RE::TESFaction* akAlias, bool returnOldVal = false);
        int modInterestValue(int amt, RE::BGSLocation* akLoc, RE::TESFaction* akAlias, bool returnOldVal = false);
        
        void setInterestLimits(int min, int max, bool local = false);
        int getInterestMin();
        int getInterestMax();
    }  // namespace interest
    namespace recognition {
        int getRecognitionValue(RE::BGSLocation* akLoc, RE::TESFaction* akAlias);
        int setRecognitionValue(int amt, RE::BGSLocation* akLoc, RE::TESFaction* akAlias, bool returnOldVal = false);
        int modRecognitionValue(int amt, RE::BGSLocation* akLoc, RE::TESFaction* akAlias, bool returnOldVal = false);
       
        void setRecognitionLimits(int min, int max, bool local = false);
        int getRecognitionMin();
        int getRecognitionMax();
    }


    //bool setCurrentRegion(RE::StaticFunctionTag*, RE::BGSRE::BGSLocation** fameLoc);
   
    bool papyrusRegister(RE::BSScript::IVirtualMachine* Registry);
}  // namespace gossip