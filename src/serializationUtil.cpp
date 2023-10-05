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
    RE::TESForm* readForm(SKSE::SerializationInterface* evt) {
        RE::FormID oldForm;
        RE::FormID newForm = 0;
        evt->ReadRecordData(oldForm);
        evt->ResolveFormID(oldForm, newForm);
        //auto handler = RE::TESDataHandler::GetSingleton();
        //auto aForm = handler->LookupForm<RE::TESGlobal>(newForm, "Gossip.esp");
        RE::TESForm* aForm = RE::TESForm::LookupByID(newForm);
        if (!aForm) {
            logger::error("Failed to retrieve a form {:x},{:x}", oldForm, newForm);
            return nullptr;
        }
        logger::info("Retrieved form {} with FormID: {:x}", aForm->GetFormEditorID(), newForm);
        return aForm;
    };
    size_t getSize(SKSE::SerializationInterface* evt) {
        size_t size;
        evt->ReadRecordData(size);
        return size;
    }
}