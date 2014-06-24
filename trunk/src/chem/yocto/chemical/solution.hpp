#ifndef YOCTO_CHEMICAL_SOLUTION_INCLUDED
#define YOCTO_CHEMICAL_SOLUTION_INCLUDED 1

#include "yocto/chemical/collection.hpp"
#include "yocto/sequence/array.hpp"

namespace yocto
{
    
    namespace chemical
    {
        
        class solution : public array<double>
        {
        public:
            explicit solution( const collection &from );
            virtual ~solution() throw();
            const collection &lib;
            
            virtual const char *name() const throw();
            virtual size_t      size() const throw();
            virtual size_t      capacity() const throw();
            virtual void        reserve(size_t );
            virtual void        free() throw();
            virtual void        release() throw();
            
            double       & operator[]( const string &id );
            const double & operator[]( const string &id ) const;

            void output( std::ostream &) const;
            inline friend std::ostream & operator<<( std::ostream &os, const solution &self)
            {
                self.output(os);
                return os;
            }
            
            
        private:
            const size_t n;
            size_t       m; //!< memory
            double      *C; //!< conc
            
            YOCTO_DISABLE_ASSIGN(solution);
            virtual const double *get_item() const throw();
        };
    }
}

#endif

