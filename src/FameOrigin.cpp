#include "FameOrigin.h"
namespace gossip {
    
    template <typename T>
    fameInfo::fameInfo(SKSE::SerializationInterface* evt, T* glob) {
        if (glob) {
            fameGlobal = glob->As<RE::TESGlobal>();
        }
        evt->ReadRecordData(max);

        evt->ReadRecordData(min);

        std::size_t size;
        
        evt->ReadRecordData(size);
        tags.reserve(size);
        for (int i = 0; i < size; ++i) {
            tags.push_back(readString(evt));
        }
        name = readString(evt);
    }
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

    



    fameData::fameData(SKSE::SerializationInterface* evt) : localBound(evt), value(evt){
        RE::TESGlobal* glob = nullptr;
        readForm(evt, glob);
        if (!glob) return;

        fameInfo* tmpInfo = &(*infoRelay)[glob];
    }
    void fameData::operator()(SKSE::SerializationInterface* evt){
        
    };
}  // namespace gossip