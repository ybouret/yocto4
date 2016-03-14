#ifndef YOCTO_GRAPHICS_QUADTREE_INCLUDED
#define YOCTO_GRAPHICS_QUADTREE_INCLUDED 1

#include "yocto/graphics/rectangle.hpp"

namespace yocto
{
    namespace graphics
    {

        class Quad : public rectangle
        {
        public:
            static const size_t NSUB = 4;
            explicit Quad(const unit_t X, const unit_t Y, const unit_t W, const unit_t H, const int Level);
            virtual ~Quad() throw();

            Quad     *parent;
            Quad     *sub[NSUB];
            const int level;

            void  subdivide();

            YOCTO_MAKE_OBJECT
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Quad);
        };

    }

}

#endif
