#ifndef YOCTO_GRAPHIX_RGBA2DATA_INCLUDED
#define YOCTO_GRAPHIX_RGBA2DATA_INCLUDED 1

#include "yocto/graphix/rgb.hpp"

namespace yocto
{
    namespace graphix
    {

        class rgba2data : public object
        {
        public:
            virtual ~rgba2data() throw();

            void operator()(void *addr, const RGBA &c);

        protected:
            explicit rgba2data() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(rgba2data);
            virtual void put(void *addr,const RGBA &c) = 0;
        };

#define YOCTO_GRAPHIX_RGBA2DATA(CLASS,CODE) \
class CLASS : public rgba2data {       \
public:\
inline explicit CLASS() throw() : rgba2data() {}\
inline virtual ~CLASS() throw() {} \
\
private:\
YOCTO_DISABLE_COPY_AND_ASSIGN(CLASS);\
virtual void put(void *addr,const RGBA &C) { CODE; }\
}

        //! for pixmap4
        YOCTO_GRAPHIX_RGBA2DATA(put_rgba,*(RGBA *)addr=C);

    }
}

#endif
