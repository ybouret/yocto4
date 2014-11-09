#ifndef YOCTO_FAME_CURV_INCLUDED
#define YOCTO_FAME_CURV_INCLUDED 1

#include "yocto/fame/array1d.hpp"

namespace yocto
{
    namespace fame
    {
        
        //______________________________________________________________________
        //
        // ! VisIt oriented
        //______________________________________________________________________
        class CurvInfo
        {
        public:
            explicit CurvInfo(const string &id);
            
            virtual ~CurvInfo() throw();
            
            const string name;
            string       XLabel;
            string       YLabel;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(CurvInfo);
        };
    }
}

#endif
