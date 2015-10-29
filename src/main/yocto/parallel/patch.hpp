#ifndef YOCTO_PARALLEL_PATCH_INCLUDED
#define YOCTO_PARALLEL_PATCH_INCLUDED 1

#include "yocto/parallel/coord.hpp"
#include "yocto/type/traits.hpp"
#include <iostream>

namespace yocto
{
    namespace parallel
    {

        //! a cartesian patch, base class
        class patch_info
        {
        public:
            virtual ~patch_info() throw();
            const size_t dim;     //!< 1 | 2 | 3


        protected:
            explicit patch_info(const size_t d) throw();
            patch_info(const patch_info &) throw();
            
            size_t setup(const void  *lower,
                         const void  *upper,
                         const void  *pitch,
                         const void  *width) throw();

        private:
            YOCTO_DISABLE_ASSIGN(patch_info);
        };



        template <typename COORD>
        class patch_of : public patch_info
        {
        public:
            typedef COORD                                         coord;         //!< POD coordinate
            typedef const     COORD                               const_coord;   //!< const coordinate
            typedef typename  type_traits<COORD>::parameter_type  param_coord;   //!< coordinate as parameter

            //! #COORD = 1|2|3
            static const size_t DIM = sizeof(COORD)/sizeof(unit_t);

            const_coord   lower; //!< lower coordinate
            const_coord   upper; //!< upper coordinate
            const_coord   width; //!< coordinate width
            const_coord   pitch; //!< coordinate pitch: (1,nx,nx*ny)
            const size_t  items; //!< number of linear items within the layout


            //! prepare patch
            /**
             \param lo lower coordinates
             \param up upper coordinates
             */
            inline  patch_of(const_coord lo,
                             const_coord hi ) throw() :
            patch_info( DIM ),
            lower( lo ),
            upper( hi ),
            width(),
            pitch(),
            items( patch_info::setup( &lower, &upper, &pitch, &width ) )
            {

            }

            

            //! copy patch_of
            inline patch_of( const patch_of &other ) throw() :
            patch_info( other       ),
            lower( other.lower ),
            upper( other.upper ),
            width( other.width ),
            pitch( other.pitch ),
            items( other.items )
            {
            }

            inline unit_t offset_of( param_coord c ) const throw()
            {
                unit_t ans = __coord(c,0) - __coord(lower,0);
                for( size_t i=1; i < DIM; ++i )
                    ans += __coord(pitch,i) * ( __coord(c,i) - __coord(lower,i) );
                return ans;
            }

            //! destruct patch
            inline virtual ~patch_of() throw() {}

            //! show it
            friend inline std::ostream & operator<<( std::ostream &lay_os, const patch_of &p )
            {
                lay_os << "{ " << p.lower << " -> " << p.upper << " : width=" << p.width << " => #" << p.items << " pitch=" << p.pitch << " }";
                return lay_os;
            }

            //! comparison
            inline bool same_metrics_than( const patch_of &other ) const throw()
            {
                return are_same_coord(lower,other.lower) && are_same_coord(upper,other.upper);
            }

            //! contains coord ?
            inline bool has( param_coord c ) const throw()
            {
                for(size_t dim=0;dim<DIM;++dim)
                {
                    const unit_t u = __coord(c,dim);
                    if( u < __coord(lower,dim) || __coord(upper,dim) < u )
                        return false;
                }
                return true;
            }

            //! contains layout ?
            inline bool contains( const patch_of &sub ) const throw()
            {
                return has(sub.lower) && has(sub.upper);
            }



        private:
            YOCTO_DISABLE_ASSIGN(patch_of);
        };

        typedef patch_of<coord1D> patch1D;
        typedef patch_of<coord2D> patch2D;
        typedef patch_of<coord3D> patch3D;

        
    }

}

#endif

