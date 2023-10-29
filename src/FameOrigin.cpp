#include "FameOrigin.h"
namespace gossip {
    infoMap* infoRelay = nullptr;
    fameInfo::fameInfo(RE::TESGlobal* newForm, std::string name, std::uint16_t min, std::uint16_t max, std::vector<std::string> tags)
        : fameGlobal(newForm), name(name), tags(tags), fameLimit(fameLimit::limit_type::regional, min, max) {
        logger::info("New fame {} ", name);
    }

    void fameInfo::operator()(SKSE::SerializationInterface* evt) {
        logger::debug("Saving {} fame", name);
        static_cast<fameLimit>(*this)(evt);
        evt->WriteRecordData(fameGlobal->GetFormID());
        std::size_t size = tags.size();
        evt->WriteRecordData(size);
        for (int i = 0; i < tags.size(); i++) {
            writeString(evt, tags[i]);
        }
        writeString(evt, name);
    }

    



    fameData::fameData(SKSE::SerializationInterface* evt) : localBound(evt){
        evt->ReadRecordData(raw);
        evt->ReadRecordData(val);
        evt->ReadRecordData(_gossip);
        RE::TESGlobal* glob = nullptr;
        readForm(evt, glob);
        if (!glob) 
        {
            info = nullptr;
            return;
        }
        auto entry = infoRelay->find(glob);
        info = entry != infoRelay->end() ? &entry->second : nullptr;
    }
    void fameData::operator()(SKSE::SerializationInterface* evt) { 
        evt->WriteRecordData(raw);
        evt->WriteRecordData(val);
        evt->WriteRecordData(_gossip);
        evt->WriteRecordData(info->getGlobal()->GetFormID());
    };
}  // namespace gossip