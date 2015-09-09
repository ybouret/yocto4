#ifndef YOCTO_CHEMICAL_EFFECTORS_INCLUDED
#define YOCTO_CHEMICAL_EFFECTORS_INCLUDED 1

#include "yocto/chemical/parameters.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace chemical
    {
        
        class effector : public counted_object
        {
        public:
            typedef intr_ptr<string,effector> pointer;
            typedef set<string,pointer>       db;
            const string name;
            double       pace; //!< initially 1.0, may be used for Vmax or scaling
            
            virtual ~effector() throw();
            const string & key() const throw();
            
            //! compute the rate without Vmax
            virtual void rate(array<double>       &rho,
                              const double         t,
                              const array<double> &Cin,
                              const array<double> &Cout,
                              const parameters    &params) = 0;

            //! multiply rho[1..params.lib.size] by pace
            void update(array<double> &rho, const parameters &params) const throw();

            friend std::ostream & operator<<( std::ostream &, const effector &);

        protected:
            explicit effector(const string &id);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effector);
        };
        
        
        class effectors : public effector::db
        {
        public:
            explicit effectors() throw();
            virtual ~effectors() throw();
            
            void rate(array<double>       &rho,
                      const double         t,
                      const array<double> &Cin,
                      const array<double> &Cout,
                      const parameters    &params);
            
            effector & operator[](const string &name );
            effector & operator[](const char   *name );

            void rescale_pace( const double speedup ) throw();

            friend std::ostream & operator<<( std::ostream &, const effectors &);


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(effectors);
            vector<double> _rho;
        };
        
    }
}

#endif
