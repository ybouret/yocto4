#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/ordered/sorted-vector.hpp"


namespace yocto
{
    namespace chemical
    {
        static inline
        bool check_valid( const array<integer_t> &P ) throw()
        {
            for(size_t j=P.size();j>0;--j)
            {
                if(0!=P[j])
                    return true;
            }
            return false;
        }
        
        static inline
        size_t count_items( size_t &s, const array<integer_t> &P ) throw()
        {
            size_t count = 0;
            s = 0;
            for(size_t j=P.size();j>0;--j)
            {
                if(P[j]!=0)
                {
                    s = j;
                    ++count;
                }
            }
            return count;
        }
        
        void boot:: operator()( array<double> &C, const collection &lib, equilibria &cs, double t )
        {
            cs.startup(lib);
            try
            {
                const size_t N = cs.N;
                const size_t M = cs.M;
                assert(C.size()>=M);
                if(N>M)
                    throw exception("too many reactions");
                const size_t Nc = constraints.size();
                if(Nc+N!=M)
                    throw exception("#species=%u != #reactions=%u + #constraints=%u ", unsigned(M), unsigned(N), unsigned(Nc));
                
                if(Nc<=0)
                {
                    std::cerr << "NOT IMPLEMENTED : NO CONSTRAINTS" << std::endl;
                    return;
                }
                
                //______________________________________________________________
                //
                // Fill the P matrix and Lambda vector
                //______________________________________________________________
                imatrix_t P(Nc,M);
                vector_t  Lambda(Nc,0.0);
                for(size_t i=1;i<=Nc;++i)
                {
                    const constraint &cc = *constraints[i];
                    Lambda[i] = cc.sum;
                    cc.fill(P[i]);
                }
                
                std::cerr << "P0="      << P      << std::endl;
                std::cerr << "Lambda0=" << Lambda << std::endl;
                
                
                //______________________________________________________________
                //
                // optimize with singleton
                //______________________________________________________________
                matrix_t             &Nu = cs.Nu;
                sorted_vector<size_t> fixed(Nc,as_capacity);
                bool                  changed = false;
                for(size_t i=1;i<=Nc;++i)
                {
                    //__________________________________________________________
                    //
                    // how many not zero items...
                    //__________________________________________________________
                    size_t       s     = 0;
                    const size_t count = count_items(s,P[i]);
                    
                    //__________________________________________________________
                    //
                    // None: bad
                    //__________________________________________________________
                    if(count<=0)
                        throw exception("unexpected empty constraint[%u]!", unsigned(i));
                    
                    
                    //__________________________________________________________
                    //
                    // general case
                    //__________________________________________________________
                    if(count>1)
                        continue;
                    
                    //__________________________________________________________
                    //
                    // a singleton !
                    //__________________________________________________________
                    assert(1==count);
                    if( !fixed.insert(s) )
                        throw exception("multiple single constraint on species #%u", unsigned(s) );
                    
                    //__________________________________________________________
                    //
                    // simplify the projection matrix
                    //__________________________________________________________
                    changed = true;
                    
                    assert(0!=P[i][s]);
                    const double lhs = (Lambda[i] /= P[i][s]);
                    P[i][s] = 1;
                    
                    //__________________________________________________________
                    //
                    // simplify P: inform other rows of that case
                    //__________________________________________________________
                    for(size_t k=1;k<=Nc;++k)
                    {
                        if(i!=k)
                        {
                            array<integer_t> &Pk = P[k];
                            integer_t        &p  = Pk[s];
                            if(p>0)
                            {
                                Lambda[k] -= p*lhs;
                                p          = 0;
                                if( !check_valid(Pk) )
                                    throw exception("degenerate constraints");
                            }
                        }
                    }
                    
                    //__________________________________________________________
                    //
                    // simplify Nu from fixed species
                    //__________________________________________________________
                    for(size_t k=N;k>0;--k)
                    {
                        Nu[k][s] = 0;
                    }
                    
                    
                }
                if(changed)
                {
                    std::cerr << "#has changed..." << std::endl;
                    std::cerr << "Nu=" << Nu << std::endl;
                    cs.find_active_species();
                    std::cerr << "P=" << P << std::endl;
                    std::cerr << "Lambda=" << Lambda << std::endl;
                }
                
                matrix_t P2(Nc,Nc);
                for(size_t i=Nc;i>0;--i)
                {
                    for(size_t j=i;j>0;--j)
                    {
                        double sum = 0;
                        for(size_t k=M;k>0;--k)
                        {
                            sum += P[i][k] * P[j][k];
                        }
                        P2[i][j] = P2[j][i] = sum;
                    }
                }
                std::cerr << "P2=" << P2 << std::endl;
                
                
            }
            catch(...)
            {
                cs.restore_topology();
                throw;
            }
        }
    }
}
