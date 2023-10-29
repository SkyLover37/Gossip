#include <Gossip.h>
namespace gossip {
    Gossip o_gossip;
    Gossip::Gossip() : profile(RE::PlayerCharacter::GetSingleton()) {
    
        infoRelay = &fame;
    
    }
    fameInfo* Gossip::operator[](RE::TESGlobal *global) {
        if (!global || !isActive) return nullptr;
        auto entry = fame.find(global);
        if (entry == fame.end()) {
            logger::error("Could not find fame associated with {} {} {:x}", global->GetName(),
                          global->GetFormEditorID(), global->GetFormID());
            return nullptr;
        }
        return &entry->second;
    }
    region *Gossip::getRegionObj(RE::TESFaction * fac, RE::BGSLocation * loc) { 
        if (!loc || !isActive) return nullptr;
        logger::debug("getting {} region for {}", loc->GetName(), fac ? fac->GetName() : "player");
        regionMap & rMap = profile.regionMap;
        if (fac) 
        {
            fameAlias *alias = getAliasObj(fac);
            if (alias) {
                rMap = alias->regionMap;       
            }
        }
        auto entry = rMap.find(loc);
        if (entry == rMap.end()) {
            logger::debug("creating new region");
            entry = rMap.insert(std::make_pair(loc, region(loc))).first;
        }
        return &entry->second;
    }
    RE::TESFaction *Gossip::currentFaction() {

        return profile.activeAlias? profile.activeAlias->faction : nullptr; }
    RE::BGSLocation *Gossip::currentLocation() {
        return checkLocation(RE::PlayerCharacter::GetSingleton()->GetCurrentLocation());
    }
    RE::BGSLocation *Gossip::checkLocation(RE::BGSLocation *checkLoc, bool checkParent) {
        logger::debug("Checking if {} is tracked location.", checkLoc->GetFormEditorID());
        if (!checkLoc) 
        {
            logger::debug("checkLocation passed null");
            return nullptr;
        }
        RE::BGSLocation *newLoc;
        if (std::find(trackedLocations.begin(), trackedLocations.end(), checkLoc) != trackedLocations.end()) {
            
            return checkLoc;
        } else if (checkParent && std::find(trackedLocations.begin(), trackedLocations.end(),
                                            newLoc = checkLoc->parentLoc) != trackedLocations.end()) {
            return newLoc;
        }
        return nullptr;
    }

    fameData *Gossip::getFameObj(RE::TESGlobal *global, RE::TESFaction * faction, RE::BGSLocation * loc) {
        
        if (!global || !loc || !isActive) 
        {
            logger::debug("Failed to retrieve fame object {} {} {}", global ? global->GetFormEditorID() : "ERROR",
                          loc ? loc->GetFormEditorID() : "ERROR", isActive);
            return nullptr;
        }
        logger::debug("Getting {} object in {} for {}", global->GetFormEditorID(), loc->GetName(),
                      faction ? faction->GetName() : "Player");
        auto entry = fame.find(global);
        if (entry == fame.end()) return nullptr;
        fameInfo *info = &entry->second;
        if (!info) return nullptr;
        region* fameRegion = getRegionObj(faction, loc);
        if (!fameRegion) return nullptr;
        fameMap &fMap = fameRegion->fameMap;
        auto data = fMap.find(info);
        if (data == fMap.end()) {
            logger::debug("Creating new fame data");
            data = fMap.insert(std::make_pair(info, fameData(info))).first;
        }
        return &data->second;
         
    }
    fameAlias* Gossip::getAliasObj(RE::TESFaction * fac) { 
        if (!fac) return nullptr; 
        auto entry = profile.aliasMap.find(fac);
        return entry != profile.aliasMap.end() ? &entry->second : nullptr;
    }
    void onGameSaved(SKSE::SerializationInterface *evt) {
        logger::info("Game save");
        
        Gossip& gossip = Gossip::getSingleton();
        if (!gossip.isReady()) return;
        if (!evt->OpenRecord('GSIP', 1)) {
        
        } else {
            logger::debug("Saving gossip");
            evt->WriteRecordData(gossip.interest->GetFormID());
            evt->WriteRecordData(gossip.recognition->GetFormID());
        }
        std::size_t size = gossip.fame.size();
        if (!evt->OpenRecord('FAME', 1)) {
        } else {
            evt->WriteRecordData(size);
            logger::info("saving {} fame", size);
            for (auto &famein : Gossip::getSingleton().fame) {
                famein.second(evt);
            }
        }
        if (!evt->OpenRecord('LOCN', 1)) {
        
        } else {
            logger::debug("saving {} locations", o_gossip.trackedLocations.size());
            evt->WriteRecordData(o_gossip.trackedLocations.size());
            for (auto &entry : o_gossip.trackedLocations) {
                evt->WriteRecordData(entry->GetFormID());
            };
        }
        if (!evt->OpenRecord('PROF', 1)) {
        } else {
            //evt->WriteRecordData(gossip->npcProfile.size());
            //for (auto &profile : gossip->npcProfile) {
            logger::debug("saving profile");
            o_gossip.profile(evt);
            //}
        }
        if (!evt->OpenRecord('TLRC', 1)) {
        } else {
            logger::debug("saving {} tolerance", gossip.regionTolerance.size());
            evt->WriteRecordData(gossip.regionTolerance.size());
            for (auto &tolerance : gossip.regionTolerance) {
                evt->WriteRecordData(tolerance.first->formID);
            }
        }
    }
    void onGameLoad(SKSE::SerializationInterface *evt) {
       
        Gossip& o_gossip = Gossip::getSingleton();
        // gossip->busy = true;
        std::uint32_t type;
        std::uint32_t version;
        std::uint32_t length;

        bool error = false;

        while (!error && evt->GetNextRecordInfo(type, version, length)) {
            //
            switch (type) {
                case 'GSIP': {
                    RE::TESGlobal *inter = nullptr;
                    RE::TESGlobal *Recog = nullptr;
                    readForm(evt, inter);
                    readForm(evt, Recog);
                    o_gossip.setup(inter, Recog);
                    break;
                }
                case 'FAME': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    logger::info("fame count {}", size);
                    for (int i = 0; i < size; i++) {
                        auto info = fameInfo(evt);
                        if (!info) continue;
                        o_gossip.fame.insert({info.getGlobal(), info});
                    }
                    break;
                }
                case 'LOCN': {
                    std::size_t size = getSize(evt);
                    o_gossip.trackedLocations.reserve(size);
                    for (int i = 0; i < size; i++) {
                        RE::BGSLocation *loc = nullptr;
                        readForm(evt, loc);
                        if (!loc) continue;
                        o_gossip.trackedLocations.push_back(loc);
                    }
                    break;
                }
                case 'PROF': {
                    o_gossip.profile(evt);
                    break;
                }
                case 'TLRC': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    for (int i = 0; i < size; i++) {
                        int tolerance = 0;
                        RE::BGSLocation *loc = nullptr;
                        RE::TESGlobal *glob = nullptr;
                        readForm(evt, loc);
                        readForm(evt, glob);
                        int tol = 0;
                        evt->ReadRecordData(tol);
                        if (!loc) continue;
                        o_gossip.regionTolerance[loc].insert(std::make_pair(glob, tol));
                    }
                    break;
                }
            }
        }
        // gossip->busy = false;
    }
    void onRevert(SKSE::SerializationInterface *evt) { 

         Gossip& gossip = Gossip::getSingleton();
         
        gossip = Gossip();
    }
}  // namespace gossip