#ifndef YOCTO_GRAPHIX_BITMAP_INCLUDED
#define YOCTO_GRAPHIX_BITMAP_INCLUDED 1

#include "yocto/graphics/rectangle.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace graphics
    {


        //______________________________________________________________________
        //
        // linear memory for some fixed-length types
        //______________________________________________________________________
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

            enum position_type
            {
                at_lower, //!< x,y <=0
                is_inner, //!< 0<x,y<w-1,h-1
                at_upper  //!< x,y >= w-1,h-1
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
            bitmap(const bitmap &bmp);

            //! partial hard copy
            bitmap(const bitmap &bmp, const patch &area);

            //! full shared copy
            bitmap(bitmap *bmp) throw();

            //! shared copy
            bitmap(bitmap *bmp, const patch &area);

            //! map user data
            bitmap(void        *data,
                   const unit_t D,
                   const unit_t W,
                   const unit_t H,
                   const unit_t S );


            //! clean memory
            virtual ~bitmap() throw();
            
            //! check valid depth, throw on error
            static unit_t check_d(unit_t);

            //! check valid width, throw on error
            static unit_t check_w(unit_t);

            //! check valud height, throw on error
            static unit_t check_h(unit_t);

            //! address of first element of line 0<=j<h
            void       *get_line(const unit_t j) throw();

            //! address of first element of line 0<=j<h
            const void *get_line(const unit_t j) const throw();

            //! get address of item i,j
            void        *get(const unit_t i,const unit_t j) throw();

            //! wrapper to get address from vertex
            inline void *get(const vertex &v) throw() { return get(v.x,v.y); }

            //! get address of item i,j, CONST
            const void *get(const unit_t i,const unit_t j) const throw();

            //! wrapper to get address from vertex, CONST
            inline const void *get(const vertex &v) const throw() { return get(v.x,v.y); }

            //! zero memory
            void ldz() throw();

            void copy(const bitmap &bmp) throw(); //!< assume same sizes

            inline position_type x_position(const unit_t x) const throw()
            {
                return (x<=lower.x) ?  at_lower : ( (x>=upper.x) ? at_upper : is_inner );
            }

            inline position_type y_position(const unit_t y) const throw()
            {
                return (y<=lower.y) ?  at_lower : ( (y>=upper.y) ? at_upper : is_inner );
            }

            //! save as uncompressed bitmap
            /**
             assuming RGB of RGBA !!!
             - .bmp
             - .ppm
             - .eps
             */
            void save(const string &filename,
                      const bool    in_color = true ) const;


        private:
            YOCTO_DISABLE_ASSIGN(bitmap);
            union
            {
                size_t  allocated; //!< for global data
                bitmap *shared;    //!< for shared bitmap
            };
            
        };

    }
}

#endif

