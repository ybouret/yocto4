#ifndef YOCTO_GRAPHICS_REGION_INCLUDED
#define YOCTO_GRAPHICS_REGION_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/code/static-check.hpp"

namespace yocto
{
    namespace graphics
    {

        class regxel
        {
        public:
            vertex  r;
            regxel *next;
            regxel *prev;
            YOCTO_MAKE_OBJECT

            regxel() throw() : r(), next(0), prev(0) {}
            regxel(const vertex &pos) throw() : r(pos), next(0), prev(0) {}
            
            ~regxel() throw() {}

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(regxel);

        };

        typedef core::list_of_cpp<regxel> regxels;

        //! a simple region
        class region : public regxels
        {
        public:
            explicit region() throw();
            virtual ~region() throw();
            region(const region &);
            
        private:
            YOCTO_DISABLE_ASSIGN(region);
        };

        typedef vector<region> regions;
        
        


    }
    
}

#endif

