#pragma once

namespace gossip {
    class UpdateHook {
        SINGLETONHEADER(UpdateHook)
    public:
        void Setup();

    private:
        bool _hooked = false;
        
        static void Update(RE::Actor* a_actor, float a_delta);
        static void UpdateFrame();
        inline static REL::Relocation<decltype(Update)> Update_old;
        inline static REL::Relocation<decltype(UpdateFrame)> onFrame_old;
    };
}  // namespace DeviousDevices