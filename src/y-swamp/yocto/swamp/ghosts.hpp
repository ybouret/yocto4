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
        
        //! a ghost is a list of offsets
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
            
            const position site;     //!< ghost position
            offsets_list   offsets;  //!< initial empty list
            
            //! ctor: set ghost position
            explicit ghost( ghost::position p ) throw();
            const char *position_name() const throw();
            
            //! get peer position
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
            
            explicit local_ghosts_pair( ghost::position source );            
            ~local_ghosts_pair() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(local_ghosts_pair);
        };
        
        //! common information for ghosts
        class ghosts_base : public object
        {
        public:
            const size_t       count;       //!< number of ghosts points/lines/slices
            const size_t       num_offsets; //!< to be set during allocation
            
            virtual ~ghosts_base() throw();
            
        protected:
            explicit ghosts_base( size_t num_ghosts ) throw(); 
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(ghosts_base);
        };
        
        
        //! local ghosts for local PBC
        class local_ghosts : public ghosts_base
        {
        public:
            local_ghosts_pair  lower; //!< lower.inside and its mirror
            local_ghosts_pair  upper; //!< upper.inside and its mirror
            
            //! prepare ghosts pairs, no memory allocated
            explicit local_ghosts( size_t num_ghosts, size_t dim );            
            virtual ~local_ghosts() throw();
            
            //! direct copy transfert
            void transfer( const linear_handles & handles ) const throw();
            
            //! transfer only one of the handles
            void transfer1( linear_base &A ) const throw();
            
            //! shared pointer
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
            
            async_ghosts_pair( ghost::position source ) throw();            
            ~async_ghosts_pair() throw();
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(async_ghosts_pair);
        };
        
        //! MPI style async ghosts
        class async_ghosts : public ghosts_base
        {
        public:            
            async_ghosts( size_t num_ghosts, ghost::position source, int a_peer );            
            virtual ~async_ghosts() throw();
            
            
            //! allocate memory once offsets are computed
            void allocate_for( linear_handles &handles );
            
            //! store inner data into inner_buf
            void store_inner( const linear_handles &handles ) throw();
            
            //! store inner data into inner_buf
            /**
             must have enough memory...
             \return the number_of stored bytes is A.item_size() * num_offset
             */
            size_t store_inner1( const linear_base &A ) throw();
            
            //! query outer data from outer_buf
            void query_outer( const linear_handles &handles ) const throw();
            
            //! query outer data from outer_buf
            void query_outer1( linear_base &A ) const throw();
            
            
            typedef shared_ptr<async_ghosts> ptr;
            
            async_ghosts_pair   self;  //!< the I/O pair
            const int           peer;  //!< MPI peer
            uint8_t            *inner_buf;
            uint8_t            *outer_buf;
            size_t              length;
            
        private:
            size_t               iolen;  //!< twice length
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(async_ghosts);
        };
        
        
        //! describe all ghosts topology
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
