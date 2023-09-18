#include <Region.h>
namespace gossip {
    void region::save(SKSE::SerializationInterface* evt) {
        evt->WriteRecordData(interest);
        evt->WriteRecordData(fame.size());
        for (auto& fame : fame) {
            evt->WriteRecordData(fame.first->formID);
            evt->WriteRecordData(fame.second.size());
            for (auto& grup : fame.second) {
                grup.second.save(evt);
            }
        }
    }
}  // namespace gossip