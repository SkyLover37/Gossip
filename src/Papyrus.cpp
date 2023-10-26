
#include <Papyrus.h>

namespace gossip {
    bool getActive(RE::StaticFunctionTag *) { 
        return o_gossip.active; 
    }
    bool setActive(RE::StaticFunctionTag *) { 
        o_gossip.setState(true);
        return o_gossip.active;
    }
    bool setInactive(RE::StaticFunctionTag *) {
        o_gossip.setState(false);
        return o_gossip.active;
    }
    float getVersion(RE::StaticFunctionTag *) { 
        auto plugin = PluginDeclaration::GetSingleton();
        return plugin->GetVersion().major() + (plugin->GetVersion().minor()/10); }
    std::string getVersionString(RE::StaticFunctionTag *) { return PluginDeclaration::GetSingleton()->GetVersion().string(); }

    bool gossip::newFameAlias(RE::StaticFunctionTag *, RE::TESFaction *Alias, std::string aliasName) {
        Gossip& gossip = Gossip::getSingleton();
        if (gossip.profile.aliasMap.contains(Alias)) {
            return false;
        }
        else {
            gossip.profile.aliasMap.insert(std::make_pair(Alias, fameAlias(aliasName, Alias)));
            return true;
        }
        return false;
    }

    void gossip::newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc) {
        Gossip& gossip = Gossip::getSingleton();
        if (gossip.checkLocation(newLoc) == nullptr) {
            gossip.trackedLocations.push_back(newLoc);
        }
    }

    void gossip::newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, std::vector<std::string> tags, int min,
        int max, bool force) {
        Gossip& gossip = Gossip::getSingleton();
        if(!gossip.fame.contains(global) || force){
            gossip[global] = fameInfo(global, fameName, min, max, tags);
        }
    }

    RE::TESGlobal *gossip::getFameGlobal(RE::StaticFunctionTag *, std::string globalName) { 
        Gossip& gossip = Gossip::getSingleton();
        for(auto& var : gossip.fame) {
            if (var.second.getName() == globalName) return var.first;
        }
        return nullptr;

    }

    void addFameTag(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, std::string valueTag) {
    
        Gossip& gossip = Gossip::getSingleton();
        auto & tags = gossip[valueKey].tags;
        //If tag exists, exit.
        if (std::find(tags.begin(), tags.end(), valueTag) == tags.end()) return;
        gossip[valueKey].tags.push_back(valueTag);

    }

    bool removeFameTag(RE::StaticFunctionTag *, RE::TESGlobal* valueKey, std::string valueTag) { 
        Gossip&gossip = Gossip::getSingleton();
        auto &tags = gossip[valueKey].tags;
        auto requestedTag = std::find(tags.begin(), tags.end(), valueTag);
        if (requestedTag == tags.end()) return false;
        tags.erase(requestedTag);
        return true;

    }

    std::vector<std::string> getValueTags(RE::StaticFunctionTag *, RE::TESGlobal *valueGlobal) {
        return Gossip::getSingleton()[valueGlobal].tags;
    }


    std::string gossip::getFameName(RE::StaticFunctionTag *, RE::TESGlobal *global) { 
        Gossip& gossip = Gossip::getSingleton();
        return gossip[global].name;
    }

    std::vector<RE::TESGlobal *> gossip::getAllFameGlobals(RE::StaticFunctionTag *) {
        Gossip&gossip = Gossip::getSingleton();
        std::vector<RE::TESGlobal *> globals;

        //globals.resize(gossip.fame.size());
        for (auto &var : gossip.fame) {
            globals.push_back(var.first);
        }
        return globals;
    }

    std::vector<std::string> gossip::getAllFameNames(RE::StaticFunctionTag *) {
        Gossip&gossip = Gossip::getSingleton();
        std::vector<std::string> names{};
        log::debug("getting all fame names: {}", gossip.fame.size());
        //names.resize(gossip.fame.size());
        for (auto &var : gossip.fame) {
            log::debug("Fame name: {}", var.second.name);
            names.push_back(var.second.name);
        }

        return names;
    }

    std::vector<std::string> getAllLocationNames(RE::StaticFunctionTag *) { 
        std::vector<std::string> names;
        names.reserve(o_gossip.trackedLocations.size());
        for(RE::BGSLocation* var : o_gossip.trackedLocations) {
            names.push_back(var->GetFullName());
        }
        return names;
    }

    std::vector<RE::BGSLocation *> gossip::getAllLocations(RE::StaticFunctionTag *) {
        return Gossip::getSingleton().trackedLocations;
    }
   
    bool setCurrentAlias(RE::StaticFunctionTag *, RE::TESFaction *actorAlias) {
        if (!Gossip::getSingleton().profile.aliasMap.contains(actorAlias)) return false;
        Gossip::getSingleton().profile.activeAlias = &Gossip::getSingleton().profile[actorAlias];
        return true;
    }
    bool isTrackedLocation(RE::StaticFunctionTag *, RE::BGSLocation *akLoc, bool checkParent) { 
        return Gossip::getSingleton().checkLocation(akLoc, checkParent);
    }
    RE::BGSLocation *getCurrentTrackedLocation(RE::StaticFunctionTag *) { return o_gossip.checkLocation(RE::PlayerCharacter::GetSingleton()->GetCurrentLocation()); }
    RE::BGSLocation *resolveTrackedLocation(RE::StaticFunctionTag *, RE::BGSLocation *akLoc) { 
        RE::BGSLocation *rVal = o_gossip.checkLocation(akLoc);
        if (rVal != nullptr) return rVal;
        return nullptr;
    }
    namespace fame {
        bool switchFameLimit(bool local, RE::TESGlobal *Glob, RE::BGSLocation *Loc, RE::TESFaction *Fac) {
            if (!Loc) Loc = o_gossip.currentLocation();
            if (!Fac) Fac = o_gossip.currentFaction();

            auto fameObj = o_gossip.getFameObj(Glob, Fac, Loc);
            if (local) {
                fameObj->limit = &fameObj->localBound;
            } else {
                fameObj->limit = fameObj->info->getLimit();
            }
            if (*(fameObj->limit) == fameLimit::limit_type::local && local) return true;
            if (*fameObj->limit == fameLimit::limit_type::regional && !local) return true;
            return false;
        }
        bool isLimitLocal(RE::TESGlobal* Glob, RE::BGSLocation *akLoc, RE::TESFaction *akAlias) { 
            if (!akLoc) akLoc = o_gossip.currentLocation();
            if (!akAlias) akAlias = o_gossip.currentFaction();
                
            return o_gossip.getFameObj(Glob, akAlias, akLoc)->localBound == fameLimit::limit_type::local;
        }
        int getFameValue(RE::TESGlobal *glob, RE::BGSLocation *akLoc, RE::TESFaction *akAlias) { 
            if (!akLoc) akLoc = o_gossip.currentLocation();
            if (!akAlias) akAlias = o_gossip.currentFaction();

            return *o_gossip.getFameObj(glob, akAlias, akLoc);
        }
        int setFameValue(RE::TESGlobal *glob, int amt, RE::BGSLocation *akLoc, RE::TESFaction *akAlias, bool returnOldVal) {
            if (!akLoc) akLoc = o_gossip.currentLocation();
            if (!akAlias) akAlias = o_gossip.currentFaction();

            
            auto fameObj = o_gossip.getFameObj(glob, akAlias, akLoc);
            int oldVal = *fameObj;

            *fameObj = amt;
            return returnOldVal ? oldVal : *fameObj;
        }
        int modFameValue(RE::TESGlobal *glob, int amt, RE::BGSLocation *akLoc, RE::TESFaction *akAlias, bool returnOldVal) {
            if (!akLoc) akLoc = o_gossip.currentLocation();
            if (!akAlias) akAlias = o_gossip.currentFaction();

            auto fameObj = o_gossip.getFameObj(glob, akAlias, akLoc);
            int oldVal = *fameObj;

            *fameObj += amt;
            return returnOldVal ? oldVal : *fameObj;
        }
        void setFameLimits(RE::TESGlobal *Glob, int min, int max, RE::BGSLocation* akLoc, RE::TESFaction* akFac) {
            if (!akLoc) akLoc = o_gossip.currentLocation();
            if (!akFac) akFac = o_gossip.currentFaction();
            
            auto fameObj = o_gossip.getFameObj(Glob, akFac, akLoc);
            


        }
        int getFameMin(RE::TESGlobal *Glob) { return 0; }
        int getFameMax(RE::TESGlobal *Glob) { return 0; }
    }  // namespace fame

    namespace gossipVal {
        int getGossipValue(RE::TESGlobal *glob, RE::BGSLocation *akLoc, RE::TESFaction *akAlias) { return 0; }
        int setGossipValue(RE::TESGlobal *glob, RE::BGSLocation *akLoc, RE::TESFaction *akAlias, bool returnOldVal) {
            return 0;
        }
        int modGossipValue(RE::TESGlobal *glob, RE::BGSLocation *akLoc, RE::TESFaction *akAlias, bool returnOldVal) {
            return 0;
        }
    }  // namespace gossipVal
    namespace tolerance {
        int getToleranceValue(RE::TESGlobal *glob, RE::BGSLocation *akLoc, RE::TESFaction *akAlias) { return 0; }
        int setToleranceValue(RE::TESGlobal *glob, int amt, RE::BGSLocation *akLoc, RE::TESFaction *akAlias,
                              bool returnOldVal) {
            return 0;
        }
        int modToleranceValue(RE::TESGlobal *glob, int amt, RE::BGSLocation *akLoc, RE::TESFaction *akAlias,
                              bool returnOldVal) {
            return 0;
        }
        int setToleranceLimits(RE::TESGlobal *Glob, int min, int max, bool local) { return 0; }
        int getToleranceMin(RE::TESGlobal *Glob) { return 0; }
        int getToleranceMax(RE::TESGlobal *Glob) { return 0; }
    }  // namespace tolerance
    namespace ninterest {
        int getInterestValue(RE::BGSLocation *akLoc, RE::TESFaction *akAlias) { return 0; }
        int setInterestValue(int amt, RE::BGSLocation *akLoc, RE::TESFaction *akAlias, bool returnOldVal) { return 0; }
        int modInterestValue(int amt, RE::BGSLocation *akLoc, RE::TESFaction *akAlias, bool returnOldVal) { return 0; }
        void setInterestLimits(int min, int max, bool local) {}
        int getInterestMin() { return 0; }
        int getInterestMax() { return 0; }
    }  // namespace interest
    namespace recognition {
        int getRecognitionValue(RE::BGSLocation *akLoc, RE::TESFaction *akAlias) { return 0; }
        int setRecognitionValue(int amt, RE::BGSLocation *akLoc, RE::TESFaction *akAlias, bool returnOldVal) {
            return 0;
        }
        int modRecognitionValue(int amt, RE::BGSLocation *akLoc, RE::TESFaction *akAlias, bool returnOldVal) {
            return 0;
        }
        void setRecognitionLimits(int min, int max, bool local) {}
        int getRecognitionMin() { return 0; }
        int getRecognitionMax() { return 0; }
    }  // namespace recognition

    bool papyrusRegister(RE::BSScript::IVirtualMachine *Registry) {
        Registry->RegisterFunction("newFame", "GIP_SKSE", newFame);
        Registry->RegisterFunction("newFameAlias", "GIP_SKSE", newFameAlias);
        Registry->RegisterFunction("newLocation", "GIP_SKSE", newLocation);
        Registry->RegisterFunction("getFameGlobal", "GIP_SKSE", getFameGlobal);
        Registry->RegisterFunction("getFameTags", "GIP_SKSE", getValueTags);
        Registry->RegisterFunction("getFameName", "GIP_SKSE", getFameName);
        Registry->RegisterFunction("getAllFameGlobals", "GIP_SKSE", getAllFameGlobals);
        Registry->RegisterFunction("getAllFameNames", "GIP_SKSE", getAllFameNames);
        Registry->RegisterFunction("getAllLocations", "GIP_SKSE", getAllLocations);
        Registry->RegisterFunction("getFameGlobal", "GIP_SKSE", getFameGlobal);
        //Registry->RegisterFunction("setCurrentRegion", "GIP_SKSE", setCurrentRegion);
        Registry->RegisterFunction("getCurrentTrackedLocation", "GIP_SKSE", getCurrentTrackedLocation);
        Registry->RegisterFunction("setCurrentAlias", "GIP_SKSE", setCurrentAlias);
        Registry->RegisterFunction("addFameTag", "GIP_SKSE", addFameTag);
        Registry->RegisterFunction("removeFameTag", "GIP_SKSE", removeFameTag);
        Registry->RegisterFunction("isTrackedLocation", "GIP_SKSE", isTrackedLocation);
        Registry->RegisterFunction("resolveTrackedLocation", "GIP_SKSE", resolveTrackedLocation);
        Registry->RegisterFunction("getAllLocationNames", "GIP_SKSE", getAllLocationNames);
        Registry->RegisterFunction("setActive", "GIP_SKSE", setActive);
        Registry->RegisterFunction("setInactive", "GIP_SKSE", setInactive);
        Registry->RegisterFunction("getActive", "GIP_SKSE", getActive);
        Registry->RegisterFunction("getVersion", "GIP_SKSE", getVersion);
        Registry->RegisterFunction("getVersionString", "GIP_SKSE", getVersionString);
        return true;
    }
}  // namespace gossip

