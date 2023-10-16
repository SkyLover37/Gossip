#include "valueData.h"
namespace gossip {
    valueData::valueData(SKSE::SerializationInterface* evt) {
        evt->ReadRecordData(_value);
        evt->ReadRecordData(_max);
        evt->ReadRecordData(_min);
    };
    void valueData::save(SKSE::SerializationInterface* evt) {
        evt->WriteRecordData(_value);
        evt->WriteRecordData(_max);
        evt->WriteRecordData(_min);
    };
}  // namespace gossip