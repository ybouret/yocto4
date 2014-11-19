#ifndef YOCTO_CHEMICAL_BOOT_INCLUDED
#define YOCTO_CHEMICAL_BOOT_INCLUDED 1

#include "yocto/chemical/library.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    
    namespace chemical
    {
        
        class constraint : public counted_object
        {
        public:
            typedef arc_ptr<constraint> pointer;
            
            class member
            {
            public:
                const species::pointer sp;
                const int              weight; //!< !=0
                
                member( const species::pointer &a, const int w ) throw();
                ~member() throw();
                member(const member &) throw();
                
            private:
                YOCTO_DISABLE_ASSIGN(member);
            };
            
            const double level;
            
            explicit constraint(const double l) throw();
            virtual ~constraint() throw();
            
            constraint  &add( const species::pointer &sp, const int weight );
            size_t count() const throw();
            
            const member & operator[](size_t i) const throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(constraint);
            vector<member> members;
        };
        
        typedef vector<constraint::pointer> constraints;
        
        class boot : public constraints
        {
        public:
            
            constraint & new_constraint( const double level );
            
            explicit boot() throw();
            virtual ~boot() throw();
            
            void electroneutrality( const library &lib );
            
            void output( std::ostream &os ) const;
            inline friend std::ostream &operator<<(std::ostream &os, const boot &b)
            {
                b.output(os);
                return os;
            }
            
            void conserve( const double Ctot, const species::pointer &sp );
            void conserve( const double Ctot, const species::pointer &A, const species::pointer &B);
            void conserve( const double Ctot, const species::pointer &A, const species::pointer &B, const species::pointer &C);

            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(boot);
        };
    }
    
}

#endif
