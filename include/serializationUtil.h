#pragma once
namespace gossip {
    void writeString(SKSE::SerializationInterface* evt, std::string string);
    std::string readString(SKSE::SerializationInterface* evt);
    RE::TESForm* readForm(SKSE::SerializationInterface* evt);
    size_t getSize(SKSE::SerializationInterface* evt);
}  // namespace gossip