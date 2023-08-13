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
                        logger::info("index {}", i);
                        int max;
                        int min;
                        
                        std::string name;
                        std::vector<std::string> tags;
                        RE::FormID oldForm;
                        RE::FormID newForm = 0;
                        evt->ReadRecordData(oldForm);
                        evt->ResolveFormID(oldForm, newForm);
                        logger::info("{:x},{:x}", oldForm, newForm);
                        auto handler = RE::TESDataHandler::GetSingleton();
                        auto tempform = RE::TESForm::LookupByID<RE::TESGlobal>(newForm);
                        evt->ReadRecordData(max);

                        evt->ReadRecordData(min);

                        std::size_t size;
                        evt->ReadRecordData(size);
                        logger::info("{}", size);
                        for (int i = 0; i < size; ++i) {
                            std::string elem;
                            evt->ReadRecordData(elem);
                            tags.push_back(elem);
                        }
                        std::size_t strings;
                        evt->ReadRecordData(strings);
                        name.reserve(strings);
                        evt->ReadRecordData(name.data(), static_cast<std::uint32_t>(strings));
                        if (tempform != nullptr) {
                            gossip->fame[tempform] =
                               fameInfo(tempform, name, min, max, tags);
                            logger::info("{}, {}, {}, {}, {}", max, min, tags.size(), name, tempform->GetFormType());

                        } else {
                            logger::info("Failed to retrieve a global");
                        }
                        
                    }
                }
                
            }
        }
        //gossip->busy = false;
    }
    void Gossip::onRevert(SKSE::SerializationInterface *evt) {}
}  // namespace gossip