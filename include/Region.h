#pragma once

#include <Scandal.h>
namespace gossip {

    class fame : public valueData<short> {
    public:   
        typedef valueData<short> super;

        //Set as class as I plan to add more
        class : public valueData<long long> {
        public:
            typedef valueData<long long> super;
            void load(SKSE::SerializationInterface* evt) { super(evt);
            }
        } local;
        
        fameInfo* info;

    
        fame(fameInfo* tmpInfo) : info(tmpInfo){};
        fame(SKSE::SerializationInterface* evt);
    };

    using fameMap = std::map<fameInfo*, fame>;
    using interest = valueData<int>;
    class region : public interest , public fameMap {
    public:
        typedef valueData<int> super;
        RE::BGSLocation* tLoc;

    
        region(){};
        region(SKSE::SerializationInterface* evt, infoMap& info);
        void save(SKSE::SerializationInterface* evt);
    };
    using regionMap = std::map<RE::BGSLocation*, region>;
}  // namespace gossip