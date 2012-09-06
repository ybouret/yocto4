/**
 \file
 \brief layout of logical indices
 */

#ifndef YOCTO_SWAMP_LAYOUT_INCLUDED
#define YOCTO_SWAMP_LAYOUT_INCLUDED 1


#include "yocto/swamp/types.hpp"
#include <iostream>

namespace yocto 
{
    
    namespace swamp
    {
        
        //! base layout, operations on coordinates
        class layout_base : public object
        {
        public:
            virtual ~layout_base() throw();
            
            const size_t dimensions;
            
        protected:
            explicit layout_base(const size_t num_dims) throw();
            
            //! order lower/upper, compute width and return #items
            size_t   setup( const void *lower_addr, const void *upper_addr, const void *pitch_addr, const void *width_addr) const throw();
            
            //! detect if coord is inside
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
            YOCTO_DISABLE_COPY_AND_ASSIGN(layout_base);
        };
        
        //! describe indices, no memory is allocated
        template <typename COORD>
        class layout : public layout_base
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
			inline explicit layout( param_coord lo, param_coord hi ) throw() :
			layout_base( DIMENSIONS ),
			lower( lo ),
			upper( hi ),
			width( ),
            pitch(),
			items( layout_base::setup( &lower, &upper, &pitch, &width ) )
			{
				
			}
			
			//! copy layout
			inline layout( const layout &other ) throw() :
			layout_base( DIMENSIONS ),
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
			inline virtual ~layout() throw() {}
            
            //! test that a coordinate is inside
            inline bool has( param_coord c ) const throw() { return layout_base::is_inside( &c, &lower, &upper); }
            
            //! test that a sub layout is inside
            inline bool contains( const layout &sub ) const throw() { return has(sub.lower) && has(sub.upper); }
            
            //! load offsets of a sub layout in this
            inline void load_offsets( const layout &sub, offsets_list &offsets ) const throw()
            {
                assert( this->contains(sub) );
                offsets.reserve( sub.items );
                __ld(sub, offsets,int2type<DIMENSIONS>());
            }
            
            friend inline std::ostream & operator<<( std::ostream &lay_os, const layout &the_layout )
            {
                lay_os << "{ " << the_layout.lower << " -> " << the_layout.upper << " : #" << the_layout.width << "= " << the_layout.items << " }"; 
                return lay_os;
            }
            
            inline const layout & __layout() const throw() { return *this; }
            
            //! MPI style splitting along dimension dim
			inline layout split( size_t rank, size_t size, size_t dim = DIMENSIONS-1 ) const
			{
                assert( dim < DIMENSIONS );
				const unit_t Lo = __coord(lower,dim);
				const unit_t Hi = __coord(upper,dim);
				coord        s_lo(lower);
				coord        s_hi(upper);
				unit_t      &lo = __coord(s_lo,dim);
				unit_t      &hi = __coord(s_hi,dim);
				layout_base::split(lo,hi,Lo,Hi,rank,size);
				return layout(s_lo,s_hi);
			}

            
        private:
            inline void __ld( const layout &sub, offsets_list &offsets, int2type<1> ) const throw()
            {
                const unit_t xmin = __coord(sub.lower,0);
                const unit_t xmax = __coord(sub.upper,0);
                for( unit_t x = xmin; x <= xmax; ++x )
                {
                    const unit_t u = offset_of(x); assert(u>=0); assert(size_t(u)<items);
                    offsets.store(u);
                }
            }
            
            inline void __ld( const layout &sub, offsets_list &offsets, int2type<2> ) const throw()
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
            
            inline void __ld( const layout &sub, offsets_list &offsets, int2type<3> ) const throw()
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
            
            
            YOCTO_DISABLE_ASSIGN(layout);
        };
        
        
    }
    
}

#endif
