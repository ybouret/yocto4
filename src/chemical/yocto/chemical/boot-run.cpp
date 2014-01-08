#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/ios/ocstream.hpp"


namespace yocto
{
    namespace chemical
    {
        
        namespace {
            
            typedef math::algebra<double> mkl;
            
            class boot_solver
            {
            public:
                ~boot_solver() throw() {}
                
                equilibria  &cs;
                const double t;
                const size_t M;  //!< lib.size
                const size_t N;  //!< cs.size
                const size_t Nc; //!< ini.size
                matrix_t     P;
                vector_t     Lam;
                vector_t     Mu;  //!< to store PX
                matrix_t     P2; //!< P*P' then LU
                lu_t         LU;
                
                boot_solver(equilibria       &usr_cs,
                            const double      usr_t,
                            boot::loader     &ini,
                            collection       &lib ) :
                cs(usr_cs),
                t(usr_t),
                M( lib.size() ),
                N( cs.size()  ),
                Nc( ini.size() ),
                P(),
                Lam(),
                Mu(),
                P2(),
                LU()
                {
                    
                    static const double FTOL = math::numeric<double>::ftol;
                    if( Nc+N != M )
                        throw exception("#species=%u != (#equilibria=%u+#constraints=%u)", unsigned(M), unsigned(N), unsigned(Nc) );
                    
                    //__________________________________________________________
                    //
                    // initializing stuff
                    //__________________________________________________________
                    cs.build_from(lib);
                    
                    //__________________________________________________________
                    //
                    // no species
                    //__________________________________________________________
                    if(M<=0)
                        return;
                    
                    //__________________________________________________________
                    //
                    // no equilbrium
                    //__________________________________________________________
                    if(N<=0)
                    {
                        assert(Nc==M);
                        //------------------------------------------------------
                        // the P matrix squared
                        //------------------------------------------------------
                        P.make(M,M);
                        LU.ensure(M);
                        ini.fill(P, cs.C);
                        std::cerr << "P=" << P << std::endl;
                        std::cerr << "Lam=" << cs.C << std::endl;
                        if( !LU.build(P) )
                            throw exception("invalid chemical constraints");
                        LU.solve(P,cs.C);
                        cs.cleanup_C();
                        return;
                    }
                    
                    //__________________________________________________________
                    //
                    // memory
                    //__________________________________________________________
                    P.     make(Nc,M);
                    Lam.   make(Nc,0);
                    Mu.    make(Nc,0);
                    P2.    make(Nc,Nc);
                    LU.    ensure(Nc);     //! to inverse P2
                    
                    //__________________________________________________________
                    //
                    // constant part
                    //__________________________________________________________
                    ini.fill(P, Lam);
                    std::cerr << "P=" << P << std::endl;
                    std::cerr << "Lam=" << Lam << std::endl;
                    mkl::mul_rtrn(P2, P, P);
                    if( !LU.build(P2) )
                        throw exception("singular chemical constraints");
                    
                    //__________________________________________________________
                    //
                    // scale all reactions
                    //__________________________________________________________
                    cs.scale_all(t);
                    
                    
                    //__________________________________________________________
                    //
                    // algorithm
                    //__________________________________________________________
                    static const size_t MAX_STEP_PER_SPECIES = 4;
                    const size_t steps_before_rms = max_of<size_t>(2,MAX_STEP_PER_SPECIES * M);
                    
                    static const size_t MAX_FAILURES = 10;
                    size_t num_failures = 0;
                    
                    vector_t dX(M,0);
                    
                INITIALIZE:
                    // build a random valid concentration set
                    if(num_failures>MAX_FAILURES)
                        throw exception("too many failures in chemical boot: check constraints");
                    
                    cs.trial(ini.ran, t);
                    size_t count   =  0;
                    double old_rms = -1;
                    
                VIRTUAL_STEP:
                    ++count;
                    //----------------------------------------------------------
                    // from a normalized C, compute the external source
                    //----------------------------------------------------------
                    build_dC();
                    //std::cerr << "sigma=" << cs.dC << std::endl;
                    
                    //----------------------------------------------------------
                    // legalize the source
                    //----------------------------------------------------------
                    cs.legalize_dC(t,false);
                    //std::cerr << "dC=" << cs.dC << std::endl;
                    
                    //----------------------------------------------------------
                    // save old position
                    //----------------------------------------------------------
                    mkl::set(dX,cs.C);
                    
                    //----------------------------------------------------------
                    // update/clear
                    //----------------------------------------------------------
                    mkl::add(cs.C, cs.dC);
                    cs.cleanup_C();
                    if( mkl::sum(cs.C) <= 0 )
                    {
                        std::cerr << "Invalid Composition after step!" << std::endl;
                        ++num_failures;
                        goto INITIALIZE;
                    }
                    
                    //----------------------------------------------------------
                    // normalize
                    //----------------------------------------------------------
                    cs.normalize_C(t);
                    //std::cerr<< "C=" << cs.C << std::endl;
                    
                    //----------------------------------------------------------
                    //compute effective step
                    //----------------------------------------------------------
                    mkl::sub(dX, cs.C);
                    //std::cerr << "dX=" << dX << std::endl;
                    
                    //----------------------------------------------------------
                    //check cycling
                    //----------------------------------------------------------
                    const double rms = __RMS(dX);
                    if(count>steps_before_rms)
                    {
                        assert(old_rms>=0);
                        if(rms>=old_rms)
                            goto FINALIZE;
                    }
                    old_rms = rms;
                    
                    //----------------------------------------------------------
                    //check convergence
                    //----------------------------------------------------------
                    for(size_t i=M;i>0;--i)
                    {
                        double err = fabs(dX[i]);
                        if(err<=cs.tiny) err=0;
                        if( err > FTOL * fabs(cs.C[i]))
                            goto VIRTUAL_STEP;
                    }
                   
                FINALIZE:
                    //__________________________________________________________
                    //
                    // done
                    //__________________________________________________________
                    //std::cerr << "Done" << std::endl;
                    //std::cerr << "Cn=" << cs.C << std::endl;
                    
                    //__________________________________________________________
                    //
                    // Check Errors for rounding
                    //__________________________________________________________
                    build_dC();
                    vector_t &Y = cs.dC;
                    //std::cerr << "Y1=" << Y << std::endl;
                    for(size_t i=M;i>0;--i)
                    {
                        double err = fabs(Y[i]);
                        if(err>0)
                        {
                            err = pow(10,ceil(log10(err)));
                        }
                        Y[i] = err;
                    }
                    //std::cerr << "Y2=" << Y << std::endl;
                    for(size_t i=M;i>0;--i)
                    {
                        if( cs.C[i] <= Y[i] ) cs.C[i] = 0;
                    }
                    cs.normalize_C(t);
                    //std::cerr << "C=" << cs.C << std::endl;

                }
                
                //! dC = P'*(P*P')^(-1)(Lam - PC)
                inline void build_dC() throw()
                {
                    mkl::mul(Mu,P,cs.C);      // Mu = PC
                    mkl::subp(Mu,Lam);        // Mu = Lam - PC
                    LU.solve(P2,Mu);          // Mu = (P*P')^(-1)(Lam - PC)
                    mkl::mul_trn(cs.dC,P,Mu); // dC = P'*(P*P')^(-1)(Lam - PC)
                }
                
                static inline double __RMS(const array<double> &v) throw()
                {
                    double rms = 0;
                    const size_t n = v.size();
                    for(size_t i=n;i>0;--i)
                    {
                        const double tmp = v[i];
                        rms += tmp * tmp;
                    }
                    return sqrt(rms/n);
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(boot_solver);
            };
            
        }
        
        
        void boot::loader::operator()(equilibria &cs, collection &lib, double t)
        {
            boot_solver solve(cs,t,*this,lib);
        }
        
        
    }
}
