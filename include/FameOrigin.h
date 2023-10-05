#pragma once
#include <serializationUtil.h>
namespace gossip {
    struct fameInfo {
        int max = 100;
        int min = 0;
        std::vector<std::string> tags;
        std::string name = "";
        RE::TESGlobal* fameGlobal = nullptr;
        fameInfo(){};
        template <typename T>
        fameInfo(SKSE::SerializationInterface* evt, T* glob) {
            std::string name;
            std::vector<std::string> tags;
            if (glob) {
                fameGlobal = glob->As<RE::TESGlobal>();
            }
            evt->ReadRecordData(max);

            evt->ReadRecordData(min);

            std::size_t size;
            evt->ReadRecordData(size);
            for (int i = 0; i < size; ++i) {
                tags.push_back(readString(evt));
            }
            name = readString(evt);
        }
        fameInfo(RE::TESGlobal* newForm, std::string name, int min, int max, std::vector<std::string> tags)
            : fameGlobal(newForm), name(name), min(min), max(max), tags(tags) {
            logger::info("New fame {} ", name);
        }

        void save(SKSE::SerializationInterface* evt) {
            evt->WriteRecordData(fameGlobal->GetFormID());
            evt->WriteRecordData(max);
            evt->WriteRecordData(min);
            std::size_t size = tags.size();
            evt->WriteRecordData(size);
            for (int i = 0; i < tags.size(); i++) {
                writeString(evt, tags[i]);
            }
            size = name.length() + 1;
            evt->WriteRecordData(size);
            evt->WriteRecordData(name.data(), static_cast<std::uint32_t>(size));
        }
        void setValueMin(int amt);
        void setValueMax(int amt);
        int getValueMin();
        int getValueMax();
    };
}  // namespace gossip