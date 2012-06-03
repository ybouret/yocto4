#ifndef YOCTO_AQ_INITIALIZER_INCLUDED
#define YOCTO_AQ_INITIALIZER_INCLUDED 1

#include "yocto/aqueous/chemsys.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/shared-ptr.hpp"

namespace yocto 
{
    namespace aqueous
    {
        
        class initializer;
        
        class constraint 
        {
        public:
            typedef map<string,double>     weights;
            typedef shared_ptr<constraint> ptr;
            
            explicit constraint( const library &L, double v ) throw();
            virtual ~constraint() throw();
            const double value;
            
            void add( const string &id, double w );
            
        private:
            YOCTO_DISABLE_ASSIGN(constraint);
            weights        W;
            friend class initializer;
            
        public:
            const library &lib;
        };
        
        class initializer
        {
        public:
            explicit initializer(const library &L ) throw();
            virtual ~initializer() throw();
            
            constraint & create( double v );
            
            void operator()( chemsys &cs , double t);
            
        private:
            vector< constraint::ptr > constraints;
            YOCTO_DISABLE_COPY_AND_ASSIGN(initializer);
        public:
            const library &lib;
        };
        
        
    }
    
}

#endif
