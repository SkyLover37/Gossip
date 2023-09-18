
#include <Papyrus.h>

namespace gossip {
    bool gossip::newFameAlias(RE::StaticFunctionTag*, RE::TESFaction* Alias, std::string aliasName) {
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
        if (std::find(gossip->trackedLocations.begin(), gossip->trackedLocations.end(), newLoc) == gossip->trackedLocations.end()) {
            gossip->trackedLocations.push_back(newLoc);
        }
    }

    void gossip::newFame(RE::StaticFunctionTag*, RE::TESGlobal* global, std::string fameName, int min, int max,
        std::vector<std::string> tags, bool force) {
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

        globals.resize(gossip->fame.size());
        for (auto &var : gossip->fame) {
            globals.push_back(var.first);
        }
        return globals;
    }

    std::vector<std::string> gossip::getAllFameNames(RE::StaticFunctionTag *) {
        Gossip *gossip = Gossip::getSingleton();
        std::vector<std::string> names{};

        names.resize(gossip->fame.size());
        for (auto &var : gossip->fame) {
            names.push_back(var.second.name);
        }

        return names;
    }

    std::vector<RE::BGSLocation *> gossip::getAllLocations(RE::StaticFunctionTag *) {
        return Gossip::getSingleton()->trackedLocations;
    }

    int getGossipValue(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *valueKey, int amt,
                       RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias* alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        return alias->known[fameLoc].fame[valueKey][gossip::valueType::gossip].getValue();

    }

    int setGossipValue(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *valueKey, int amt,
                       bool returnOldValue,
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

    int removeGossipValue(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *valueKey, int amt,
                          bool returnOldValue,
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

    int addGossipValue(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *valueKey, int amt,
                       bool returnOldValue,
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

    int modGossipValue(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *valueKey, int amt,
                       bool returnOldValue, RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        int oldVal = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].getValue();
        int sendBack;
        sendBack = alias->known[fameLoc].fame[valueKey][gossip::valueType::gossip].modValue(amt);
        if (returnOldValue) sendBack = oldVal;
        return sendBack;
    }

    int getFameValue(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *valueKey, int amt,
                     RE::TESFaction *fameAlias) {

        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        return alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].getValue();
    }

    int setFameValue(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *valueKey, int amt,
                     bool returnOldValue,
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

    int removeFameValue(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *valueKey, int amt,
                        bool returnOldValue,
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

    int addFameValue(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *valueKey, int amt,
                     bool returnOldValue,
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

    int modFameValue(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *valueKey, int amt,
                     bool returnOldValue, RE::TESFaction *fameAlias) {
        Gossip *gossip = Gossip::getSingleton();
        gossip::fameAlias *alias = gossip->currentFameAlias;
        if (fameAlias != nullptr) alias = &gossip->Alias[fameAlias];
        int oldVal = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].getValue();
        int sendBack;
        sendBack = alias->known[fameLoc].fame[valueKey][gossip::valueType::fame].modValue(amt);
        if (returnOldValue) sendBack = oldVal;
        return sendBack;
    }

    //std::vector<std::string> gossip::getAllLocationNames(RE::StaticFunctionTag *) { return std::vector<std::string>(); }
    
    //Interest
    int gossip::setInterest(RE::StaticFunctionTag*, RE::TESFaction* actorAlias, RE::BGSLocation *fameLoc, int amt) { 
        Gossip *gossip = Gossip::getSingleton();
        gossip->Alias[actorAlias].setInterest(fameLoc, amt);
        return gossip->Alias[actorAlias].getInterest(fameLoc);
    }

    int gossip::addInterest(RE::StaticFunctionTag *, RE::TESFaction *actorAlias, RE::BGSLocation *fameLoc, int amt) { 
    
        Gossip *gossip = Gossip::getSingleton();
        gossip->Alias[actorAlias].addInterest(fameLoc, amt);
        return gossip->Alias[actorAlias].getInterest(fameLoc);

    }

    int gossip::removeInterest(RE::StaticFunctionTag *, RE::TESFaction *actorAlias, RE::BGSLocation *fameLoc, int amt) {
        Gossip *gossip = Gossip::getSingleton();
        gossip->Alias[actorAlias].removeInterest(fameLoc, amt);
        return gossip->Alias[actorAlias].getInterest(fameLoc);
    }

    int gossip::getInterest(RE::StaticFunctionTag *, RE::TESFaction* actorAlias,RE::BGSLocation *fameLoc) { return Gossip::getSingleton()->Alias[actorAlias].getInterest(fameLoc); }




    //Tolerance
    int gossip::setTolerance(RE::StaticFunctionTag*,RE::BGSLocation *fameLoc, RE::TESGlobal *fameGlobal, int amt) { 

        Gossip *gossip = Gossip::getSingleton();
        gossip->regionTolerance[fameLoc].setValue(amt);
        return gossip->regionTolerance[fameLoc].getValue();
    }

    int gossip::getTolerance(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *fameGlobal) { 
        
        Gossip *gossip = Gossip::getSingleton();
        return gossip->regionTolerance[fameLoc].getValue();
    
    }


    int gossip::addTolerance(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *fameGlobal, int amt) {
        
        Gossip *gossip = Gossip::getSingleton();
        gossip->regionTolerance[fameLoc].addValue(amt);
        return gossip->regionTolerance[fameLoc].getValue();

    }

    int gossip::removeTolerance(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc, RE::TESGlobal *fameGlobal, int amt) {
        Gossip *gossip = Gossip::getSingleton();
        gossip->regionTolerance[fameLoc].removeValue(amt);
        return gossip->regionTolerance[fameLoc].getValue();
    }
    bool setCurrentRegion(RE::StaticFunctionTag *, RE::BGSLocation *fameLoc) {
        if (Gossip::getSingleton()->currentFameAlias == nullptr) return false;
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
    bool papyrusRegister(RE::BSScript::IVirtualMachine *Registry) {
        Registry->RegisterFunction("newFame", "GIP_SKSE", newFame);
        Registry->RegisterFunction("newFameAlias", "GIP_SKSE", newFameAlias);
        Registry->RegisterFunction("newLocation", "GIP_SKSE", newLocation);
        Registry->RegisterFunction("getFameGlobal", "GIP_SKSE", getFameGlobal);
        Registry->RegisterFunction("getValueTags", "GIP_SKSE", getValueTags);
        Registry->RegisterFunction("newFame", "GIP_SKSE", getFameName);
        Registry->RegisterFunction("getAllFameGlobals", "GIP_SKSE", getAllFameGlobals);
        Registry->RegisterFunction("getAllFameNames", "GIP_SKSE", getAllFameNames);
        Registry->RegisterFunction("getAllLocations", "GIP_SKSE", getAllLocations);
        Registry->RegisterFunction("getGossipValue", "GIP_SKSE", getGossipValue);
        Registry->RegisterFunction("setGossipValue", "GIP_SKSE", setGossipValue);
        Registry->RegisterFunction("modGossipValue", "GIP_SKSE", modGossipValue);
        Registry->RegisterFunction("removeGossipValue", "GIP_SKSE", removeGossipValue);
        Registry->RegisterFunction("addGossipValue", "GIP_SKSE", addGossipValue);
        Registry->RegisterFunction("getFameGlobal", "GIP_SKSE", getFameGlobal);
        Registry->RegisterFunction("setFameValue", "GIP_SKSE", setFameValue);
        Registry->RegisterFunction("removeFameValue", "GIP_SKSE", removeFameValue);
        Registry->RegisterFunction("modFameValue", "GIP_SKSE", modFameValue);
        Registry->RegisterFunction("addFameValue", "GIP_SKSE", addFameValue);
        Registry->RegisterFunction("setInterest", "GIP_SKSE", setInterest);
        Registry->RegisterFunction("addInterest", "GIP_SKSE", addInterest);
        Registry->RegisterFunction("removeInterest", "GIP_SKSE", removeInterest);
        Registry->RegisterFunction("getInterest", "GIP_SKSE", getInterest);
        Registry->RegisterFunction("setTolerance", "GIP_SKSE", setTolerance);
        Registry->RegisterFunction("getTolerance", "GIP_SKSE", getTolerance);
        Registry->RegisterFunction("removeTolerance", "GIP_SKSE", removeTolerance);
        Registry->RegisterFunction("addTolerance", "GIP_SKSE", addTolerance);
        Registry->RegisterFunction("setCurrentRegion", "GIP_SKSE", setCurrentRegion);
        Registry->RegisterFunction("setCurrentAlias", "GIP_SKSE", setCurrentAlias);
        Registry->RegisterFunction("addFameTag", "GIP_SKSE", addFameTag);
        Registry->RegisterFunction("removeFameTag", "GIP_SKSE", removeFameTag);
        return true;
    }
}  // namespace gossip