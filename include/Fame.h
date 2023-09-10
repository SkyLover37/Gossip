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
        int setValue(RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
        int addValue(RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
        int removeValue(RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal, int amt);
        int getValue(RE::BGSKeyword* alias, RE::BGSLocation* fameLoc, RE::TESGlobal* fameGlobal);

        void setValueMin(RE::StaticFunctionTag*, RE::TESGlobal* global, int amt, RE::BGSLocation* newLoc);
        void setValueMax(RE::StaticFunctionTag*, RE::TESGlobal* global, int amt, RE::BGSLocation* newLoc);

    };
}  // namespace goszsip