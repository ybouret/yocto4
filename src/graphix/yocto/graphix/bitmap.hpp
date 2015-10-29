#ifndef YOCTO_GRAPHIX_BITMAP_INCLUDED
#define YOCTO_GRAPHIX_BITMAP_INCLUDED 1

#include "yocto/graphix/rectangle.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace graphix
    {

        class bitmap : public counted_object, public patch
        {
        public:
            typedef arc_ptr<bitmap> pointer;

            enum memory_type
            {
                memory_is_global, //!< in dynamic memory
                memory_is_shared, //!< pointing to another bitmap
                memory_from_user  //!< using local/static data
            };

            typedef void * (*xshift_proc)(void *start, unit_t dx);


            const unit_t      depth;  //!< 1,2,3,4,8 or 16
            const unit_t      w;      //!< width.x
            const unit_t      h;      //!< width.y
            const unit_t      pitch;  //!< w*d, length of ascanline in bytes
            const unit_t      stride; //!< bytes to get to next line, stride>=pitch
            void             *entry;  //!< address or first item
            const memory_type model;
            xshift_proc       xshift;
            
            //! allocate a bitmap
            explicit bitmap(const unit_t D, const unit_t W, const unit_t H);

            //! hard copy a bitmap
            bitmap(const bitmap &bmp, const patch *area);

            //! soft copy, shared data
            bitmap(const bitmap::pointer &bmp, const patch *area);

            //! map usr data
            bitmap(void *data, const unit_t D, const unit_t W, const unit_t H, const unit_t S );


            //! clean memory
            virtual ~bitmap() throw();
            

            static unit_t check_d(unit_t);
            static unit_t check_w(unit_t);
            static unit_t check_h(unit_t);

            //! address of first element of line 0<=j<h
            void       *get_line(const unit_t j) throw();

            //! address of first element of line 0<=j<h
            const void *get_line(const unit_t j) const throw();

            //! get address of item i,j
            void       *get(const unit_t i,const unit_t j) throw();

            //! get address of item i,j
            const void *get(const unit_t i,const unit_t j) const throw();

            void ldz() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(bitmap);
            union
            {
                size_t  allocated; //!< for global data
                bitmap *shared;    //!< for shared bitmap
            };
            
        };

    }
}

#endif

