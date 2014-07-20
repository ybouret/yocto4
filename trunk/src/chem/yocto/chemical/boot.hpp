#ifndef YOCTO_CHEMICAL_BOOT_INCLUDED
#define YOCTO_CHEMICAL_BOOT_INCLUDED 1

#include "yocto/chemical/equilibria.hpp"

namespace yocto
{
    namespace chemical
    {
        
        class boot
        {
        public:
            
            //! a species and its coefficient
            class item
            {
            public:
                const species::pointer spec;
                const int              coef;
                explicit item( const species::pointer &s, const int w);
                virtual ~item() throw();
                item(const item &other) throw();
                const string &key() const throw();
            
                typedef set<string,item> db;
                
            private:
                YOCTO_DISABLE_ASSIGN(item);
            };
            
            //! a constraint is a set of items with a sum
            class constraint : public counted_object, public item::db
            {
            public:
                //! increase collection count...
                explicit constraint(const double value)  throw();
                
                //! decrease collection count
                virtual ~constraint() throw();
                
                const double      sum;
                
                constraint &weight( const species::pointer &p, const int w );
                
                typedef arc_ptr<constraint> pointer;
                
                friend std::ostream & operator<<(std::ostream &os, const constraint &cc);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(constraint);
            };
            
            
            
            explicit boot();
            virtual ~boot() throw();
            
            constraint &create(const double value);
            
            //! conserve one species
            void conserve(const species::pointer &sp, const double C);
            
            //! conserve the sum of two species
            void conserve(const species::pointer &sp1, const species::pointer &sp2, const double C);
            
            //! conserve the sum of three species
            void conserve(const species::pointer &sp1, const species::pointer &sp2, const species::pointer &sp3, const double C);

            
            void electroneutrality( const collection &lib );
            
            friend std::ostream & operator<<( std::ostream &os, const boot &ini);
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(boot);
            vector<constraint::pointer> constraints;
        };
        
    }
}

#endif
