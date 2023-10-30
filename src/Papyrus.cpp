
#include <Papyrus.h>

namespace gossip {
    bool isReady() { return o_gossip.isReady(); }
    bool setup(RE::StaticFunctionTag *, RE::TESGlobal *interest, RE::TESGlobal *recognition) { return o_gossip.setup(interest, recognition); }
    bool getActive(RE::StaticFunctionTag *) { 
        return o_gossip.isActive; 
    }
    bool setActive(RE::StaticFunctionTag *) { 
        logger::debug("setting gossip to active {}", o_gossip.setActive(true));
        return o_gossip.isActive;
    }
    bool setInactive(RE::StaticFunctionTag *) {
        o_gossip.setActive(false);
        return o_gossip.isActive;
    }
    float getVersion(RE::StaticFunctionTag *) { 
        auto plugin = PluginDeclaration::GetSingleton();
        return plugin->GetVersion().major() + (plugin->GetVersion().minor()/10.0f); }
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
        if (!newLoc) return;
        Gossip& gossip = Gossip::getSingleton();
        //check if location is already tracked
        if (gossip.checkLocation(newLoc) == nullptr) {
            logger::info("New location: {}", newLoc->GetFormEditorID());
            gossip.trackedLocations.push_back(newLoc);
            //gossip.profile.regionMap.insert(std::make_pair(newLoc, ))
        }
    }

    void newFame(RE::StaticFunctionTag *, RE::TESGlobal *global, std::string fameName, int min, int max, bool force) {
            if (!global) return;
            Gossip &gossip = Gossip::getSingleton();
            if (!gossip.fame.contains(global) || force) {

                gossip.fame.insert(std::make_pair(global, fameInfo(global, fameName, min, max, std::vector<std::string>())));
            
            }
    }

    void gossip::newFameWithTags(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, std::vector<std::string> tags, int min,
        int max, bool force) {
            if (!global) return;
        Gossip& gossip = Gossip::getSingleton();
        if(!gossip.fame.contains(global) || force){
            gossip.fame.insert(std::make_pair(global,  fameInfo(global, fameName, min, max, tags)));
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
        auto & tags = gossip[valueKey]->tags;
        //If tag exists, exit.
        if (std::find(tags.begin(), tags.end(), valueTag) == tags.end()) return;
        gossip[valueKey]->tags.push_back(valueTag);

    }

    bool removeFameTag(RE::StaticFunctionTag *, RE::TESGlobal* valueKey, std::string valueTag) { 
        Gossip&gossip = Gossip::getSingleton();
        auto &tags = gossip[valueKey]->tags;
        auto requestedTag = std::find(tags.begin(), tags.end(), valueTag);
        if (requestedTag == tags.end()) return false;
        tags.erase(requestedTag);
        return true;

    }

    std::vector<std::string> getValueTags(RE::StaticFunctionTag *, RE::TESGlobal *valueGlobal) {
        return Gossip::getSingleton()[valueGlobal]->tags;
    }




    std::string gossip::getFameName(RE::StaticFunctionTag *, RE::TESGlobal *global) { 
        Gossip& gossip = Gossip::getSingleton();
        return gossip[global]->name;
    }

    std::vector<RE::TESGlobal *> gossip::getAllFameGlobals(RE::StaticFunctionTag *) {
        Gossip& gossip = Gossip::getSingleton();
        logger::debug("getAllFameGlobals()");
        std::vector<RE::TESGlobal *> globals{};

        globals.reserve(gossip.fame.size());
        for (auto &var : gossip.fame) {
            logger::debug("Adding {} to array.", var.first ? var.first->GetFormEditorID() : "none");
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
        bool useGlobalFameLimit(RE::StaticFunctionTag *, RE::TESGlobal *Glob, RE::BGSLocation *akLoc,
                                RE::TESFaction *akFac) {
            if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return false;
            }
            auto fameObj = o_gossip.getFameObj(Glob, akFac, akLoc);
            if (!fameObj) return false;
            fameObj->limit = fameObj->info->getLimit();

            return *fameObj->limit == fameLimit::limit_type::regional;
        }
        bool useLocalFameLimit(RE::StaticFunctionTag *, RE::TESGlobal *Glob, RE::BGSLocation *Loc,
                               RE::TESFaction *Fac) {
            if (!Loc) {
                Loc = o_gossip.currentLocation();
                if (!Loc) return false;
            }

            auto fameObj = o_gossip.getFameObj(Glob, Fac, Loc);
            if (!fameObj) return false;
            fameObj->limit = &fameObj->localBound;
            logger::debug("localBound: {} {}", fameObj->localBound.min(), fameObj->localBound.max());
            logger::debug("limit: ", fameObj->limit->min(), fameObj->limit->max());
            return *(fameObj->limit) == fameLimit::limit_type::local;
        }
        bool isLimitLocal(RE::StaticFunctionTag *, RE::TESGlobal *Glob, RE::BGSLocation *akLoc,
                          RE::TESFaction *akAlias) { 
            
             if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return false;
            }
             auto fameObj = o_gossip.getFameObj(Glob, akAlias, akLoc);
            if (!fameObj) return false;
            return fameObj->limit->type == fameLimit::limit_type::local;
        }
        int getFameValue(RE::StaticFunctionTag *, RE::TESGlobal *glob, RE::BGSLocation *akLoc,
                         RE::TESFaction *akAlias) { 
            if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
            logger::debug("Getting Fame in {} {} {}", glob->GetName(), akLoc->GetName(),
                          akAlias ? akAlias->GetName() : "none");
            auto fameObj = o_gossip.getFameObj(glob, akAlias, akLoc);
            return fameObj ? *fameObj : -1;
        }
        int setFameValue(RE::StaticFunctionTag *, RE::TESGlobal *glob, int amt, RE::BGSLocation *akLoc,
                         RE::TESFaction *akAlias, bool returnOldVal) {
            if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
            
            auto fameObj = o_gossip.getFameObj(glob, akAlias, akLoc);
            if (!fameObj) return -1;
            int oldVal = *fameObj;

            fameObj->set(amt);
            return returnOldVal ? oldVal : *fameObj;
        }
        int modFameValue(RE::StaticFunctionTag *, RE::TESGlobal *glob, int amt, RE::BGSLocation *akLoc,
                         RE::TESFaction *akAlias, bool returnOldVal) {
            
            if (!akLoc) 
            {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
                
            
            logger::debug("Modding {}'s {} Fame in {}", akAlias ? akAlias->GetName() : "Player" , glob->GetFormEditorID(),
                          akLoc->GetName());
            auto fameObj = o_gossip.getFameObj(glob, akAlias, akLoc);
            if (!fameObj) return -1;
            int oldVal = *fameObj;

            fameObj->mod(amt);
            return returnOldVal ? oldVal : *fameObj;
        }
        void setFameLimits(RE::StaticFunctionTag *, RE::TESGlobal *Glob, int min, int max) {
            
            auto fameObj = o_gossip[Glob]->getLimit();
            fameObj->setLimits(min, max);


        }
        int getFameMin(RE::StaticFunctionTag *, RE::TESGlobal *Glob) {
            return o_gossip[Glob] ? o_gossip[Glob]->getLimit()->min() : -1;
        }
        int getFameMax(RE::StaticFunctionTag *, RE::TESGlobal *Glob) {
            return o_gossip[Glob] ? o_gossip[Glob]->getLimit()->max() : -1;
        }

        void setLocalFameLimits(RE::StaticFunctionTag *, RE::TESGlobal *Glob, int min, int max, RE::BGSLocation *akLoc,
                                RE::TESFaction *akAlias) {
            if (!akLoc) akLoc = o_gossip.currentLocation();
            if (!akAlias) akAlias = o_gossip.currentFaction();
            logger::debug("setting local limits to {} {}", min, max);
            auto fameObj = o_gossip.getFameObj(Glob, akAlias, akLoc);
            fameObj->localBound.setLimits(min, max);

        }
        int getLocalFameMin(RE::StaticFunctionTag *, RE::TESGlobal *Glob, RE::BGSLocation *akLoc,
                            RE::TESFaction *akAlias) { 
            
             if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
             
            return o_gossip.getFameObj(Glob, akAlias, akLoc)->localBound.min(); 
        }
        int getLocalFameMax(RE::StaticFunctionTag *, RE::TESGlobal *Glob, RE::BGSLocation *akLoc,
                            RE::TESFaction *akAlias) { 
            if (!akLoc) akLoc = o_gossip.currentLocation();
            if (!akAlias) akAlias = o_gossip.currentFaction();
             
            return o_gossip.getFameObj(Glob, akAlias, akLoc)->localBound.max();
             
        }
        void papyrusRegister(RE::BSScript::IVirtualMachine *Registry) {
            std::string script = "GIP_SKSE";
            Registry->RegisterFunction("useGlobalFameLimit", script, useGlobalFameLimit);
            Registry->RegisterFunction("useLocalFameLimit", script, useLocalFameLimit);
            Registry->RegisterFunction("isLimitLocal", script, isLimitLocal);
            Registry->RegisterFunction("getFameValue", script, getFameValue);
            Registry->RegisterFunction("setFameValue", script, setFameValue);
            Registry->RegisterFunction("modFameValue", script, modFameValue);
            Registry->RegisterFunction("setFameLimit", script, setFameLimits);
            Registry->RegisterFunction("getFameMin", script, getFameMin);
            Registry->RegisterFunction("getFameMax", script, getFameMax);
            Registry->RegisterFunction("setLocalFameLimit", script, setLocalFameLimits);
            Registry->RegisterFunction("getLocalFameMin", script, getLocalFameMin);
            Registry->RegisterFunction("getLocalFameMax", script, getLocalFameMax);
           
            
        }
    }  // namespace fame
    
    namespace gossipVal {
        int getGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *glob, RE::BGSLocation *akLoc,
                           RE::TESFaction *akAlias) {
            if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
            
            return 0;
        }
        int setGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *glob, RE::BGSLocation *akLoc,
                           RE::TESFaction *akAlias, bool returnOldVal) {
            if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
            
            return 0;
        }
        int modGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *glob, RE::BGSLocation *akLoc,
                           RE::TESFaction *akAlias, bool returnOldVal) {
            if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
            
            return 0;
        }
        void papyrusRegister(RE::BSScript::IVirtualMachine *Registry) {
            std::string script = "GIP_SKSE";
            Registry->RegisterFunction("getGossipValue", script, getGossipValue);
            Registry->RegisterFunction("setGossipValue", script, setGossipValue);
            Registry->RegisterFunction("modGossipValue", script, modGossipValue);
        }
    }  // namespace gossipVal
    namespace ntolerance {
        int getToleranceValue(RE::StaticFunctionTag *, RE::TESGlobal *glob, RE::BGSLocation *akLoc) {
            if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
            auto tol = o_gossip.getToleranceObj(akLoc, glob);
            if (!tol) return -1;
            return *tol;
        }
        int setToleranceValue(RE::StaticFunctionTag *, RE::TESGlobal *glob, int amt, RE::BGSLocation *akLoc,
                              bool returnOldVal) {
            if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
            auto tol = o_gossip.getToleranceObj(akLoc, glob);
            if (!tol) return -1;
            int oldVal = *tol;
            tol->set(amt);
            return returnOldVal ? oldVal : static_cast<int>(*tol);
        }
        int modToleranceValue(RE::StaticFunctionTag *, RE::TESGlobal *glob, int amt, RE::BGSLocation *akLoc,
                              bool returnOldVal) {
            if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
            auto tol = o_gossip.getToleranceObj(akLoc, glob);
            if (!tol) return -1;
            int oldVal = *tol;
            tol->mod(amt);
            return returnOldVal ? oldVal : static_cast<int>(*tol);
        }
        void setToleranceLimits(RE::StaticFunctionTag *, RE::TESGlobal *Glob, int min, int max, RE::BGSLocation* akLoc) { 
            
             if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return;
            }
             auto tol = o_gossip.getToleranceObj(akLoc, Glob);
            if (!tol) return;
             tol->operator()(min, max);
        }
        int getToleranceMin(RE::StaticFunctionTag *, RE::TESGlobal *Glob, RE::BGSLocation* akLoc) { 
            
             if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
             auto tol = o_gossip.getToleranceObj(akLoc, Glob);
             if (!tol) return -1;
             return tol->min();
        }
        int getToleranceMax(RE::StaticFunctionTag *, RE::TESGlobal *Glob, RE::BGSLocation* akLoc) { 
            
             if (!akLoc) {
                akLoc = o_gossip.currentLocation();
                if (!akLoc) return -1;
            }
             auto tol = o_gossip.getToleranceObj(akLoc, Glob);
             if (!tol) return -1;
             return tol->max();
        }
        void papyrusRegister(RE::BSScript::IVirtualMachine *Registry) {
            std::string script = "GIP_SKSE";
            Registry->RegisterFunction("getToleranceValue", script, getToleranceValue);
            Registry->RegisterFunction("setToleranceValue", script, setToleranceValue);
            Registry->RegisterFunction("modToleranceValue", script, modToleranceValue);
            Registry->RegisterFunction("setToleranceLimits", script, setToleranceLimits);
            Registry->RegisterFunction("getToleranceMin", script, getToleranceMin);
            Registry->RegisterFunction("getToleranceMax", script, getToleranceMax);
        }
    }

    bool papyrusRegister(RE::BSScript::IVirtualMachine *Registry) {
        std::string script = "GIP_SKSE";
        Registry->RegisterFunction("setup", script, setup);
        Registry->RegisterFunction("newFame", "GIP_SKSE", newFame);
        Registry->RegisterFunction("newFameWithTags", script, newFameWithTags);
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
        Registry->RegisterFunction("isActive", "GIP_SKSE", getActive);
        Registry->RegisterFunction("getVersion", "GIP_SKSE", getVersion);
        Registry->RegisterFunction("getVersionString", "GIP_SKSE", getVersionString);
        fame::papyrusRegister(Registry);
        gossipVal::papyrusRegister(Registry);
        ntolerance::papyrusRegister(Registry);

        
        return true;
    }
}  // namespace gossip

