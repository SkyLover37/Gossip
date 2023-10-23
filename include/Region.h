
#include <Scandal.h>
namespace gossip {


    class region : valueData<int>, fameMap {
    public:
        typedef fameMap fameMap;
        typedef valueData<int> interest;
        RE::BGSLocation* tLoc;

        region(SKSE::SerializationInterface* evt);
        void operator()(SKSE::SerializationInterface* evt);
        fameData* operator[](fameInfo* info) { return (*this)[info];
        }
    };
    using regionMap = std::map<RE::BGSLocation*, region>;
}  // namespace gossip