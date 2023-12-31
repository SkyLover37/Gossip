#include <Gossip.h>
namespace gossip {
    Gossip o_gossip;
    Gossip::Gossip() : profile(RE::PlayerCharacter::GetSingleton()) {
    
        infoRelay = &fame;
    
    }

    SKSE::RegistrationSet<const RE::TESObjectREFR *> playerSightEvent{"gossipSight"sv};
    SKSE::RegistrationSet<const RE::TESObjectREFR *> playerSightLostEvent{"gossipLostSight"sv};
    SKSE::RegistrationSet<const RE::BGSLocation *, const RE::BGSLocation *> playerRegionChange{"gossipRegionChange"sv}; 
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
        if (!checkLoc) 
        {
            logger::debug("checkLocation passed null");
            return nullptr;
        }

        logger::debug("Checking if {} is tracked location.", checkLoc->GetName());
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
        if (entry == fame.end()) 
        {
            logger::debug("Failed to find fameInfo");
            return nullptr;
        }
        fameInfo *info = &entry->second;
        if (!info) return nullptr;
        region* fameRegion = getRegionObj(faction, loc);
        if (!fameRegion) 
        {
            logger::debug("Failed to retrieve fame region");
            return nullptr;
        }
        fameMap &fMap = fameRegion->fameMap;
        auto data = fMap.find(info);
        if (data == fMap.end()) {
            logger::debug("Creating new fame data");
            data = fMap.insert(std::make_pair(info, fameData(info))).first;
        }
        if (!data->second.limit) 
        {
            logger::debug("The limit value of {} fameData in {} on {} is not set", global->GetFormEditorID(),
                          loc->GetName(), faction ? faction->GetName() : "Player");
            return nullptr;
        }
        return &data->second;
         
    }
    tolerance *Gossip::getToleranceObj(RE::BGSLocation *loc, RE::TESGlobal *glob) {
        if (!glob || !loc || !isActive) {
            logger::debug("Failed to retrieve tolerance object {} {} {}",
                          glob ? glob->GetFormEditorID() : "GLOBAL::ERROR",
                          loc ? loc->GetFormEditorID() : "LOCATION::ERROR", isActive);
            return nullptr;
        }
        logger::debug("Getting {} tolerance object in {}", glob->GetFormEditorID(), loc->GetFormEditorID());
        auto locentry = regionTolerance[loc];
        auto entry = locentry.insert(std::make_pair(glob, tolerance()));
        return &entry.first->second;

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
        if (!evt->OpenRecord('GSIP', 3)) {
        
        } else {
            logger::debug("Saving gossip");
            playerSightEvent.Save(evt);
            playerSightLostEvent.Save(evt);
            playerRegionChange.Save(evt);
            evt->WriteRecordData(gossip.interest->GetFormID());
            evt->WriteRecordData(gossip.recognition->GetFormID());
            evt->WriteRecordData(gossip.isActive);
        }
        std::size_t size = gossip.fame.size();
        if (!evt->OpenRecord('FAME', 3)) {
        } else {
            evt->WriteRecordData(size);
            logger::info("saving {} fame", size);
            for (auto &famein : Gossip::getSingleton().fame) {
                famein.second.save(evt);
            }
        }
        if (!evt->OpenRecord('LOCN', 3)) {
        
        } else {
            logger::debug("saving {} locations", o_gossip.trackedLocations.size());
            evt->WriteRecordData(o_gossip.trackedLocations.size());
            for (auto &entry : o_gossip.trackedLocations) {
                evt->WriteRecordData(entry->GetFormID());
            };
        }
        if (!evt->OpenRecord('PROF', 3)) {
        } else {
            //evt->WriteRecordData(gossip->npcProfile.size());
            //for (auto &profile : gossip->npcProfile) {
            logger::debug("saving profile");
            o_gossip.profile.operator()(evt);
            //}
        }
        if (!evt->OpenRecord('TLRC', 3)) {
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
        bool isActive = false;
        bool error = false;

        while (!error && evt->GetNextRecordInfo(type, version, length)) {
            //
            switch (type) {
                case 'GSIP': {
                    logger::debug("Loading gossip object");
                    RE::TESGlobal *inter = nullptr;
                    RE::TESGlobal *Recog = nullptr;
                    if (version >= 2) {
                        playerSightEvent.Load(evt);
                        playerSightLostEvent.Load(evt);
                        playerRegionChange.Load(evt);
                    }
                    readForm(evt, inter);
                    readForm(evt, Recog);
                    o_gossip.setup(inter, Recog);
                    evt->ReadRecordData(isActive);
                    break;
                }
                case 'FAME': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    logger::info("fame count {}", size);
                    for (int i = 0; i < size; i++) {
                        auto info = fameInfo(evt);
                        if (!info) continue;
                       
                        auto entry = o_gossip.fame.insert({info.getGlobal(), info});
                        if (!entry.second) {
                            if (entry.first->second.getGlobal() == o_gossip.interest) {
                                logger::info("Loading interest");
                                entry.first->second = info;
                                continue;
                            } else if (entry.first->second.getGlobal() == o_gossip.recognition) {
                                logger::info("Loading recognition");
                                entry.first->second = info;
                                continue;
                            }
                            logger::debug("Fame entry found to already exist during load, this shouldn't happen.");
                        }

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
                    logger::debug("Loading profile");
                    o_gossip.profile = fameProfile(evt, version);
                    break;
                }
                case 'TLRC': {
                    logger::debug("Loading tolerance");
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
        o_gossip.setActive(isActive);
        // gossip->busy = false;
    }
    void onRevert(SKSE::SerializationInterface *evt) { 

        Gossip& gossip = Gossip::getSingleton();
         
        gossip = Gossip();
        infoRelay = &gossip.fame;
    }
}  // namespace gossip