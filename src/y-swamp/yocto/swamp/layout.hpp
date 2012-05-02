#ifndef YOCTO_SWAMP_LAYOUT_INCLUDED
#define YOCTO_SWAMP_LAYOUT_INCLUDED 1


#include "yocto/swamp/types.hpp"
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
            size_t   setup( const void *lower_addr, const void *upper_addr, const void *width_addr) const throw();
            
            //! detect is coord is inside
            bool     is_inside( const void *coord_addr, const void *lower_addr, const void *upper_addr) const throw();
                               
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
			items( layout_base::setup( &lower, &upper, &width ) )
			{
				
			}
			
			//! copy layout
			inline layout( const layout &other ) throw() :
			layout_base( DIMENSIONS ),
			lower( other.lower ),
			upper( other.upper ),
			width( other.width ),
			items( other.items )
			{
			}
			
			//! destruct layout
			inline virtual ~layout() throw() {}

            //! test that a coordinate is inside
            inline bool has( param_coord c ) const throw() { return layout_base::is_inside( &c, &lower, &upper); }
            
            //! test that a sub layout is inside
            inline bool contains( const layout &sub ) const throw() { return has(sub.lower) && has(sub.upper); }
            
        private:
            YOCTO_DISABLE_ASSIGN(layout);
        };
        
        
    }
    
}

#endif
