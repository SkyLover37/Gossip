#include "FameAlias.h"
//#include <Scandal.h>
namespace gossip {
    
    fameAlias::fameAlias(SKSE::SerializationInterface* evt, std::uint32_t version) {
        logger::debug("loading alias");
        name = readString(evt);
        readForm(evt, faction);

        // std::string aliasName = readString(evt);

        std::size_t size;
        evt->ReadRecordData(size);
        for (int i = 0; i < size; i++) {
            auto reg = region(evt, version);
            if (!reg) continue;
            regionMap.insert({reg.tLoc, reg});
        }
    }
    void fameAlias::operator()(SKSE::SerializationInterface* evt) {
        logger::debug("saving alias");
        writeString(evt, name);
        evt->WriteRecordData(faction->formID);

        evt->WriteRecordData(regionMap.size());
        for (auto& knownEntry : regionMap) {
            evt->WriteRecordData(knownEntry.first->formID);
            knownEntry.second(evt);
        }
        
        
    }
    fameProfile::fameProfile(SKSE::SerializationInterface* evt, std::uint32_t Version) {
        logger::debug("loading profile");
        //readForm(evt, akActor);
        std::size_t size = getSize(evt);
        for (int i = 0; i < size; i++) {
            auto data = fameAlias(evt, Version);
            aliasMap.insert(std::make_pair(data.faction, data));
        }

        size = getSize(evt);
        logger::debug("Loading {} regions", size);
        for (int i = 0; i < size; ++i) {
            auto data = region(evt, Version);
            if (!data.tLoc) continue;
            regionMap.insert(std::make_pair(data.tLoc, data));
        }
        size = getSize(evt);
        sawPlayerSex.reserve(size);
        for (int i = 0; i < size; ++i) {
            RE::Actor* actorForm;
            readForm(evt, actorForm);
            if (!actorForm) continue;
            logger::debug("Loaded {} from co-save", actorForm->GetName());
            sawPlayerSex.push_back(actorForm);
        }
        logger::debug("Finished profile load");
    }
    void fameProfile::operator()(SKSE::SerializationInterface* evt) {
        logger::debug("saving profile");
        //evt->WriteRecordData(akActor->GetFormID());
        evt->WriteRecordData(aliasMap.size());
        for (auto entry : aliasMap) {
            logger::debug("Saving {} alias", entry.second.faction->GetFormEditorID());
            entry.second(evt);
        }
        evt->WriteRecordData(regionMap.size());
        for (auto entry : regionMap) {
            logger::debug("Saving {} region", entry.second.tLoc->GetFormEditorID());
            entry.second(evt);
        }
        evt->WriteRecordData(sawPlayerSex.size());
        for (auto& entry : sawPlayerSex) {
            evt->WriteRecordData(entry->GetFormID());
        }
    }
}  // namespace gossip