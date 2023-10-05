
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
        Gossip* gossip = Gossip::getSingleton();
        if (gossip->Alias.contains(Alias)) {
            return false;
        }
        else {
            gossip->Alias[Alias] = fameAlias(aliasName, Alias);
            return true;
        }
        return false;
    }

    void gossip::newLocation(RE::StaticFunctionTag*, RE::BGSLocation* newLoc) {
        Gossip* gossip = Gossip::getSingleton();
        if (gossip->checkLocation(newLoc) == nullptr) {
            gossip->trackedLocations.push_back(newLoc);
        }
    }

    void gossip::newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, std::vector<std::string> tags, int min,
        int max, bool force) {
        Gossip* gossip = Gossip::getSingleton();
        if(!gossip->fame.contains(global) || force){
            gossip->fame[global] = fameInfo(global, fameName, min, max, tags);
        }
    }

    RE::TESGlobal *gossip::getFameGlobal(RE::StaticFunctionTag *, std::string globalName) { 
        Gossip *gossip = Gossip::getSingleton();
        for(auto& var : gossip->fame) {
            if (var.second.name == globalName) return var.first;
        }
        return nullptr;

    }

    void addFameTag(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, std::string valueTag) {
    
        Gossip *gossip = Gossip::getSingleton();
        auto & tags = gossip->fame[valueKey].tags;
        //If tag exists, exit.
        if (std::find(tags.begin(), tags.end(), valueTag) == tags.end()) return;
        gossip->fame[valueKey].tags.push_back(valueTag);

    }

    bool removeFameTag(RE::StaticFunctionTag *, RE::TESGlobal* valueKey, std::string valueTag) { 
        Gossip *gossip = Gossip::getSingleton();
        auto &tags = gossip->fame[valueKey].tags;
        auto requestedTag = std::find(tags.begin(), tags.end(), valueTag);
        if (requestedTag == tags.end()) return false;
        tags.erase(requestedTag);
        return true;

    }

    std::vector<std::string> getValueTags(RE::StaticFunctionTag *, RE::TESGlobal *valueGlobal) {
        return Gossip::getSingleton()->fame[valueGlobal].tags;
    }


    std::string gossip::getFameName(RE::StaticFunctionTag *, RE::TESGlobal *global) { 
        Gossip *gossip = Gossip::getSingleton();
        return gossip->fame[global].name;
    }

    std::vector<RE::TESGlobal *> gossip::getAllFameGlobals(RE::StaticFunctionTag *) {
        Gossip *gossip = Gossip::getSingleton();
        std::vector<RE::TESGlobal *> globals;

        //globals.resize(gossip->fame.size());
        for (auto &var : gossip->fame) {
            globals.push_back(var.first);
        }
        return globals;
    }

    std::vector<std::string> gossip::getAllFameNames(RE::StaticFunctionTag *) {
        Gossip *gossip = Gossip::getSingleton();
        std::vector<std::string> names{};
        log::debug("getting all fame names: {}", gossip->fame.size());
        //names.resize(gossip->fame.size());
        for (auto &var : gossip->fame) {
            log::debug("Fame name: {}", var.second.name);
            names.push_back(var.second.name);
        }

        return names;
    }

    std::vector<std::string> getAllLocationNames(RE::StaticFunctionTag *) { 
        std::vector<std::string> names;
        names.resize(o_gossip.trackedLocations.size());
        for(RE::BGSLocation* var : o_gossip.trackedLocations) {
            names.push_back(var->GetFullName());
        }
        return names;
    }

    std::vector<RE::BGSLocation *> gossip::getAllLocations(RE::StaticFunctionTag *) {
        return Gossip::getSingleton()->trackedLocations;
    }

    int getGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, RE::BGSLocation *fameLoc, RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias* alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        return alias->known[fameLoc].fame[valueKey][gossip::valueType::gossip].getValue();

    }

    int setGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, bool returnOldValue,
                       RE::BGSLocation *fameLoc,
                       RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        int oldVal = alias->known[fameLoc].fame[valueKey][gossip::valueType::gossip].getValue();
        int sendBack;
        sendBack = alias->known[fameLoc].fame[valueKey][gossip::valueType::gossip].setValue(amt);
        if (returnOldValue) sendBack = oldVal;
        return sendBack;
        
    }

    int removeGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, bool returnOldValue,
                          RE::BGSLocation *fameLoc,
                          RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        int oldVal = alias->known[fameLoc].fame[valueKey][gossip::valueType::gossip].getValue();
        int sendBack;
        sendBack = alias->known[fameLoc].fame[valueKey][gossip::valueType::gossip].removeValue(amt);
        if (returnOldValue) sendBack = oldVal;
        return sendBack;
    }

    int addGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, bool returnOldValue,
                       RE::BGSLocation *fameLoc,
                       RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        int oldVal = alias->known[fameLoc].fame[valueKey][gossip::valueType::gossip].getValue();
        int sendBack;
        sendBack = alias->known[fameLoc].fame[valueKey][gossip::valueType::gossip].addValue(amt);
        if (returnOldValue) sendBack = oldVal;
        return sendBack;
    }

    int modGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, bool returnOldValue,
                       RE::BGSLocation *fameLoc, RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        int oldVal = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].getValue();
        int sendBack;
        sendBack = alias->known[fameLoc].fame[valueKey][gossip::valueType::gossip].modValue(amt);
        if (returnOldValue) sendBack = oldVal;
        return sendBack;
    }

    void setGossipValueMin(RE::StaticFunctionTag *, RE::TESGlobal *fameGlobal, int amt, RE::TESFaction *actorAlias,
                          RE::BGSLocation *akLoc) {
        region *region = o_gossip.currentRegion;
        fameAlias *alias = o_gossip.currentFameAlias;
        if (actorAlias != nullptr) alias = &o_gossip.Alias[actorAlias];
        if (akLoc != nullptr) region = &alias->known[akLoc];
        region->fame[fameGlobal][valueType::gossip].setValueMin(amt);
    }

    void setGossipValueMax(RE::StaticFunctionTag *, RE::TESGlobal *fameGlobal, int amt, RE::TESFaction *actorAlias,
                          RE::BGSLocation *akLoc) {
        region *region = o_gossip.currentRegion;
        fameAlias *alias = o_gossip.currentFameAlias;
        if (actorAlias != nullptr) alias = &o_gossip.Alias[actorAlias];
        if (akLoc != nullptr) region = &alias->known[akLoc];
        region->fame[fameGlobal][valueType::gossip].setValueMax(amt);
    }

    int getFameValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, RE::BGSLocation *fameLoc, RE::TESFaction *fameAlias) {

        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        return alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].getValue();
    }

    int setFameValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, bool returnOldValue,
                     RE::BGSLocation *fameLoc,
                     RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        int oldVal = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].getValue();
        int sendBack;
        sendBack = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].setValue(amt);
        if (returnOldValue) sendBack = oldVal;
        return sendBack;
    }

    int removeFameValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, bool returnOldValue,
                        RE::BGSLocation *fameLoc,
                        RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        int oldVal = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].getValue();
        int sendBack;
        sendBack = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].removeValue(amt);
        if (returnOldValue) sendBack = oldVal;
        return sendBack;
    }

    int addFameValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, bool returnOldValue,
                     RE::BGSLocation *fameLoc,
                     RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        int oldVal = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].getValue();
        int sendBack;
        sendBack = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].addValue(amt);
        if (returnOldValue) sendBack = oldVal;
        return sendBack;
    }

    int modFameValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, bool returnOldValue,
                     RE::BGSLocation *fameLoc, RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        int oldVal = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].getValue();
        int sendBack;
        sendBack = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].modValue(amt);
        if (returnOldValue) sendBack = oldVal;
        return sendBack;
    }

    void setFameValueMin(RE::StaticFunctionTag *, RE::TESGlobal *fameGlobal, int amt, RE::TESFaction *actorAlias,
                        RE::BGSLocation *akLoc) {
        region *region = o_gossip.currentRegion;
        fameAlias *alias = o_gossip.currentFameAlias;
        if (actorAlias != nullptr) alias = &o_gossip.Alias[actorAlias];
        if (akLoc != nullptr) region = &alias->known[akLoc];
        region->fame[fameGlobal][valueType::fame].setValueMin(amt);
    }

    void setFameValueMax(RE::StaticFunctionTag *, RE::TESGlobal *fameGlobal, int amt, RE::TESFaction *actorAlias,
                        RE::BGSLocation *akLoc) {
        region *region = o_gossip.currentRegion;
        fameAlias *alias = o_gossip.currentFameAlias;
        if (actorAlias != nullptr) alias = &o_gossip.Alias[actorAlias];
        if (akLoc != nullptr) region = &alias->known[akLoc];
        region->fame[fameGlobal][valueType::fame].setValueMax(amt);
    }

    //std::vector<std::string> gossip::getAllLocationNames(RE::StaticFunctionTag *) { return std::vector<std::string>(); }
    
    //Interest
    int gossip::setInterest(RE::StaticFunctionTag*, int amt, RE::TESFaction* actorAlias, RE::BGSLocation *fameLoc) { 
        Gossip *gossip = Gossip::getSingleton();
        valueData *data = gossip->Alias[actorAlias].known[fameLoc].getInterest();
        
        return data->addValue(amt);
    }

    int gossip::addInterest(RE::StaticFunctionTag *, int amt, RE::TESFaction *actorAlias, RE::BGSLocation *fameLoc) { 
    
        Gossip *gossip = Gossip::getSingleton();
        valueData* interest = gossip->Alias[actorAlias].known[fameLoc].getInterest();
        return interest->addValue(amt);

    }

    int gossip::removeInterest(RE::StaticFunctionTag *, int amt, RE::TESFaction *actorAlias, RE::BGSLocation *fameLoc) {
        Gossip *gossip = Gossip::getSingleton();
        valueData *data = gossip->Alias[actorAlias].getRegion(fameLoc)->getInterest();
        data->removeValue(amt);
        return data->getValue();
    }
    
    int gossip::getInterest(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESFaction *actorAlias) {
        return Gossip::getSingleton()->Alias[actorAlias].getRegion(fameLoc)->getInterest()->getValue();
    }

    int modInterest(RE::StaticFunctionTag *, int amt, RE::TESFaction *actorAlias, RE::BGSLocation *fameLoc) {
        return Gossip::getSingleton()->Alias[actorAlias].getRegion(fameLoc)->getInterest()->modValue(amt);
    }

    void setInterestMin(RE::StaticFunctionTag *, int amt, RE::BGSLocation *akLoc, RE::TESFaction *actorAlias) {
        if (actorAlias == nullptr) actorAlias = o_gossip.currentFameAlias->form->As<RE::TESFaction>();
        if (akLoc == nullptr) akLoc = o_gossip.currentRegion->form->As<RE::BGSLocation>();
        o_gossip.getInterest(akLoc, actorAlias)->setValueMin(amt);
    }

    void setInterestMax(RE::StaticFunctionTag *, int amt, RE::BGSLocation *akLoc, RE::TESFaction *actorAlias) { 
        
        if (actorAlias == nullptr) actorAlias = o_gossip.currentFameAlias->form->As<RE::TESFaction>();
        if (akLoc == nullptr) akLoc = o_gossip.currentRegion->form->As<RE::BGSLocation>();
        o_gossip.getInterest(akLoc, actorAlias)->setValueMax(amt);
    }

    



    //Tolerance
    int gossip::setTolerance(RE::StaticFunctionTag*,RE::TESGlobal *fameGlobal, int amt, RE::BGSLocation *fameLoc, RE::TESFaction* alias) { 

        Gossip *gossip = Gossip::getSingleton();
        gossip->regionTolerance[fameLoc].setValue(amt);
        return gossip->regionTolerance[fameLoc].getValue();
    }

    int gossip::getTolerance(RE::StaticFunctionTag *, RE::TESGlobal *fameGlobal, RE::BGSLocation *fameLoc, RE::TESFaction* alias) { 
        
        Gossip *gossip = Gossip::getSingleton();
        return gossip->regionTolerance[fameLoc].getValue();
    
    }


    int gossip::addTolerance(RE::StaticFunctionTag *, RE::TESGlobal *fameGlobal, int amt, RE::BGSLocation *fameLoc, RE::TESFaction* alias) {
        
        Gossip *gossip = Gossip::getSingleton();
        gossip->regionTolerance[fameLoc].addValue(amt);
        return gossip->regionTolerance[fameLoc].getValue();

    }

    int gossip::removeTolerance(RE::StaticFunctionTag *, RE::TESGlobal *fameGlobal, int amt, RE::BGSLocation *fameLoc, RE::TESFaction* alias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip->regionTolerance[fameLoc].removeValue(amt);
        return gossip->regionTolerance[fameLoc].getValue();
    }
    int modTolerance(RE::StaticFunctionTag *, RE::TESGlobal *fameGlobal, int amt, RE::BGSLocation *fameLoc, RE::TESFaction* alias) { return Gossip::getSingleton()->regionTolerance[fameLoc].modValue(amt);
    }
    void setToleranceMin(RE::StaticFunctionTag *, RE::TESGlobal *glob, int amt, RE::BGSLocation* akLoc) {
        if (akLoc == nullptr) akLoc = o_gossip.currentRegion->form->As<RE::BGSLocation>();
        o_gossip.regionTolerance[akLoc].setValueMin(amt);
    }
    void setToleranceMax(RE::StaticFunctionTag *, RE::TESGlobal *glob, int amt, RE::BGSLocation *akLoc) { 
    if (akLoc == nullptr) akLoc = o_gossip.currentRegion->form->As<RE::BGSLocation>();
        o_gossip.regionTolerance[akLoc].setValueMax(amt);
    }
    bool setCurrentRegion(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc) {
        if (Gossip::getSingleton()->currentFameAlias == nullptr) {
            logger::error("There is no current alias");
            return false;
        }
        if (Gossip::getSingleton()->checkLocation(fameLoc) == nullptr) {
            
        }
        //if the region is not in the map, create the object.
        if (!Gossip::getSingleton()->currentFameAlias->known.contains(fameLoc)) {
            Gossip::getSingleton()->currentFameAlias->known[fameLoc] = region();
        }
        Gossip::getSingleton()->currentRegion = &Gossip::getSingleton()->currentFameAlias->known[fameLoc];
        return true;
    }
    bool setCurrentAlias(RE::StaticFunctionTag *, RE::TESFaction *actorAlias) {
        if (!Gossip::getSingleton()->Alias.contains(actorAlias)) return false;
        Gossip::getSingleton()->currentFameAlias = &Gossip::getSingleton()->Alias[actorAlias];
        return true;
    }
    bool isTrackedLocation(RE::StaticFunctionTag *, RE::BGSLocation *akLoc, bool checkParent) { 
        return Gossip::getSingleton()->checkLocation(akLoc, checkParent);
    }
    RE::BGSLocation *resolveTrackedLocation(RE::StaticFunctionTag *, RE::BGSLocation *akLoc) { 
        RE::BGSLocation *rVal = o_gossip.checkLocation(akLoc);
        if (rVal != nullptr) return rVal;
        return nullptr;
    }
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
        Registry->RegisterFunction("getGossipValue", "GIP_SKSE", getGossipValue);
        Registry->RegisterFunction("setGossipValue", "GIP_SKSE", setGossipValue);
        Registry->RegisterFunction("modGossipValue", "GIP_SKSE", modGossipValue);
        Registry->RegisterFunction("removeGossipValue", "GIP_SKSE", removeGossipValue);
        Registry->RegisterFunction("addGossipValue", "GIP_SKSE", addGossipValue);
        //Registry->RegisterFunction("getFameGlobal", "GIP_SKSE", getFameGlobal);
        Registry->RegisterFunction("setFameValue", "GIP_SKSE", setFameValue);
        Registry->RegisterFunction("removeFameValue", "GIP_SKSE", removeFameValue);
        Registry->RegisterFunction("modFameValue", "GIP_SKSE", modFameValue);
        Registry->RegisterFunction("addFameValue", "GIP_SKSE", addFameValue);
        Registry->RegisterFunction("setInterest", "GIP_SKSE", setInterest);
        Registry->RegisterFunction("addInterest", "GIP_SKSE", addInterest);
        Registry->RegisterFunction("removeInterest", "GIP_SKSE", removeInterest);
        Registry->RegisterFunction("getInterest", "GIP_SKSE", getInterest);
        Registry->RegisterFunction("modInterest", "GIP_SKSE", getInterest);
        Registry->RegisterFunction("setTolerance", "GIP_SKSE", setTolerance);
        Registry->RegisterFunction("getTolerance", "GIP_SKSE", getTolerance);
        Registry->RegisterFunction("modTolerance", "GIP_SKSE", modTolerance);
        Registry->RegisterFunction("removeTolerance", "GIP_SKSE", removeTolerance);
        Registry->RegisterFunction("addTolerance", "GIP_SKSE", addTolerance);
        Registry->RegisterFunction("setCurrentRegion", "GIP_SKSE", setCurrentRegion);
        Registry->RegisterFunction("setCurrentAlias", "GIP_SKSE", setCurrentAlias);
        Registry->RegisterFunction("addFameTag", "GIP_SKSE", addFameTag);
        Registry->RegisterFunction("removeFameTag", "GIP_SKSE", removeFameTag);
        Registry->RegisterFunction("isTrackedLocation", "GIP_SKSE", isTrackedLocation);
        Registry->RegisterFunction("resolveTrackedLocation", "GIP_SKSE", resolveTrackedLocation);
        Registry->RegisterFunction("setGossipValueMax", "GIP_SKSE", setGossipValueMax);
        Registry->RegisterFunction("setGossipValueMin", "GIP_SKSE", setGossipValueMin);
        Registry->RegisterFunction("setFameValueMax", "GIP_SKSE", setFameValueMax);
        Registry->RegisterFunction("setFameValueMin", "GIP_SKSE", setFameValueMin);
        Registry->RegisterFunction("setInterestMax", "GIP_SKSE", setInterestMax);
        Registry->RegisterFunction("setInterestMin", "GIP_SKSE", setInterestMin);
        Registry->RegisterFunction("setToleranceMax", "GIP_SKSE", setToleranceMax);
        Registry->RegisterFunction("setToleranceMin", "GIP_SKSE", setToleranceMin);
        Registry->RegisterFunction("getAllLocationNames", "GIP_SKSE", getAllLocationNames);
        Registry->RegisterFunction("setActive", "GIP_SKSE", setActive);
        Registry->RegisterFunction("setInactive", "GIP_SKSE", setInactive);
        Registry->RegisterFunction("getActive", "GIP_SKSE", getActive);
        Registry->RegisterFunction("getVersion", "GIP_SKSE", getVersion);
        Registry->RegisterFunction("getVersionString", "GIP_SKSE", getVersionString);
        return true;
    }
}  // namespace gossip