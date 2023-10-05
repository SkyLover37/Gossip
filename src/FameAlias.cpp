#include "FameAlias.h"
//#include <Scandal.h>
namespace gossip {
    fameProfile::fameProfile(SKSE::SerializationInterface* evt) {
        akActor = readForm(evt)->As<RE::TESObjectREFR>();
        evt->ReadRecordData(viewingTime);
        evt->ReadRecordData(depravity);
        evt->ReadRecordData(disGood);
        evt->ReadRecordData(disBad);
        size_t size;
        evt->ReadRecordData(size);
        for (int i = 0; i < size; ++i) {
            fameAlias* alias;// = &Gossip::getSingleton()->Alias[readForm(evt)->As<RE::TESFaction>()];

            interest[alias] = valueData(evt);
        }
    }
    void fameProfile::save(SKSE::SerializationInterface* evt) {
        if (!evt->OpenRecord('PROF', 1)) {
        } else {
            evt->WriteRecordData(akActor->formID);
            evt->WriteRecordData(viewingTime);
            evt->WriteRecordData(depravity);
            evt->WriteRecordData(disGood);
            evt->WriteRecordData(disBad);
            size_t size = interest.size();
            evt->WriteRecordData(size);
            for (auto& entry : interest) {
                evt->WriteRecordData(entry.first->form->formID);
                evt->WriteRecordData(entry.second);
            }
        } 
    }
    int fameProfile::setInterest(RE::BGSLocation* fameLoc, int amt) { return 0; }
    int fameProfile::addInterest(RE::BGSLocation* fameLoc, int amt) { return 0; }
    int fameProfile::removeInterest(RE::BGSLocation* fameLoc, int amt) { return 0; }
    int fameProfile::getInterest(RE::BGSLocation* fameLoc) { return 0; }
    void fameProfile::clearValues(valueType type, RE::BGSLocation* akLoc, RE::TESFaction* akAlias) {
    

    }
    void fameAlias::save(SKSE::SerializationInterface* evt) {
        if (!evt->OpenRecord('ALAS', 1)) {
        } else {
            writeString(evt, name);
            evt->WriteRecordData(form->formID);
            for (auto& knownAlias : known) evt->WriteRecordData(known.size());

            for (auto& knownEntry : known) {
                evt->WriteRecordData(knownEntry.first->formID);
                knownEntry.second.save(evt);
            }
        }
    }
    region* fameAlias::getRegion(RE::BGSLocation* loc) { return &known[loc]; }
   

    valueData* fameAlias::getValueObject(valueType val, RE::BGSLocation* loc, RE::TESGlobal* global) {
        return &known[loc].fame[global][val];
    }
   
}  // namespace gossip