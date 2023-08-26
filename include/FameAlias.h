struct fameProfile {
    RE::TESObjectREFR* akActor;
    float viewingTime;
    int interest;
    int relation;
};
struct fameAlias {
    std::string name = "";
    RE::TESForm* form = nullptr;
    std::map<RE::BGSLocation*, region> known;
    fameAlias(){};
    fameAlias(SKSE::SerializationInterface* evt) {
        name = readString(evt);
        form = readForm(evt);
        std::size_t size;
        evt->ReadRecordData(size);
        for (int i = 0; i < size; i++) {
            RE::BGSLocation* loc = readForm(evt)->As<RE::BGSLocation>();
            known[loc] = region(evt);
        }
    };
    fameAlias(std::string name, RE::TESForm* form) : name(name), form(form) {
        for (auto& entry : Gossip::getSingleton()->trackedLocations) {
            addLocation(entry);
        }
    }
    void addLocation(RE::BGSLocation* akLoc) {
        for (auto& fameEntry : Gossip::getSingleton()->fame) {
            known[akLoc].fame[fameEntry.first] = fameData();
        }
    }
    void save(SKSE::SerializationInterface* evt) {
        if (!evt->OpenRecord('ALAS', 1)) {
        } else {
            writeString(evt, name);
            evt->WriteRecordData(form->formID);
            evt->WriteRecordData(known.size());
            for (auto& knownEntry : known) {
                evt->WriteRecordData(knownEntry.first->formID);
                knownEntry.second.save(evt);
            }
        }
    }

    void load(SKSE::SerializationInterface* evt) {
        std::uint32_t type;
        std::uint32_t version;
        std::uint32_t length;
        RE::FormID oldForm;
        bool error = false;
        evt->ReadRecordData(&oldForm, length);
        evt->ResolveFormID(oldForm, oldForm);
        form = RE::TESForm::LookupByID(oldForm);
        if (form) {
            logger::info("{} :: {}", form->GetName(), oldForm);
        } else
            logger::error("Failed to retrieve form :: {}", oldForm);
    }
};