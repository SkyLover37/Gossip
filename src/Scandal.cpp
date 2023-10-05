#include "Scandal.h"
namespace gossip {
    RE::BGSLocation *Gossip::checkLocation(RE::BGSLocation *checkLoc, bool checkParent) { 
        RE::BGSLocation *newLoc;
        if (std::find(trackedLocations.begin(), trackedLocations.end(), checkLoc) != trackedLocations.end()) {
            return checkLoc;
        } else if (checkParent && std::find(trackedLocations.begin(), trackedLocations.end(), newLoc = checkLoc->parentLoc) != trackedLocations.end()) {
            return newLoc;
        }
        return nullptr;
    }

    valueData *Gossip::getInterest() { return currentRegion->getInterest(); }

    valueData *Gossip::getInterest(RE::BGSLocation *akLoc, RE::TESFaction *alias) {
        return Alias[alias].known[akLoc].getInterest();
    }

    valueData *Gossip::getInterest(RE::BGSLocation *akLoc) { return currentFameAlias->known[akLoc].getInterest(); }
   
    valueData *Gossip::getValueObj(valueType valueType, RE::TESGlobal* global) { 
        return &currentRegion->fame[global][valueType];
    }
    valueData *Gossip::getValueObj(valueType valueType, RE::BGSLocation *loc, RE::TESGlobal *global) { 
        return &currentFameAlias->known[loc].fame[global][valueType];
    }
    valueData *Gossip::getValueObj(valueType valueType, RE::TESFaction *fac, RE::BGSLocation *Loc,
                                   RE::TESGlobal *global) {
        return &Alias[fac].known[Loc].fame[global][valueType];
    }
    bool Gossip::setState(bool active) { 
        o_gossip.active = active;
        return o_gossip.active;
    }
    void Gossip::onGameSaved(SKSE::SerializationInterface *evt) {
        logger::info("Game save");
        Gossip *gossip = Gossip::getSingleton();

        std::size_t size = gossip->fame.size();
        if (!evt->OpenRecord('FAME', 1)) {
        } else {
            evt->WriteRecordData(size);
            logger::info("{}", size);
            for (auto &famein : Gossip::getSingleton()->fame) {
                famein.second.save(evt);
            }
        }
        if (!evt->OpenRecord('ALAS', 1)) { 
        }else {
            evt->WriteRecordData(gossip->Alias.size());
            for (auto& alias : gossip->Alias)
            {
                evt->WriteRecordData(alias.first->formID);
                alias.second.save(evt);
            }
        }
        if (!evt->OpenRecord('PROF', 1)) {
        
        } else {
            evt->WriteRecordData(gossip->npcProfile.size());
            for (auto &profile : gossip->npcProfile) {
                profile.second.save(evt);
            }
        }
        if (!evt->OpenRecord('TLRC', 1)) {
        
        } else {
            evt->WriteRecordData(gossip->regionTolerance.size());
            for (auto &tolerance : gossip->regionTolerance) {
                evt->WriteRecordData(tolerance.first->formID);
                evt->WriteRecordData(tolerance.second);
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
                        RE::TESForm* tempf = readForm(evt);
                        auto info = fameInfo(evt, tempf);
                        if (!tempf) continue;
                    
                        log::debug("Retrieved fame {}", info.fameGlobal->GetName());
                        o_gossip.fame[info.fameGlobal] = info; 
                        
                    }
                    break;
                }
               case 'ALAS': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    for (int i = 0; i < size; i++) {
                        auto form = readForm(evt);
                        auto alias = fameAlias(evt);
                        if (!form) continue;
                        RE::TESFaction *fac = form->As<RE::TESFaction>();
                        o_gossip.Alias[fac] = alias; 
                    }
                    break;
               }
               case 'PROF': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    for (int i = 0; i < size; i++) {
                        fameProfile akProfile(evt);
                        if (!akProfile.akActor) continue;
                        o_gossip.npcProfile[akProfile.akActor] = akProfile;
                    }
                   break;
               }
               case 'TLRC': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    for (int i = 0; i < size; i++) {
                        int tolerance;
                        auto form = readForm(evt);
                        valueData tol(evt);
                        if (!form) continue;
                        RE::BGSLocation *loc = form->As<RE::BGSLocation>();
                        
                        o_gossip.regionTolerance[loc] = tol; 
                    }
                    break;
               }
                
            }
        }
        //gossip->busy = false;
    }
    void Gossip::onRevert(SKSE::SerializationInterface *evt) { o_gossip = Gossip(); }
}  // namespace gossip