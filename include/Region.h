#pragma once
#include <Fame.h>

namespace gossip {
    struct region {
        valueData interest;
        RE::TESForm* form;
        typedef std::map<valueType, valueData> fameGroup;
        std::map<RE::TESGlobal*, fameGroup> fame;
        region(){};
        region(SKSE::SerializationInterface* evt) {
            interest = valueData(evt);
            form = readForm(evt);
            std::size_t size;
            evt->ReadRecordData(size);
            for (int i = 0; i < size; i++) {
                RE::TESGlobal* glob = readForm(evt)->As<RE::TESGlobal>();
                size_t size = getSize(evt);
                for (int i = 0; i < size; ++i) {
                    valueData data(evt);
                    fame[glob][data.type] = data;
                }
            }
        }
        void save(SKSE::SerializationInterface* evt);
        valueData* getInterest();
    };
}  // namespace gossip