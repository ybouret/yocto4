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
                    if(M<=0)
                        return;
                    
                    if(N<=0)
                    {
                        
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
                    std::cerr << "P2=" << P2 << std::endl;
                    if( !LU.build(P2) )
                        throw exception("singular chemical constraints");
                    
                    // scale all reactions
                    cs.scale_all(t);
                    
                    
                    //__________________________________________________________
                    //
                    // algorithm
                    //__________________________________________________________
                    vector_t dX(M,0);

                    // build a random valid concentration set
                    cs.trial(ini.ran, t);
                    std::cerr << "C=" << cs.C << std::endl;
                    std::cerr << std::endl;
                    
                    
                VIRTUAL_STEP:
                    //----------------------------------------------------------
                    // from a normalized C, compute the external source
                    //----------------------------------------------------------
                    build_dC();
                    //std::cerr << "sigma=" << cs.dC << std::endl;
                    
                    //----------------------------------------------------------
                    // legalize the source
                    //----------------------------------------------------------
                    cs.legalize_dC(t,false);
                    //std::cerr << "dC   =" << cs.dC << std::endl;
                    
                    //----------------------------------------------------------
                    // save old position
                    //----------------------------------------------------------
                    mkl::set(dX,cs.C);
                    
                    //----------------------------------------------------------
                    // update/clear
                    //----------------------------------------------------------
                    mkl::add(cs.C, cs.dC);
                    cs.cleanup_C();
                    //std::cerr << "C=" << cs.C << std::endl;
                    if( mkl::sum(cs.C) <= 0 )
                    {
                        std::cerr << "Invalid Composition after step!" << std::endl;
                        exit(1);
                    }
                    
                    //----------------------------------------------------------
                    // normalize
                    //----------------------------------------------------------
                    cs.normalize_C(t);
                    std::cerr<< "C=" << cs.C << std::endl;
                    
                    //----------------------------------------------------------
                    //compute effective step
                    //----------------------------------------------------------
                    mkl::sub(dX, cs.C);
                    std::cerr << "dX=" << dX << std::endl;
                    for(size_t i=M;i>0;--i)
                    {
                        double err = fabs(dX[i]);
                        if(err<=cs.tiny) err=0;
                        if( err > FTOL * fabs(cs.C[i]))
                            goto VIRTUAL_STEP;
                    }

                    //----------------------------------------------------------
                    // done
                    //----------------------------------------------------------
                    std::cerr << "Done" << std::endl;
                    std::cerr << "C=" << cs.C << std::endl;

                }
                
                //! dC = P'*(P*P')^(-1)(Lam - PC)
                inline void build_dC() throw()
                {
                    mkl::mul(Mu,P,cs.C);      // Mu = PC
                    mkl::subp(Mu,Lam);        // Mu = Lam - PC
                    LU.solve(P2,Mu);          // Mu = (P*P')^(-1)(Lam - PC)
                    mkl::mul_trn(cs.dC,P,Mu); // dC = P'*(P*P')^(-1)(Lam - PC)
                }
                
                //! find max acceptable step
                inline double step_max(bool &full) const throw()
                {
                    assert(true==full);
                    double shrink = 1;
                    for(size_t i=M;i>0;--i)
                    {
                        const double dC = cs.dC[i];
                        if( dC < 0 )
                        {
                            const double D  = -dC;     assert(D>0);
                            const double C  = cs.C[i];
                            if(D>C)
                            {
                                shrink  = min_of<double>(shrink,C/D);
                                full = false;
                            }
                        }
                    }
                    if(!full)
                    {
                        std::cerr << "\t[DECREASED]" << std::endl;
                        }
                        else
                        {
                            std::cerr << "\t[FULL STEP]" << std::endl;
                        }
                        
                        return shrink;
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
