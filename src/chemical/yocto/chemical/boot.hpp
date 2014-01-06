#ifndef YOCTO_CHEMICAL_BOOT_INCLUDED
#define YOCTO_CHEMICAL_BOOT_INCLUDED 1

#include "yocto/chemical/equilibria.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/ptr/shared.hpp"

namespace yocto
{
    namespace chemical
    {
        
        
        struct boot
        {
            
            //! a constituent: species + weight
            class constituent : public object, public counted
            {
            public:
                explicit constituent( const species::ptr &sp, const double w ) throw();
                virtual ~constituent() throw();
                constituent( const constituent &) throw();
                
                const species::ptr spec;
                const double       weight;
                const string &     key() const throw();
                
                typedef intr_ptr<string,constituent> ptr;
                
            private:
                YOCTO_DISABLE_ASSIGN(constituent);
            };
            
            typedef key_hasher<string>        hasher_type;
            typedef memory::pooled::allocator allocator_type;
            typedef set<string,constituent::ptr,hasher_type,allocator_type> constituents;
            
            //! a constraint: constituents + values
            class constraint : public constituents
            {
            public:
                explicit constraint(const double v) throw();
                virtual ~constraint() throw();
                
                double value;
                
                constraint &weight( const species::ptr &sp, const double w);
                
                
                typedef shared_ptr<constraint> ptr;
                
                friend std::ostream & operator<<( std::ostream &, const constraint &);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(constraint);
            };
            
            typedef vector<constraint::ptr,allocator_type> constraints;
            
            //! a vector of constraints
            class loader : public constraints
            {
            public:
                explicit loader() throw();
                virtual ~loader() throw();
                
                constraint & add(double value);
                
                //! helper: set species concentration
                void define( const species::ptr &sp, const double conc);
                
                //! electroneutrality: disabled is no charged species
                void electroneutrality( const collection &lib );
                
                //! compute matrix and constraint vector
                /**
                 \param P a matrix MxNc (M is the lib.size)
                 \param Lam a vector Nc
                 \warning the collection must have updated indices !
                 */
                void fill( matrix_t &P, array<double> &Lam) const throw();
                
                
                friend std::ostream & operator<<( std::ostream &, const loader &);
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(loader);
            };
            
        };
    }
}


#endif
