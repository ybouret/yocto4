#ifndef YOCTO_GRAPHICS_REGION_INCLUDED
#define YOCTO_GRAPHICS_REGION_INCLUDED 1

#include "yocto/graphics/types.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    namespace graphics
    {
        typedef list<vertex> _region;

        class region : public _region
        {
        public:
            typedef _region::node_type node_type;

            explicit region() throw() : _region() {}
            explicit region(const size_t n) : _region(n,as_capacity) {}
            virtual ~region() throw() {}
            region(const region &r) : _region(r) {}

            inline region & operator=( const region &r )
            {
                _region tmp(r);
                swap_with(tmp);
                return *this;
            }

            void simplify() throw();
            const node_type *head() const throw() { return list_.head; }
            inline void shift(const vertex &p) throw();

            void load_block(const vertex &org, unit_t w);
            void load_disk(const vertex &org, unit_t r);


        };

    }
}

#endif
