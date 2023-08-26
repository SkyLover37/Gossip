struct region {
    int interest;
    std::map<RE::TESGlobal*, fameData> fame;
    region(){};
    region(SKSE::SerializationInterface* evt) {
        evt->ReadRecordData(interest);
        std::size_t size;
        evt->ReadRecordData(size);
        for (int i = 0; i < size; i++) {
            RE::TESGlobal* glob = readForm(evt)->As<RE::TESGlobal>();
            fame[glob] = fameData(evt);
        }
    }
    void save(SKSE::SerializationInterface* evt) {
        evt->WriteRecordData(interest);
        evt->WriteRecordData(fame.size());
        for (auto& fame : fame) {
            evt->WriteRecordData(fame.first->formID);
            fame.second.save(evt);
        }
    }
};