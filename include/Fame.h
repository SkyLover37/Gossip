#pragma once
#include <FameOrigin.h>
namespace gossip {
    enum valueType { 
        tolerance,
        fame,
        gossip,
        interest,
    };
    struct valueData {
        valueType type;
        fameInfo* info;
        int value = 0;
        bool localLimit;
        int max = 100;
        int min = 0;
        valueData() {}
        valueData(valueType type) : type(type) {};
        valueData(SKSE::SerializationInterface* evt) {
            evt->ReadRecordData(type);
            evt->ReadRecordData(localLimit);
            evt->ReadRecordData(value);
            evt->ReadRecordData(max);
            evt->ReadRecordData(min);
        };
        void save(SKSE::SerializationInterface* evt) {
            evt->WriteRecordData(type);
            evt->WriteRecordData(localLimit);
            evt->WriteRecordData(value);
            evt->WriteRecordData(max);
            evt->WriteRecordData(min);

        };
        int setValue(int amt);
        int addValue(int amt);
        int removeValue(int amt);
        int modValue(int amt);
        int getValue();
        void setValueMin(int amt);
        void setValueMax(int amt);
        int getValueMin();
        int getValueMax();
        

    };
}  // namespace goszsip