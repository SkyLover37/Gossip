

namespace gossip {
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
                evt->WriteRecordData(entry.first.form->formID);
                evt->WriteRecordData(entry.second);
            }
        } 
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
}  // namespace gossip