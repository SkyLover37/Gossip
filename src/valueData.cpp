#include "valueData.h"
namespace gossip {
    template <typename T, typename B>
    valueData<T, B>::valueData(SKSE::SerializationInterface* evt) : limit(evt) {
        evt->ReadRecordData(raw);
        evt->ReadRecordData(val);
    };
    template <typename T, typename B>
    void valueData<T, B>::save(SKSE::SerializationInterface* evt) {
        evt->WriteRecordData(raw);
        evt->WriteRecordData(val);
        limit(evt);
    }
}  // namespace gossip