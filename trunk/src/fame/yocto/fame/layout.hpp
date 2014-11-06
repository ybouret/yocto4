#ifndef YOCTO_FAME_LAYOUT_INCLUDED
#define YOCTO_FAME_LAYOUT_INCLUDED 1

#include "yocto/fame/types.hpp"
#include <iostream>

namespace yocto
{
    
    namespace fame
    {
        
        class layout : public object
        {
        public:
            virtual ~layout() throw();
            const size_t dimensions;     //!< 1 | 2 | 3
            
            
        protected:
            explicit layout(const size_t dim) throw();
            
            size_t setup(const void  *lower,
                         const void  *upper,
                         const void  *pitch,
                         const void  *width) throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(layout);
        };
        
        template <typename COORD>
        class layout_of : public layout
        {
        public:
            typedef COORD                                         coord;         //!< POD coordinate
            typedef const     COORD                               const_coord;   //!< const coordinate
            typedef typename  type_traits<COORD>::parameter_type  param_coord;   //!< coordinate as parameter
            
            //! #COORD = 1|2|3
            static const size_t DIMENSIONS = sizeof(COORD)/sizeof(unit_t);
            
            const_coord   lower; //!< lower coordinate
            const_coord   upper; //!< upper coordinate
            const_coord   width; //!< coordinate width
            const_coord   pitch; //!< coordinate pitch: (1,nx,nx*ny)
            const size_t  items; //!< number of linear items within the layout
            
            
            //! prepare layout
            /**
             \param lo lower coordinates
             \param up upper coordinates
             The coordinates are ordered.
             */
            inline  layout_of( param_coord lo, param_coord hi ) throw() :
            layout( DIMENSIONS ),
            lower( lo ),
            upper( hi ),
            width(),
            pitch(),
            items( layout::setup( &lower, &upper, &pitch, &width ) )
            {
                
            }
            
            //! copy layout
            inline layout_of( const layout_of &other ) throw() :
            layout( DIMENSIONS ),
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
                for( size_t i=1; i < DIMENSIONS; ++i )
                    ans += __coord(pitch,i) * ( __coord(c,i) - __coord(lower,i) );
                return ans;
            }
            
            //! destruct layout
            inline virtual ~layout_of() throw() {}
            
            //! show it
            friend inline std::ostream & operator<<( std::ostream &lay_os, const layout_of &the_layout )
            {
                lay_os << "{ " << the_layout.lower << " -> " << the_layout.upper << " : #" << the_layout.width << "= " << the_layout.items << " }";
                return lay_os;
            }
            
            //! comparison
            inline bool same_layout_than( const layout_of &other ) const throw()
            {
                return are_same_coord(lower,other.lower) && are_same_coord(upper,other.upper);
            }
            
            //! contains coord ?
            inline bool has( param_coord c ) const throw()
            {
                for(size_t dim=0;dim<DIMENSIONS;++dim)
                {
                    const unit_t u = __coord(c,dim);
                    if( u < __coord(lower,dim) || __coord(upper,dim) < u )
                        return false;
                }
                return true;
            }
            
            //! contains layout ?
            inline bool contains( const layout_of &sub ) const throw()
            {
                return has(sub.lower) && has(sub.upper);
            }
            
            //! helper
            inline const layout_of & __layout() const throw() { return *this; }
            
            void store_offsets_of( const layout_of &sub, offsets_list &offsets ) const
            {
                assert(this->contains(sub));
                
            }
            
        private:
            YOCTO_DISABLE_ASSIGN(layout_of);
        };
        
        typedef layout_of<coord1D> layout1D;
        typedef layout_of<coord2D> layout2D;
        typedef layout_of<coord3D> layout3D;
        
        template <size_t DIM>
        struct layout_for;
        
        template <> struct layout_for<1> { typedef layout1D type; };
        template <> struct layout_for<2> { typedef layout2D type; };
        template <> struct layout_for<3> { typedef layout3D type; };

        
    }
}

#endif

