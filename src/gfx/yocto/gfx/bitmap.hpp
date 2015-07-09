#ifndef YOCTO_GFX_BITMAP_INCLUDED
#define YOCTO_GFX_BITMAP_INCLUDED 1

#include "yocto/gfx/rectangle.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"

namespace yocto
{
    namespace gfx
    {


        //! handling linear memory
        /**
         for items of size 1,2,3,4,8,16
         */
        class bitmap : public counted_object
        {
        public:
            typedef arc_ptr<bitmap> pointer;

            enum memory_type
            {
                memory_is_global, //!< in dynamic memory
                memory_is_shared, //!< pointing to another bitmap
                memory_from_user  //!< using local/static data
            };

            //! way to get pixel dx from start
            typedef void * (*peek_proc)(void *start, const unit_t dx);

            //! way to exchange memory
            typedef void   (*swap_proc)(void *dst, void *src);

            const unit_t      d;      //!< depth in bytes: 1,2,3,4,8,16
            const unit_t      w;      //!< width
            const unit_t      h;      //!< height
            const unit_t      pitch;  //!< w*d, size of a scan line
            const unit_t      stride; //!< bytes to move one line up/down, stride>=pitch
            void             *entry;  //!< address of the (0,0) item
            const peek_proc   peek;   //!< cabled function
            const memory_type model;
            const swap_proc   swap;
            
            //! create an allocated bitmap
            explicit bitmap(const unit_t D, const unit_t W, const unit_t H);

            //! create a bitmap from user data
            explicit bitmap(const unit_t D,
                            const unit_t W,
                            const unit_t H,
                            void        *data,
                            const unit_t S);

            //! hard copy of a full/partial other bitmap
            explicit bitmap(const bitmap &other, const rectangle *rect);

            //! linking to a full/partial other bitmap
            explicit bitmap(bitmap::pointer &bmp, const rectangle *rect);


            virtual ~bitmap() throw();
            
            static unit_t check_depth( const unit_t); //!< sanity check
            static unit_t check_width( const unit_t); //!< sanity check
            static unit_t check_height(const unit_t); //!< sanity check

            inline bool contains( const rectangle &rect ) const throw()
            {
                return
                (rect.x>=0)    &&
                (rect.y>=0)    &&
                (rect.xout<=w) &&
                (rect.yout<=h);
            }

            //! get line address, no check
            inline  void *get_line(unit_t y) throw()
            {
                return static_cast<uint8_t*>(entry)+(y*stride);
            }

            //! get line address, no check
            inline  const void *get_line(unit_t y) const throw()
            {
                return static_cast<const uint8_t*>(entry)+(y*stride);
            }

            //! get item from line, no check
            inline void *get_item(void *line,unit_t x) throw()
            {
                return peek(line,x);
            }

            //! get item from line, no check
            inline const void *get_item(void *line,unit_t x) const throw()
            {
                return peek((void*)line,x);
            }

            //! get item address
            inline void *get(const unit_t x,const unit_t y) throw()
            {
                return peek(get_line(y),x);
            }

            //! get item address
            inline const void *get(const unit_t x,const unit_t y) const throw()
            {
                return peek((void*)get_line(y),x);
            }

            void ldz() throw();
            void flip_vert() throw();


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(bitmap);
            union
            {
                size_t  allocated;
                bitmap *shared;
            };


        };

    }

}

#endif

