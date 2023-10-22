namespace gossip {
    void writeString(SKSE::SerializationInterface* evt, std::string string) {
        std::size_t size = string.length() + 1;
        evt->WriteRecordData(size);
        evt->WriteRecordData(string.data(), static_cast<std::uint32_t>(size));
    }
    std::string readString(SKSE::SerializationInterface* evt) {
        std::string name;
        std::size_t strings;
        evt->ReadRecordData(strings);
        name.reserve(strings);
        evt->ReadRecordData(name.data(), static_cast<std::uint32_t>(strings));
        return name;
    }
    template <class T>
    bool readForm(SKSE::SerializationInterface* evt, T* req) {
        RE::FormID oldForm;
        RE::FormID newForm = 0;
        evt->ReadRecordData(oldForm);
        evt->ResolveFormID(oldForm, newForm);
        //auto handler = RE::TESDataHandler::GetSingleton();
        //auto aForm = handler->LookupForm<RE::TESGlobal>(newForm, "Gossip.esp");
        T*  = RE::TESForm::LookupByID<T>(newForm);
        if (!aForm) {
            logger::error("Failed to retrieve a form {:x},{:x}", oldForm, newForm);
            return false;
        }
        
        logger::info("Retrieved form {} with FormID: {:x}", req->GetFormEditorID(), newForm);
        return true;
    };
    size_t getSize(SKSE::SerializationInterface* evt) {
        size_t size;
        evt->ReadRecordData(size);
        return size;
    }
}