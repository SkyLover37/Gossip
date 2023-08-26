struct fameData {
    int fameValue{};
    int gossip{};
    bool localLimit{};
    int max = 100;
    int min = 0;
    fameData(){};
    fameData(SKSE::SerializationInterface* evt) {
        evt->ReadRecordData(fameValue);
        evt->ReadRecordData(gossip);
        evt->ReadRecordData(localLimit);
        evt->ReadRecordData(max);
        evt->ReadRecordData(min);
    };
    void save(SKSE::SerializationInterface* evt) {
        evt->WriteRecordData(fameValue);
        evt->WriteRecordData(gossip);
        evt->WriteRecordData(localLimit);
        evt->WriteRecordData(max);
        evt->WriteRecordData(min);
    };
};