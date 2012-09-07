#ifndef YOCTO_SPADE_GHOST_INCLUDED
#define YOCTO_SPADE_GHOST_INCLUDED 1

#include "yocto/spade/layout.hpp"

namespace yocto
{
    namespace spade
    {
        
        //! a ghost is a localized list of offsets...
        class ghost : public offsets_list
        {
        public:
            enum position
            {
                at_lower_x = 0,
                at_upper_x = 1,
                at_lower_y = 2,
                at_upper_y = 3,
                at_lower_z = 4,
                at_upper_z = 5
            };
            
            const position site;    //!< ghost position
            
            static const char     *get_position_name( ghost::position p ) throw();
            static ghost::position get_mirror_position( ghost::position p ) throw();
            static ghost::position get_lower_position( size_t dim );
            static ghost::position get_upper_position( size_t dim );
            
            virtual ~ghost() throw();
            
                       
            //! ctor: set ghost position
            explicit ghost( ghost::position p ) throw();
            
            //! get_position_name(site)
            const char *position_name() const throw();
            
            //! get mirror position if needed
            ghost::position mirror_position() const throw();

            //! build the corresponding inner sublayout for num_ghosts
            template <typename LAYOUT>
            inline LAYOUT inner_sublayout( const LAYOUT &L, size_t num_ghosts )
            {
                assert(num_ghosts>0);
                const size_t dim   = site>>1;
                switch( site )
                {
                    case at_lower_x:
                    case at_lower_y:
                    case at_lower_z:
                    {
                        check(dim, __coord(L.width,dim), num_ghosts);
                        typename LAYOUT::coord lower = L.lower;
                        typename LAYOUT::coord upper = lower;
                        __coord(upper,dim) += num_ghosts-1;
                        return LAYOUT(lower,upper);
                    }
                    
                    case at_upper_x:
                    case at_upper_y:
                    case at_upper_z:
                    {
                        typename LAYOUT::coord upper = L.upper;
                        typename LAYOUT::coord lower = upper;
                        __coord(upper,dim) -= (num_ghosts-1);
                        return LAYOUT(lower,upper);
                    }
                }
            }
            
            //! build the corrsponding outer sublayout for num_ghosts
            template <typename LAYOUT>
            inline LAYOUT outer_sublayout( const LAYOUT &L, size_t num_ghosts )
            {
                assert(num_ghosts>0);
                const size_t dim   = site>>1;
                switch( site )
                {
                    case at_lower_x:
                    case at_lower_y:
                    case at_lower_z:
                    {
                        typename LAYOUT::coord lower = L.lower;
                        typename LAYOUT::coord upper = lower;
                        __coord(upper,dim) -= 1;
                        __coord(lower,dim) -= num_ghosts;
                        return LAYOUT(lower,upper);
                    }
                        
                    case at_upper_x:
                    case at_upper_y:
                    case at_upper_z:
                    {
                        typename LAYOUT::coord upper = L.upper;
                        typename LAYOUT::coord lower = upper;
                        __coord(lower,dim) += 1;
                        __coord(upper,dim) += num_ghosts;
                        return LAYOUT(lower,upper);
                    }
                }
            }

            template <typename LAYOUT>
            inline void load_from( const LAYOUT &outline, const LAYOUT &L )
            {
                outline.load_offsets(L,*this);
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
            static void check( size_t dim, unit_t w, size_t ng);
        };
        
        class ghosts : public object
        {
        public:
            const size_t num_offsets; //!< to be set by setup
            
            virtual ~ghosts() throw();
            
        protected:
            explicit ghosts() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts);
        };
        
    }
}

#endif
