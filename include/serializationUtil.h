
namespace gossip {
    void writeString(SKSE::SerializationInterface* evt, std::string string);
    std::string readString(SKSE::SerializationInterface* evt);
    template <class T>
    bool readForm(SKSE::SerializationInterface* evt, T* req) {
        RE::FormID oldForm;
        RE::FormID newForm = 0;
        evt->ReadRecordData(oldForm);
        evt->ResolveFormID(oldForm, newForm);
        // auto handler = RE::TESDataHandler::GetSingleton();
        // auto aForm = handler->LookupForm<RE::TESGlobal>(newForm, "Gossip.esp");
        req = RE::TESForm::LookupByID<T>(newForm);
        if (!req) {
            logger::error("Failed to retrieve a form {:x},{:x}", oldForm, newForm);
            return false;
        }

        logger::info("Retrieved form {} with FormID: {:x}", req->GetFormEditorID(), newForm);
        return true;
    }
    size_t getSize(SKSE::SerializationInterface* evt);
}  // namespace gossip