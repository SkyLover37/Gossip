#include <Gossip.h>
namespace gossip {
    Gossip::Gossip() : profile(RE::PlayerCharacter::GetSingleton()) {
    
        infoRelay = &fame;
    
    }
    fameInfo &Gossip::operator[](RE::TESGlobal *global) {
        auto entry = fame.find(global);
        if (entry == fame.end())
            logger::error("Could not find fame associated with {} {} {:x}", global->GetName(),
                          global->GetFormEditorID(), global->GetFormID());
        return entry->second;
    }
    region *Gossip::getRegionObj(RE::TESFaction * fac, RE::BGSLocation * loc) { 
        if (fac)
            return &profile[fac][loc];
        else
            return &profile[loc];
    }
    RE::TESFaction *Gossip::currentFaction() {
        return (profile.activeAlias->faction); }
    RE::BGSLocation *Gossip::currentLocation() {
        return checkLocation(RE::PlayerCharacter::GetSingleton()->GetCurrentLocation());
    }
    RE::BGSLocation *Gossip::checkLocation(RE::BGSLocation *checkLoc, bool checkParent) {
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
        auto entry = fame.find(global);
        return entry != fame.end() ? profile[faction][checkLocation(loc)][&entry->second] : nullptr;
    }
    fameAlias *Gossip::getAliasObj(RE::TESFaction * fac) { 
         auto entry = profile.aliasMap.find(fac);
        return &entry->second;
    }
    bool Gossip::setState(bool active) {
        o_gossip.active = active;
        return o_gossip.active;
    }
    void Gossip::onGameSaved(SKSE::SerializationInterface *evt) {
        logger::info("Game save");
        Gossip&gossip = Gossip::getSingleton();

        std::size_t size = gossip.fame.size();
        if (!evt->OpenRecord('FAME', 1)) {
        } else {
            evt->WriteRecordData(size);
            logger::info("{}", size);
            for (auto &famein : Gossip::getSingleton().fame) {
                famein.second(evt);
            }
        }
        if (!evt->OpenRecord('LOCN', 1)) {
        
        } else {
            evt->WriteRecordData(o_gossip.trackedLocations.size());
            for (auto &entry : o_gossip.trackedLocations) {
                evt->WriteRecordData(entry->GetFormID());
            };
        }
        if (!evt->OpenRecord('PROF', 1)) {
        } else {
            //evt->WriteRecordData(gossip->npcProfile.size());
            //for (auto &profile : gossip->npcProfile) {
            o_gossip.profile(evt);
            //}
        }
        if (!evt->OpenRecord('TLRC', 1)) {
        } else {
            evt->WriteRecordData(gossip.regionTolerance.size());
            for (auto &tolerance : gossip.regionTolerance) {
                evt->WriteRecordData(tolerance.first->formID);
            }
        }
    }
    void Gossip::onGameLoad(SKSE::SerializationInterface *evt) {
        o_gossip = Gossip();
        // gossip->busy = true;
        std::uint32_t type;
        std::uint32_t version;
        std::uint32_t length;

        bool error = false;

        while (!error && evt->GetNextRecordInfo(type, version, length)) {
            //
            switch (type) {
                case 'FAME': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    logger::info("fame count {}", size);
                    for (int i = 0; i < size; ++i) {
                        auto info = fameInfo(evt);
                        if (!info) continue;
                        log::debug("Retrieved fame {}", info.getGlobal()->GetName());
                        o_gossip.fame.insert({info.getGlobal(), info});
                    }
                    break;
                }
                case 'LOCN': {
                    std::size_t size = getSize(evt);
                    o_gossip.trackedLocations.reserve(size);
                    for (int i = 0; i < size; i++) {
                        RE::BGSLocation *loc;
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
                        int tolerance;
                        RE::BGSLocation *loc;
                        readForm(evt, loc);
                        int tol;
                        evt->ReadRecordData(tol);
                        if (!loc) continue;
                        o_gossip.regionTolerance.insert({loc, tol});
                    }
                    break;
                }
            }
        }
        // gossip->busy = false;
    }
    void Gossip::onRevert(SKSE::SerializationInterface *evt) { o_gossip = Gossip(); }
}  // namespace gossip