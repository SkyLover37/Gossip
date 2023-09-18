#include "Papyrus.h"
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

    int getGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, RE::TESFaction *fameAlias) {
        return 0;
    }

    int setGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, RE::TESFaction *fameAlias) {
        return 0;
    }

    int removeGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, RE::TESFaction *fameAlias) {
        return 0;
    }

    int addGossipValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, RE::TESFaction *fameAlias) {
        return 0;
    }

    int getFameValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, RE::TESFaction *fameAlias) { return 0; }

    int setFameValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, RE::TESFaction *fameAlias) { return 0; }

    int removeFameValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, RE::TESFaction *fameAlias) {
        return 0;
    }

    int addFameValue(RE::StaticFunctionTag *, RE::TESGlobal *valueKey, int amt, RE::TESFaction *fameAlias) { return 0; }

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
}  // namespace gossip