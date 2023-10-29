#include "FameOrigin.h"
namespace gossip {
    infoMap* infoRelay = nullptr;
    fameInfo::fameInfo(RE::TESGlobal* newForm, std::string name, std::uint16_t min, std::uint16_t max, std::vector<std::string> tags)
        : fameGlobal(newForm), name(name), tags(tags), fameLimit(fameLimit::limit_type::regional, min, max) {
        logger::info("New fame {} ", name);
    }

    void fameInfo::save(SKSE::SerializationInterface* evt) {
        logger::debug("Saving {} fame", name);
        fameLimit::save(evt);
        evt->WriteRecordData(fameGlobal->GetFormID());
        std::size_t size = tags.size();
        evt->WriteRecordData(size);
        for (int i = 0; i < tags.size(); i++) {
            writeString(evt, tags[i]);
        }
        writeString(evt, name);
    }

    



    fameData::fameData(SKSE::SerializationInterface* evt) : localBound(evt) {
        
        evt->ReadRecordData(raw);
        evt->ReadRecordData(val);
        evt->ReadRecordData(_gossip);
        fameLimit::limit_type storedLimit;
        //fuck it, it works.
        char store;
        evt->ReadRecordData(store);
        logger::debug("set limit is {}", store);
        switch (store) {
            case 'L': {
                limit = &localBound;
                break;
            }
            case 'R':
            {
                limit = info->getLimit();
                break;
            }
        }
        RE::TESGlobal* glob = nullptr;
        readForm(evt, glob);
        if (!glob) 
        {
            logger::debug("Failed to find associated Fame Global");
            info = nullptr;
            return;
        }
        
        //infoRelay needs to be reset. dumb fuck me.
        auto entry = infoRelay->find(glob);
        info = entry != infoRelay->end() ? &entry->second : nullptr;
        logger::debug("{} {} {}", raw, val, _gossip);
        logger::debug("Limits: {} {} : {} {}", localBound.min(), localBound.max(), info->getLimit()->min(),
                      info->getLimit()->max());
    }
    void fameData::save(SKSE::SerializationInterface* evt) { 
        localBound.save(evt);
        evt->WriteRecordData(raw);
        evt->WriteRecordData(val);
        evt->WriteRecordData(_gossip);
        
        evt->WriteRecordData(static_cast<char>(limit->type));
        evt->WriteRecordData(info->getGlobal()->GetFormID());
    };
}  // namespace gossip