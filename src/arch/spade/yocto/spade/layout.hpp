#ifndef YOCTO_SPADE_LAYOUT_INCLUDED
#define YOCTO_SPADE_LAYOUT_INCLUDED 1

#include "yocto/spade/types.hpp"
#include "yocto/object.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        class layout : public object
        {
        public:
            //! default ctor
            /**
             \param num_dim > 0
             */
            explicit layout( size_t num_dim ) throw();
            virtual ~layout() throw();
            
            const size_t dimensions;
            
        protected:
            //! order lower/upper, compute width and return #items
            size_t   setup( const void *lower_addr, const void *upper_addr, const void *pitch_addr, const void *width_addr) const throw();
            
            //! detect is coord is inside
            bool     is_inside( const void *coord_addr, const void *lower_addr, const void *upper_addr) const throw();
            
            //! MPI style splitting for one dimension
			/**
			 \param lo final lower coordinate
			 \param hi final upper coordinate
			 \param Lo source lower coordinate
			 \param Hi source upper coordinate
			 \param rank MPI style rank, 0 <= rank < size
			 \param size MPI style size
			 */
            static void split(unit_t      &lo,
                              unit_t      &hi,
                              const unit_t Lo,
                              const unit_t Hi,
                              const size_t rank,
                              const size_t size );
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
			inline explicit layout_of( param_coord lo, param_coord hi ) throw() :
			layout( DIMENSIONS ),
			lower( lo ),
			upper( hi ),
			width( ),
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
                for( size_t dim=1; dim < DIMENSIONS; ++dim )
                {
                    ans += __coord(pitch,dim) * ( __coord(c,dim) - __coord(lower,dim) );
                }
                return ans;
            }
            
			//! destruct layout
			inline virtual ~layout_of() throw() {}
            
            //! test that a coordinate is inside
            inline bool has( param_coord c ) const throw() { return layout::is_inside( &c, &lower, &upper); }
            
            //! test that a sub layout is inside
            inline bool contains( const layout_of &sub ) const throw() { return has(sub.lower) && has(sub.upper); }
            
            //! load offsets of a sub layout of *this
            inline void load_offsets( const layout_of &sub, offsets_list &offsets ) const throw()
            {
                assert( this->contains(sub) );
                offsets.reserve( sub.items );
                __ld(sub, offsets,int2type<DIMENSIONS>());
            }
            
            //! show it
            friend inline std::ostream & operator<<( std::ostream &lay_os, const layout_of &the_layout )
            {
                lay_os << "{ " << the_layout.lower << " -> " << the_layout.upper << " : #" << the_layout.width << "= " << the_layout.items << " }";
                return lay_os;
            }
            
            inline const layout_of & as_layout() const throw() { return *this; }
            
            //! MPI style splitting along dimension dim
			inline layout_of split( size_t rank, size_t size, size_t dim = DIMENSIONS-1 ) const
			{
                assert( dim < DIMENSIONS );
				const unit_t Lo = __coord(lower,dim);
				const unit_t Hi = __coord(upper,dim);
				coord        s_lo(lower);
				coord        s_hi(upper);
				unit_t      &lo = __coord(s_lo,dim);
				unit_t      &hi = __coord(s_hi,dim);
				layout::split(lo,hi,Lo,Hi,rank,size);
				return layout_of<COORD>(s_lo,s_hi);
			}
            
            inline bool is_same_layout_than( const layout_of &other ) const throw()
            {
                const unit_t *u = & __coord(lower, 0);
                const unit_t *v = & __coord(other.lower, 0);
                for(size_t i=0;i< DIMENSIONS*2; ++i)
                {
                    if( u[i] != v[i] ) return false;
                }
                return true;
            }
            
        private:
            inline void __ld( const layout_of &sub, offsets_list &offsets, int2type<1> ) const throw()
            {
                const unit_t xmin = __coord(sub.lower,0);
                const unit_t xmax = __coord(sub.upper,0);
                for( unit_t x = xmin; x <= xmax; ++x )
                {
                    const unit_t u = offset_of(x); assert(u>=0); assert(size_t(u)<items);
                    offsets.store(u);
                }
            }
            
            inline void __ld( const layout_of &sub, offsets_list &offsets, int2type<2> ) const throw()
            {
                const unit_t ymin = __coord(sub.lower,1);
                const unit_t ymax = __coord(sub.upper,1);
                const unit_t xmin = __coord(sub.lower,0);
                const unit_t xmax = __coord(sub.upper,0);
                for( unit_t y = ymin; y <= ymax; ++y )
                {
                    for(unit_t x = xmin; x <= xmax; ++x )
                    {
                        const coord  c(x,y);
                        const unit_t u = offset_of(c); assert(u>=0); assert(size_t(u)<items);
                        offsets.store(u);
                    }
                }
                
            }
            
            inline void __ld( const layout_of &sub, offsets_list &offsets, int2type<3> ) const throw()
            {
                const unit_t zmin = __coord(sub.lower,2);
                const unit_t zmax = __coord(sub.upper,2);
                const unit_t ymin = __coord(sub.lower,1);
                const unit_t ymax = __coord(sub.upper,1);
                const unit_t xmin = __coord(sub.lower,0);
                const unit_t xmax = __coord(sub.upper,0);
                for( unit_t z = zmin; z <= zmax; ++z )
                {
                    for( unit_t y = ymin; y <= ymax; ++y )
                    {
                        for(unit_t x = xmin; x <= xmax; ++x )
                        {
                            const coord  c(x,y,z);
                            const unit_t u = offset_of(c); assert(u>=0); assert(size_t(u)<items);
                            offsets.store(u);
                        }
                    }
                }
            }
            
            YOCTO_DISABLE_ASSIGN(layout_of);
        };
        
        
    }
    
}

#endif
