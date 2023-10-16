#pragma once
namespace gossip {
    void writeString(SKSE::SerializationInterface* evt, std::string string);
    std::string readString(SKSE::SerializationInterface* evt);
    template <class T>
    bool readForm(SKSE::SerializationInterface* evt, T* req);
    size_t getSize(SKSE::SerializationInterface* evt);
}  // namespace gossip