namespace gossip {
    void writeString(SKSE::SerializationInterface* evt, std::string string) {
        std::size_t size = string.length();
        evt->WriteRecordData(size);
        evt->WriteRecordData(string.data(), static_cast<std::uint32_t>(size));
    }
    std::string readString(SKSE::SerializationInterface* evt) {
        std::string name;
        std::size_t strings;
        evt->ReadRecordData(strings);
        name.resize(strings);
        evt->ReadRecordData(name.data(), static_cast<std::uint32_t>(strings));
        return name;
    }
   
    size_t getSize(SKSE::SerializationInterface* evt) {
        size_t size;
        evt->ReadRecordData(size);
        return size;
    }
}