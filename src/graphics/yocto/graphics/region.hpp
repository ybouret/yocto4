#ifndef YOCTO_GRAPHICS_REGION_INCLUDED
#define YOCTO_GRAPHICS_REGION_INCLUDED 1

#include "yocto/graphics/types.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    namespace graphics
    {
        class displacement
        {
        public:
            vertex r;
            unit_t flag;
            char   data[5*sizeof(unit_t)];
            
            displacement() throw();
            displacement(const vertex &p) throw();
            ~displacement() throw();

            displacement(const displacement &) throw();
            displacement & operator=(const displacement &) throw();
            void clear() throw();

            template <typename T>
            T & get() throw()
            {
                return *(T *)&data[0];
            }

            template <typename T>
            const T & get() const throw()
            {
                return *(T *)&data[0];
            }


        };

        typedef list<displacement> region_base;

        class region : public region_base
        {
        public:
            explicit region() throw();
            virtual ~region() throw();
            region(const region &);
            region&operator=(const region &);

            void load_square(const unit_t w);
            void load_disk(const unit_t r);
        };

    }
}

#endif
