#include "yocto/chemical/boot.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{
    
    using namespace math;
    
    namespace chemical
    {
        
        
        namespace
        {
            
            class BootManager
            {
            public:
                
                const collection &lib;
                equilibria       &cs;
                const boot       &loader;
                const size_t      M;
                const size_t      N;
                const size_t      Nc;
                matrix_t          P;
                
                inline BootManager(const collection &user_lib,
                                   equilibria       &user_cs,
                                   const boot       &user_loader,
                                   const double t ) :
                lib( user_lib ),
                cs( user_cs ),
                loader( user_loader ),
                M( lib.size() ),
                N( cs.size()  ),
                Nc( loader.size() ),
                P()
                {
                    //__________________________________________________________
                    //
                    // Sanity Check....
                    //__________________________________________________________
                    
                    
                    //__________________________________________________________
                    //
                    // Memory Allocation
                    //__________________________________________________________
                    
                }
                
                inline ~BootManager() throw()
                {
                    
                }
                
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(BootManager);
            };
            
            
        }
        
        void boot:: operator()(array<double>    &C0,
                               const collection &lib,
                               equilibria       &cs,
                               double            t )
        {
            assert(C0.size()>=lib.size());
            BootManager mgr(lib,cs,*this,t);
            
            
        }
        
        
    }
    
    
}
