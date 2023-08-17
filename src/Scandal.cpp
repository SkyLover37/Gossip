#include "Scandal.h"
namespace gossip {
    
    bool Gossip::newFameAlias(RE::StaticFunctionTag *, RE::TESForm* akAlias, std::string aliasName) {
        
        Gossip::getSingleton()->Alias[akAlias] = Gossip::fameAlias(aliasName, akAlias);

        return true;
    }
    bool Gossip::newFame(RE::StaticFunctionTag *, RE::TESGlobal *global, std::string fameName, int min, int max, std::vector<std::string> tags) { 
        if (Gossip::getSingleton()->busy) return false;
        if (!global) return false;
        Gossip::getSingleton()->fame[global] = fameInfo(global, fameName, min, max, tags);
        logger::info("New fame count: {}", Gossip::getSingleton()->fame.size());
        return true; 
    }
    RE::TESGlobal *Gossip::getFameGlobal(RE::StaticFunctionTag *, std::string globalName) { return nullptr; }
        std::string Gossip::getFameName(RE::StaticFunctionTag *, RE::TESGlobal *global) { return std::string(); }
        std::vector<RE::TESGlobal *> Gossip::getAllFameGlobals(RE::StaticFunctionTag *) {
        return std::vector<RE::TESGlobal *>();
    }
    std::vector<std::string> Gossip::getAllFameNames(RE::StaticFunctionTag *) { return std::vector<std::string>(); }
    void Gossip::setFameMin(RE::StaticFunctionTag *, RE::TESGlobal *global, int amt, RE::BGSLocation *newLoc) {}
    void Gossip::setFameMax(RE::StaticFunctionTag *, RE::TESGlobal *global, int amt, RE::BGSLocation *newLoc) {}
    int Gossip::setFame(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                        RE::TESGlobal *fameGlobal, int amt) {
        return 0;
    }
    int Gossip::addFame(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                        RE::TESGlobal *fameGlobal, int amt) {
        return 0;
    }
    int Gossip::removeFame(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                           RE::TESGlobal *fameGlobal, int amt) {
        return 0;
    }
    int Gossip::getFame(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                        RE::TESGlobal *fameGlobal) {
        return 0;
    }
    bool Gossip::newLocation(RE::StaticFunctionTag *, RE::BGSLocation *newLoc, std::string locName) { return false; }
    std::vector<RE::BGSLocation *> Gossip::getAllLocations(RE::StaticFunctionTag *) {
        return std::vector<RE::BGSLocation *>();
    }
    std::vector<std::string> Gossip::getAllLocationNames(RE::StaticFunctionTag *) {
        return std::vector<std::string>();
    }
    int Gossip::setGossip(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                          RE::TESGlobal *fameGlobal, int amt) {
        return 0;
    }
    int Gossip::addGossip(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                          RE::TESGlobal *fameGlobal, int amt) {
        return 0;
    }
    int Gossip::removeGossip(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                             RE::TESGlobal *fameGlobal, int amt) {
        return 0;
    }
    int Gossip::getGossip(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                          RE::TESGlobal *fameGlobal) {
        return 0;
    }
    int Gossip::setInterest(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc, int amt) {
        return 0;
    }
    int Gossip::addInterest(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc, int amt) {
        return 0;
    }
    int Gossip::removeInterest(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc, int amt) {
        return 0;
    }
    int Gossip::getInterest(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc) { return 0; }
    int Gossip::setTolerance(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                             RE::TESGlobal *fameGlobal, int amt) {
        return 0;
    }
    int Gossip::getTolerance(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                             RE::TESGlobal *fameGlobal) {
        return 0;
    }
    int Gossip::addTolerance(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                             RE::TESGlobal *fameGlobal, int amt) {
        return 0;
    }
    int Gossip::removeTolerance(RE::StaticFunctionTag *, RE::BGSKeyword *alias, RE::BGSLocation *fameLoc,
                                RE::TESGlobal *fameGlobal, int amt) {
        return 0;
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
        
        
        Gossip *gossip = Gossip::getSingleton();
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
                        RE::TESGlobal *tempform = readForm(evt)->As<RE::TESGlobal>();
                        gossip->fame[tempform] = fameInfo(evt, tempform);
                        
                    }
                    break;
                }
               case 'ALAS': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    for (int i = 0; i < size; i++) {
                        RE::TESForm *form = readForm(evt);
                        gossip->Alias[form] = fameAlias(evt);
                    }
                    break;
               }
               case 'TLRC': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    for (int i = 0; i < size; i++) {
                        int tolerance;

                        RE::BGSLocation *form = readForm(evt)->As<RE::BGSLocation>();
                        evt->ReadRecordData(tolerance);
                        gossip->regionTolerance[form] = tolerance; 
                    }
                    break;
               }
                
            }
        }
        //gossip->busy = false;
    }
    void Gossip::onRevert(SKSE::SerializationInterface *evt) {}
}  // namespace gossip