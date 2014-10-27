#ifndef YOCTO_FAME_CURV_INCLUDED
#define YOCTO_FAME_CURV_INCLUDED 1

#include "yocto/fame/array1d.hpp"

namespace yocto
{
    namespace fame
    {
        
        
        class CurvInfo
        {
        public:
            explicit CurvInfo(const string &id);
            
            virtual ~CurvInfo() throw();
            
            const string name;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(CurvInfo);
        };
    }
}

#endif
