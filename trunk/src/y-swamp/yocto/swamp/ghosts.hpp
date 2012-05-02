#ifndef YOCTO_SWAMP_GHOSTS_INCLUDED
#define YOCTO_SWAMP_GHOSTS_INCLUDED 1

#include "yocto/swamp/arrays.hpp"
#include "yocto/swamp/array3d.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto 
{
    namespace swamp 
    {
        
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
        
        //! ghosts pair for communication
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
            const size_t       count; //!< number of ghosts points/lines/slices
            virtual ~ghosts_base() throw() {}
            
        protected:
            const size_t num_offsets;
            
            explicit ghosts_base( size_t num_ghosts ) throw() :
            count(num_ghosts),
            num_offsets(0)
            {
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts_base);
        };
        
        template <typename ARRAY>
        class local_ghosts : public ghosts_base
        {
        public:
            typedef typename ARRAY::layout_type layout_type;
            typedef array<ARRAY *>              array_handles;
            
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
            inline void transfer( array_handles &handles ) const throw()
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
                        ARRAY &A = *handles[j];
                        A.entry[ lower_mirror ] = A.entry[ lower_inside ];
                        A.entry[ upper_mirror ] = A.entry[ upper_inside ];
                    }
                }
            }
            
            
            
        private:
            const size_t num_offsets;
            YOCTO_DISABLE_COPY_AND_ASSIGN(local_ghosts);
        };
        
        
        
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
        
        template <typename ARRAY>
        class async_ghosts : public ghosts_base
        {
        public:
            typedef typename ARRAY::layout_type layout_type;            
            typedef array<ARRAY *>              array_handles;
            typedef typename ARRAY::type        type;
            typedef  array2D<type>              channels_type;
            
            async_ghosts( size_t num_ghosts, ghost::position source, int peer, array_db &db ) :
            ghosts_base( num_ghosts ),
            gpair(source),
            gpeer( peer ),
            channels(NULL),
            spec( typeid( channels_type ) ),
            adb(  db )
            {
            }
            
            async_ghosts_pair   gpair; //!< the I/O pair
            const int           gpeer; //!< MPI peer
            
            //! allocate I/O memory
            void allocate( const size_t num_channels )
            {
                assert( NULL == channels   );
                if(num_offsets>0)
                {
                    const coord2D  lo(1,1);                                //-- start @1,1
                    const coord2D  up(num_channels,num_offsets);           //-- num_channels rows of num_offsets items
                    const layout2D L(lo,up);                               //-- corresponding 2D layout
                    const string   name( gpair.inner.position_name() );    //-- its unique name in database
                    linear_base   *info = NULL;                            //-- for memory allocation
                    void          *addr = channels_type::ctor(L,&info);    //-- allocate the channels
                    adb(name, spec, addr, info, channels_type::dtor);      //-- register the channels
                    channels = & adb[ name ].template as<channels_type>(); //-- get them from the database
                }
            }
            
            
            //! store  inner data into channels
            void store_inner(  array_handles &handles ) throw()
            {
                if( num_offsets > 0 )
                {
                    assert(channels!=NULL);
                    assert(num_offsets    == gpair.inner.offsets.size());
                    assert(num_offsets    == gpair.outer.offsets.size());
                    assert(handles.size() == size_t(channels->width.y) );
                    
                    const size_t   num_handles = handles.size();
                    channels_type &C           = *channels; 
                    for( size_t i=num_offsets;i>0;--i)
                    {
                        const size_t inner_i = gpair.inner.offsets[i];
                        for( size_t j = num_handles; j>0; --j )
                        {
                            C[j][i] = (*handles)[j][inner_i];
                        }
                    }
                }
            }
            
            //! load outer data from channels
            void load_outer( array_handles &handles ) const throw()
            {
                if( num_offsets > 0 )
                {
                    assert(channels!=NULL);
                    assert(num_offsets    == gpair.inner.offsets.size());
                    assert(num_offsets    == gpair.outer.offsets.size());
                    assert(handles.size() == size_t(channels->width.y) );
                    
                    const size_t   num_handles = handles.size();
                    const channels_type &C     = *channels; 
                    for( size_t i=num_offsets;i>0;--i)
                    {
                        const size_t inner_i = gpair.inner.offsets[i];
                        for( size_t j = num_handles; j>0; --j )
                        {
                            (*handles)[j][inner_i] = C[j][i];
                        }
                    }
                }

            }
            
            
            
            
        private:
            channels_type  *channels;
            const type_spec spec;
            array_db       &adb;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(async_ghosts);
        };
        
    }
}

#endif
