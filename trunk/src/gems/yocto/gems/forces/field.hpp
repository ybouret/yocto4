#ifndef YOCTO_GEMS_FORCES_FIELD_INCLUDED
#define YOCTO_GEMS_FORCES_FIELD_INCLUDED 1

#include "yocto/gems/forces/unary.hpp"
#include "yocto/gems/forces/binary.hpp"

namespace yocto
{
    namespace gems
    {
        
        template <typename T>
        class force_field
        {
        public:
            typedef typename atom<T>::pointer         atom_ptr;
            typedef typename unary_force<T>::pointer  unary_force_ptr;
            typedef typename binary_force<T>::pointer binary_force_ptr;
            
            class unary_single
            {
            public:
                atom_ptr        a;
                unary_force_ptr f;
                inline unary_single(const atom_ptr &ap, const unary_force_ptr &fp) throw() :
                a(ap),
                f(fp)
                {
                    
                }
                
                inline ~unary_single() throw() {}
                inline  unary_single( const unary_single &other ) throw() :
                a(other.a), f(other.f)
                {
                }
                
                
            private:
                YOCTO_DISABLE_ASSIGN(unary_single);
            };
            
            class binary_single
            {
            public:
                atom_ptr        a;
                atom_ptr        b;
                unary_force_ptr f;
                inline binary_single(const atom_ptr        &ap,
                                     const atom_ptr        &bp,
                                     const unary_force_ptr &fp) throw() :
                a(ap),
                b(bp),
                f(fp)
                {
                    
                }
                
                inline ~binary_single() throw() {}
                inline  binary_single( const binary_single &other ) throw() :
                a(other.a),
                b(other.b),
                f(other.f)
                {
                }
                
                
            private:
                YOCTO_DISABLE_ASSIGN(binary_single);
            };

            
            
            
            explicit force_field() throw() :
            unary_forces(),
            binary_forces(),
            unary_singles(),
            binary_singles()
            {}
            
            virtual ~force_field() throw() {}
            
            vector<unary_force_ptr>  unary_forces;   //!< for everyone
            vector<binary_force_ptr> binary_forces;  //!< for everyone
            
            vector<unary_single>      unary_singles;
            vector<binary_single>     binary_singles;

            
            void append( unary_force<T> *f )
            {
                const unary_force_ptr p(f);
                unary_forces.push_back(p);
            }
            
            void append( binary_force<T> *f )
            {
                const binary_force_ptr p(f);
                binary_forces.push_back(p);
            }
            
            void reset( atoms<T> &a )
            {
                for(size_t i=a.size();i>0;--i)
                {
                    a[i]->f.ldz();
                }
            }
            
            //! add unary and binary forces
            T collect( atoms<T> &a )
            {
                T Ep = 0;
                const size_t n1 = unary_forces.size();
                const size_t n2 = binary_forces.size();
                for(size_t i=a.size();i>0;--i)
                {
                    atom<T> &ai = *a[i];
                    for(size_t k=n1;k>0;--k)
                    {
                        Ep += unary_forces[k]->update(ai);
                    }
                    
                    for(size_t j=i-1;j>0;--j)
                    {
                        for(size_t k=n2;k>0;--k)
                        {
                            Ep += binary_forces[k]->update(ai,*a[j]);
                        }
                    }
                }
                
                return Ep;
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(force_field);
            
        };
    }
    
}


#endif

