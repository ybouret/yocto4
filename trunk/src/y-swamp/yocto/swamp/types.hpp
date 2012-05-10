// \file
/**
 Simplified Workspaces and Meshes Processing
 */

#ifndef YOCTO_SWAMP_TYPES_INCLUDED
#define YOCTO_SWAMP_TYPES_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/string.hpp"

namespace yocto 
{
    
    namespace swamp
    {
        
        //! signed integer for indices
        typedef ptrdiff_t              unit_t;
       
        template <typename COORD>
        inline unit_t & __coord( COORD &C, size_t dim ) throw()
        {
            assert( dim < sizeof(COORD)/sizeof(unit_t));
            unit_t *c = (unit_t *) &C;
            return  c[dim];
        }
        
        template <typename COORD>
        inline const unit_t & __coord( const COORD &C, size_t dim ) throw()
        {
            assert( dim < sizeof(COORD)/sizeof(unit_t));
            unit_t *c = (unit_t *) &C;
            return  c[dim];
        }
        
        
        //! base class for list of offsets
        typedef sorted_vector<size_t>  sorted_offsets;
       
        class offsets_list : public sorted_offsets
        {
        public:
            explicit offsets_list() throw();
            explicit offsets_list(size_t num);
            virtual ~offsets_list() throw();
            offsets_list( const offsets_list & );
            
            void store( size_t indx );
            
        private:
            YOCTO_DISABLE_ASSIGN(offsets_list);
        };
        
        class variables : public vector<string>
        {
        public:
            virtual ~variables() throw();
            explicit variables() throw();
            explicit variables(size_t n);
            variables( const variables & );
            
            variables & operator<<( const string & );
            variables & operator<<( const char *id );
            
            
        private:
            YOCTO_DISABLE_ASSIGN(variables);
        };
        
        
    }
    
}

#endif
