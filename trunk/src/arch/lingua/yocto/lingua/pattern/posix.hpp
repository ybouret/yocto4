#ifndef YOCTO_LINGUA_PATTERN_POSIX_INCLUDED
#define YOCTO_LINGUA_PATTERN_POSIX_INCLUDED 1

#include "yocto/lingua/pattern.hpp"

namespace yocto
{
    namespace lingua
    {
        struct posix
		{
            
            //------------------------------------------------------------------
			// standard
            //------------------------------------------------------------------
			static pattern *upper();  //!< [A-Z]
			static pattern *lower();  //!< [a-z]
			static pattern *digit();  //!< [0-9]
			static pattern *alpha();  //!< [A-Za-z]
			static pattern *alnum();  //!< [0-9A-Za-z]
			static pattern *xdigit(); //!< [0-9A-Fa-f]
			static pattern *blank();  //!< [ \t]
			static pattern *space();  //!< [ \t\r\n\v\f]
			static pattern *punct();  //!< !"#$%&'()*+,\-./:;<=>?@[\\\]^_`{|}~
			
            //------------------------------------------------------------------
			// not standard
            //------------------------------------------------------------------
			static pattern *word();    //!< [A-Za-z0-9_]
			static pattern *endl();    //!< \n|\r|\r\n
			static pattern *dot();     //!< [^\n\r]
			static pattern *cstring(); //!< ISO C string
		};

    }
}

#endif
