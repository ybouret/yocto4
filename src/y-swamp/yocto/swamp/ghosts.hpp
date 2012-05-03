#ifndef YOCTO_SWAMP_GHOSTS_INCLUDED
#define YOCTO_SWAMP_GHOSTS_INCLUDED 1

#include "yocto/swamp/linear.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/shared-ptr.hpp"

namespace yocto 
{
    namespace swamp 
    {
        typedef array<linear_base *>  linear_handles;

        class ghost 
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
            static const char     *get_position_name( ghost::position p ) throw();
            static ghost::position get_mirror_position( ghost::position p ) throw();
            static ghost::position get_lower_position( size_t dim );
            static ghost::position get_upper_position( size_t dim );
            
            virtual ~ghost() throw();
            
            const position site;
            offsets_list   offsets;
            
            explicit ghost( ghost::position p ) throw();
            const char *position_name() const throw();
            ghost::position mirror_position() const throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghost);
        };
        
        //! ghosts pair for local communication
        class local_ghosts_pair
        {
        public:
            ghost inside; //!< inner source
            ghost mirror; //!< outer mirror target
            
            explicit local_ghosts_pair( ghost::position source ) :
            inside( source  ),
            mirror(  inside.mirror_position()  )
            {
            }
            
            ~local_ghosts_pair() throw() {}
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(local_ghosts_pair);
        };
        
        
        class ghosts_base : public object
        {
        public:
            const size_t       count;       //!< number of ghosts points/lines/slices
            const size_t       num_offsets; //!< to be set during allocation

            virtual ~ghosts_base() throw() {}
            
        protected:
            explicit ghosts_base( size_t num_ghosts ) throw() :
            count(num_ghosts),
            num_offsets(0)
            {
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts_base);
        };
        
        class local_ghosts : public ghosts_base
        {
        public:
            
            local_ghosts_pair  lower;
            local_ghosts_pair  upper;
            
            //! prepare ghosts pairs, no memory allocated
            local_ghosts( size_t num_ghosts, size_t dim ) :
            ghosts_base( num_ghosts ),
            lower( ghost::get_lower_position(dim) ),
            upper( ghost::get_upper_position(dim) )
            {
            }
            
            
            //! direct copy transfert
            inline void transfer( linear_handles &handles ) const throw()
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
            
            typedef shared_ptr<local_ghosts> ptr;
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(local_ghosts);
        };
        
        
        //! ghosts pair for async communication
        class async_ghosts_pair
        {
        public:
            ghost     inner;
            ghost     outer;
            
            async_ghosts_pair( ghost::position source ) throw() :
            inner( source ),
            outer( source )
            {
            }
            
            ~async_ghosts_pair() throw()
            {
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(async_ghosts_pair);
        };
        
        //! MPI style async ghosts
        class async_ghosts : public ghosts_base
        {
        public:            
            async_ghosts( size_t num_ghosts, ghost::position source, int a_peer ) :
            ghosts_base( num_ghosts ),
            gpair( source ),
            peer(  a_peer ),
            inner_buf(NULL),
            outer_buf(NULL),
            iolen(0)
            {
            }
            
            virtual ~async_ghosts() throw()
            {
                if( inner_buf )
                {
                    assert( outer_buf != NULL );
                    memory::kind<memory::global>::release_as<uint8_t>( inner_buf, iolen );
                    outer_buf = NULL;
                }
            }
            
            async_ghosts_pair   gpair; //!< the I/O pair
            const int           peer;  //!< MPI peer
            
            
            //! allocate memory once offsets are computed
            void allocate_for( linear_handles &handles )
            {
                assert( NULL == inner_buf );
                assert( NULL == outer_buf );
                assert( 0    == iolen );
                for( size_t j=handles.size(); j>0; --j )
                {
                    iolen += handles[j]->item_size();
                }
                iolen *= num_offsets;
                const size_t shift = iolen;
                iolen *= 2;
                inner_buf  = memory::kind<memory::global>::acquire_as<uint8_t>( iolen );
                outer_buf  = inner_buf + shift;
            }
            
            //! store inner data into inner_buf
            void store_inner( linear_handles &handles ) throw()
            {
                uint8_t *ptr = inner_buf;
                for( size_t i=num_offsets; i>0; --i )
                {
                    const size_t k = gpair.inner.offsets[i];
                    for( size_t j=handles.size(); j>0; --j )
                    {
                        const linear_base &A = *handles[j];
                        A.async_store(ptr,k);
                        assert( ptr <= inner_buf + (iolen>>1) );
                    }
                }
            }
            
            //! query outer data from outer_buf
            void query_outer( linear_handles &handles ) const throw()
            {
                const uint8_t *ptr = outer_buf;
                for( size_t i=num_offsets; i>0; --i )
                {
                    const size_t k = gpair.outer.offsets[i];
                    for( size_t j=handles.size(); j>0; --j )
                    {
                        linear_base &A = *handles[j];
                        A.async_query(ptr,k);
                        assert( ptr <= outer_buf + (iolen>>1) );
                    }
                }
                
            }
            
            typedef shared_ptr<async_ghosts> ptr;
            
        private:
            uint8_t       *inner_buf;
            uint8_t       *outer_buf;
            size_t         iolen;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(async_ghosts);
        };
        
        template <typename COORD>
        class ghosts_setup
        {
        public:
            class local_setup
            {
            public:
                COORD count;
                inline  local_setup() throw() : count() 
                {
                    memset( &count, 0x00, sizeof(count) ); 
                }
                inline ~local_setup() throw() {}
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(local_setup);
            };
            
            class async_setup
            {
            public:
                COORD count;
                COORD peer;
                inline async_setup() throw() : count(), peer() 
                {
                    memset( &count, 0x00, sizeof(count) );
                    memset( &peer,  0xFF, sizeof(peer) );
                }
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(async_setup);
            };
            
            local_setup local;
            async_setup lower;
            async_setup upper;
            
            inline ghosts_setup() throw() :
            local(),
            lower(),
            upper()
            {
            }
            
            inline ~ghosts_setup() throw()
            {
            }
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts_setup);
        };
        
    }
}

#endif
