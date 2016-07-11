#ifndef YOCTO_GRAPHICS_OPS_STENCIL_INCLUDED
#define YOCTO_GRAPHICS_OPS_STENCIL_INCLUDED 1

#include "yocto/graphics/pixmap.hpp"
#include "yocto/graphics/xpatch.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace graphics
    {

        class stencil
        {
        public:
            class row
            {
            public:
                row(float *entry)  throw();
                float &      operator[](const unit_t dx) throw();
                const float &operator[](const unit_t dx) const throw();
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(row);
                ~row() throw();
                float *addr;
            };

            stencil() throw();
            ~stencil() throw();
            stencil(const stencil &) throw();
            stencil & operator=(const stencil &) throw();

            row       & operator[](const unit_t dy) throw();
            const row & operator[](const unit_t dy) const throw();



            friend  std::ostream & operator<<( std::ostream &os, const stencil &S );

        private:
            row     *rows;
            float    v[9];
            uint64_t wksp[ YOCTO_U64_FOR_SIZE( 3*sizeof(row) ) ];
            void setup() throw();

        };

    }
}

#endif
