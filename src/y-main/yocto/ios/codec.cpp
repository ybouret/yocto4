#include "yocto/ios/codec.hpp"

namespace yocto
{
    namespace ios
    {
        codec:: ~codec() throw() {}
        codec::  codec() throw() {}
        
        string codec:: to_string()
        {
            string ans;
            char   C=0;
            while( query(C) ) ans.append(C);
            return ans;
        }
    }
}
