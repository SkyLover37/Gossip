#include "FameOrigin.h"
namespace gossip {
    infoMap* infoRelay = nullptr;
    fameInfo::fameInfo(RE::TESGlobal* newForm, std::string name, int min, int max, std::vector<std::string> tags)
        : fameGlobal(newForm), name(name), tags(tags), limit(fame::regional, min, max) {
        logger::info("New fame {} ", name);
    }

    void fameInfo::operator()(SKSE::SerializationInterface* evt) {
        evt->WriteRecordData(fameGlobal->GetFormID());
        std::size_t size = tags.size();
        evt->WriteRecordData(size);
        for (int i = 0; i < tags.size(); i++) {
            writeString(evt, tags[i]);
        }
        size = name.length() + 1;
        evt->WriteRecordData(size);
        evt->WriteRecordData(name.data(), static_cast<std::uint32_t>(size));
    }

    



    fameData::fameData(SKSE::SerializationInterface* evt) : localBound(evt), _gossip(evt), value(evt){
        RE::TESGlobal* glob = nullptr;
        readForm(evt, glob);
        if (!glob) 
        {
            return;
        }
        auto entry = infoRelay->find(glob);
        info = entry != infoRelay->end() ? &entry->second : nullptr;
    }
    void fameData::operator()(SKSE::SerializationInterface* evt){
        
    };
}  // namespace gossip