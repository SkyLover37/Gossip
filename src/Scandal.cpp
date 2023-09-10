#include "Scandal.h"
namespace gossip {
    
    bool Gossip::newFameAlias(RE::StaticFunctionTag *, RE::TESFaction* akAlias, std::string aliasName) {
        
        Gossip::getSingleton()->Alias[akAlias] = fameAlias(aliasName, akAlias);

        return true;
    }
    bool Gossip::newFame(RE::StaticFunctionTag *, RE::TESGlobal *global, std::string fameName, int min, int max, std::vector<std::string> tags) { 
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
                        RE::TESFaction *form = readForm(evt)->As<RE::TESFaction>();
                        gossip->Alias[form] = fameAlias(evt);
                    }
                    break;
               }
               case 'PROF': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    for (int i = 0; i < size; i++) {
                        fameProfile akProfile(evt);
                        gossip->npcProfile[akProfile.akActor] = akProfile;
                    }
                   break;
               }
               case 'TLRC': {
                    std::size_t size;
                    evt->ReadRecordData(size);
                    for (int i = 0; i < size; i++) {
                        int tolerance;

                        RE::BGSLocation *form = readForm(evt)->As<RE::BGSLocation>();
                        valueData tol(evt);
                        gossip->regionTolerance[form] = tol; 
                    }
                    break;
               }
                
            }
        }
        //gossip->busy = false;
    }
    void Gossip::onRevert(SKSE::SerializationInterface *evt) {}
}  // namespace gossip