#include "Scandal.h"
namespace gossip {
    
    bool Gossip::newFameAlias(RE::StaticFunctionTag *, RE::TESForm* akAlias, RE::BSString aliasName) {
        
        for (auto &entry : Gossip::getSingleton()->fame) {
            auto *info = &(entry.second);

            auto newAlias = fameInfo::fameAlias{};

            newAlias.name = aliasName;
            newAlias.form = akAlias;
            info->localFame[akAlias] = newAlias;
        };
        return true
      
    }
    bool Gossip::newFame(RE::StaticFunctionTag *, RE::TESGlobal *global, RE::BSString fameName) { return false; }
    RE::TESGlobal *Gossip::getFameGlobal(RE::StaticFunctionTag *, RE::BSString globalName) { return nullptr; }
    RE::BSString Gossip::getFameName(RE::StaticFunctionTag *, RE::TESGlobal *global) { return RE::BSString(); }
    std::vector<RE::TESGlobal *> Gossip::getAllFameGlobals(RE::StaticFunctionTag *) {
        return std::vector<RE::TESGlobal *>();
    }
    std::vector<RE::BSString> Gossip::getAllFameNames(RE::StaticFunctionTag *) { return std::vector<RE::BSString>(); }
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
    bool Gossip::newLocation(RE::StaticFunctionTag *, RE::BGSLocation *newLoc, RE::BSString locName) { return false; }
    std::vector<RE::BGSLocation *> Gossip::getAllLocations(RE::StaticFunctionTag *) {
        return std::vector<RE::BGSLocation *>();
    }
    std::vector<RE::BSString> Gossip::getAllLocationNames(RE::StaticFunctionTag *) {
        return std::vector<RE::BSString>();
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
    void Gossip::onGameSaved(SKSE::SerializationInterface *evt) {}
    void Gossip::onGameLoad(SKSE::SerializationInterface *evt) {}
    void Gossip::onRevert(SKSE::SerializationInterface *evt) {}
}  // namespace gossip