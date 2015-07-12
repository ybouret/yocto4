#ifndef YOCTO_GFX_PIXMAPS_INCLUDED
#define YOCTO_GFX_PIXMAPS_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"

namespace yocto
{
    namespace gfx
    {
        //! stack of 0..size-1 pixmaps, same dimensions
        template <typename T>
        class pixmaps
        {
        public:
            const size_t size; //!< #pixmaps
            const unit_t w;
            const unit_t h;
            
            inline pixmap<T> & operator[](size_t n) throw()
            {
                assert(n<size);
                assert(size==built);
                return pxm[n];
            }
            
            
            inline const pixmap<T> & operator[](size_t n) const throw()
            {
                assert(n<size);
                assert(size==built);
                return pxm[n];
            }
            
            explicit pixmaps(size_t n) :
            size(n>0?n:1),
            w(0),
            h(0),
            npx(size),
            pxm( memory::kind<memory::global>::acquire_as< pixmap<T> >(npx) ),
            built(0)
            {
                
            }
            
            explicit pixmaps(size_t n, const unit_t W, const unit_t H) :
            size(n>0?n:1),
            w(0),
            h(0),
            npx(size),
            pxm( memory::kind<memory::global>::acquire_as< pixmap<T> >(npx) ),
            built(0)
            {
                try
                {
                    build_pxm(W, H);
                }
                catch(...)
                {
                    kill_mem();
                    throw;
                }
            }
            
            //! erase all pixmaps, rebuild
            void create(const unit_t W, const unit_t H)
            {
                kill_pxm();
                build_pxm(W,H);
            }
            
            virtual ~pixmaps() throw()
            {
                kill();
            }
            
        private:
            size_t     npx;
            pixmap<T> *pxm;
            size_t     built; //!< for building
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(pixmaps);
            
            inline void build_pxm(const unit_t W, const unit_t H)
            {
                assert(0==built);
                try
                {
                    while(built<size)
                    {
                        new (pxm+built) pixmap<T>(W,H);
                        ++built;
                    }
                    (unit_t &)w = W;
                    (unit_t &)h = H;
                }
                catch(...)
                {
                    kill_pxm();
                    throw;
                }

            }
            
            inline void kill_pxm() throw()
            {
                (unit_t &)w = 0;
                (unit_t &)h = 0;
                while(built>0)
                {
                    pxm[--built].~pixmap<T>();
                }
            }
            
            inline void kill_mem() throw()
            {
                memory::kind<memory::global>::release_as< pixmap<T> >(pxm, npx);
            }
            
            inline void kill() throw()
            {
                kill_pxm();
                kill_mem();
            }
        };
    }
}

#endif

