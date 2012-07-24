#include "yocto/swamp/ghosts.hpp"
#include "yocto/error.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // ghost
        //
        ////////////////////////////////////////////////////////////////////////
        ghost:: ~ghost() throw() {}
        
        ghost:: ghost( ghost::position p ) throw() :
        site( p ),
        offsets()
        {
        }
        
        const char * ghost:: get_position_name( ghost::position p ) throw()
        {
            switch( p )
            {
                case at_lower_x: return "lower x";
                case at_upper_x: return "upper x";
                case at_lower_y: return "lower y";
                case at_upper_y: return "upper y";
                case at_lower_z: return "lower z";
                case at_upper_z: return "upper z";
            }
            critical_error(error_invalid_data, "invalid ghost::position name");
            return "";
        }
        
        const char *ghost:: position_name() const throw()
        {
            return get_position_name(site);
        }
        
        ghost::position ghost:: get_mirror_position( ghost::position p ) throw()
        {
            switch( p )
            {
                case at_lower_x: return at_upper_x;
                case at_upper_x: return at_lower_x;
                case at_lower_y: return at_upper_y;
                case at_upper_y: return at_lower_y;
                case at_lower_z: return at_upper_x;
                case at_upper_z: return at_lower_z;
            }
            critical_error(error_invalid_data, "invalid ghost::position mirror");
            return at_upper_x;
        }
        
        ghost::position ghost:: mirror_position() const throw()
        {
            return get_mirror_position( site );
        }
        
        ghost::position ghost:: get_lower_position( size_t dim )
        {
            switch( dim )
            {
                case 0: return at_lower_x;
                case 1: return at_lower_y;
                case 2: return at_lower_z;
                default: break;
            }
            critical_error(error_invalid_data, "invalid ghost::lower_position dimension index");
            return ghost::position(-1);
        }
        
        ghost::position ghost:: get_upper_position( size_t dim )
        {
            switch( dim )
            {
                case 0: return at_upper_x;
                case 1: return at_upper_y;
                case 2: return at_upper_z;
                default: break;
            }
            critical_error(error_invalid_data, "invalid ghost::upper_position dimension index");
            return ghost::position(-1);
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // local_ghost_pair
        //
        ////////////////////////////////////////////////////////////////////////
        local_ghosts_pair:: local_ghosts_pair( ghost::position source ) :
        inside( source  ),
        mirror(  inside.mirror_position()  )
        {
        }
        
        local_ghosts_pair:: ~local_ghosts_pair() throw() {}
        
        ////////////////////////////////////////////////////////////////////////
        //
        // ghosts_base
        //
        ////////////////////////////////////////////////////////////////////////
        ghosts_base:: ghosts_base( size_t num_ghosts ) throw() :
        count(num_ghosts),
        num_offsets(0)
        {
        }
        
        ghosts_base:: ~ghosts_base() throw() {}
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // local_ghosts
        //
        ////////////////////////////////////////////////////////////////////////
        local_ghosts:: local_ghosts( size_t num_ghosts, size_t dim ) :
        ghosts_base( num_ghosts ),
        lower( ghost::get_lower_position(dim) ),
        upper( ghost::get_upper_position(dim) )
        {
        }
        
        local_ghosts:: ~local_ghosts() throw() {}
        
        void local_ghosts:: transfer( const linear_handles & handles ) const throw()
        {
            assert( num_offsets == lower.inside.offsets.size() );
            assert( num_offsets == lower.mirror.offsets.size() );
            assert( num_offsets == upper.inside.offsets.size() );
            assert( num_offsets == upper.mirror.offsets.size() );                
            const size_t num_handles = handles.size();
            for( size_t i=num_offsets; i>0; --i )
            {
                const size_t lower_mirror = lower.mirror.offsets[i];
                const size_t lower_inside = lower.inside.offsets[i];
                const size_t upper_mirror = upper.mirror.offsets[i];
                const size_t upper_inside = upper.inside.offsets[i];
                for( size_t j=num_handles; j>0; --j )
                {
                    linear_base *A = handles[j]; assert(A!=NULL);
                    A->local_copy( lower_mirror, lower_inside );
                    A->local_copy( upper_mirror, upper_inside );
                }
            }
        }
        
        void local_ghosts:: transfer1( linear_base &A ) const throw()
        {
            assert( num_offsets == lower.inside.offsets.size() );
            assert( num_offsets == lower.mirror.offsets.size() );
            assert( num_offsets == upper.inside.offsets.size() );
            assert( num_offsets == upper.mirror.offsets.size() );  
            for( size_t i=num_offsets; i>0; --i )
            {
                const size_t lower_mirror = lower.mirror.offsets[i];
                const size_t lower_inside = lower.inside.offsets[i];
                const size_t upper_mirror = upper.mirror.offsets[i];
                const size_t upper_inside = upper.inside.offsets[i];
                A.local_copy( lower_mirror, lower_inside);
                A.local_copy( upper_mirror, upper_inside);
            }
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // async_ghost_pair
        //
        ////////////////////////////////////////////////////////////////////////
        async_ghosts_pair:: async_ghosts_pair( ghost::position source ) throw() :
        inner( source ),
        outer( source )
        {
        }
        
        async_ghosts_pair:: ~async_ghosts_pair() throw()
        {
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // async_ghosts
        //
        ////////////////////////////////////////////////////////////////////////
        async_ghosts:: async_ghosts( size_t num_ghosts, ghost::position source, int a_peer ) :
        ghosts_base( num_ghosts ),
        self( source ),
        peer( a_peer ),
        inner_buf(NULL),
        outer_buf(NULL),
        length(0),
        iolen(0)
        {
        }
        
        async_ghosts:: ~async_ghosts() throw()
        {
            if( inner_buf )
            {
                assert( outer_buf != NULL );
                memory::kind<memory::global>::release_as<uint8_t>( inner_buf, iolen );
                outer_buf = NULL;
                length    = 0;
            }
        }
        
        
        //! allocate memory once offsets are computed
        void async_ghosts:: allocate_for( linear_handles &handles )
        {
            assert( NULL == inner_buf );
            assert( NULL == outer_buf );
            assert( 0    == iolen );
            assert( 0    == length );
            for( size_t j=handles.size(); j>0; --j )
            {
                length += handles[j]->item_size();
            }
            length *= num_offsets;
            iolen   = length * 2;
            try
            {
                inner_buf  = memory::kind<memory::global>::acquire_as<uint8_t>( iolen );
            }
            catch(...) { length = 0; throw; }
            outer_buf  = inner_buf + length;
        }
        
        //! store inner data into inner_buf
        void async_ghosts:: store_inner( const linear_handles &handles ) throw()
        {
            uint8_t *ptr = inner_buf;
            for( size_t i=num_offsets; i>0; --i )
            {
                const size_t k = self.inner.offsets[i];
                for( size_t j=handles.size(); j>0; --j )
                {
                    const linear_base &A = *handles[j];
                    A.async_store(ptr,k);
                    assert( ptr <= inner_buf + length );
                }
            }
        }
        
        size_t async_ghosts:: store_inner1( const linear_base &A ) throw()
        {
            uint8_t *ptr = inner_buf;
            for( size_t i=num_offsets; i>0; --i )
            {
                const size_t k = self.inner.offsets[i];
                A.async_store(ptr, k);
                assert( ptr <= inner_buf + length );
            }
            return num_offsets * A.item_size();
        }
        
        
        //! query outer data from outer_buf
        void async_ghosts:: query_outer( const linear_handles &handles ) const throw()
        {
            const uint8_t *ptr = outer_buf;
            for( size_t i=num_offsets; i>0; --i )
            {
                const size_t k = self.outer.offsets[i];
                for( size_t j=handles.size(); j>0; --j )
                {
                    linear_base &A = *handles[j];
                    A.async_query(ptr,k);
                    assert( ptr <= outer_buf + length );
                }
            }
            
        }
        
        // idem for one field only
        void async_ghosts:: query_outer1( linear_base &A ) const throw()
        {
            const uint8_t *ptr = outer_buf;
            for( size_t i=num_offsets; i>0; --i )
            {
                const size_t k = self.outer.offsets[i];
                A.async_query(ptr,k);
                assert( ptr <= outer_buf + length );
            }
            
        }

        
        
    }
}
