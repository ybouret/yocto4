#include "ui.h"
#include "yocto/math/fcn/zfind.hpp"

using namespace math;

namespace
{
    
    class Fluoresceine : public library
    {
    public:
        
        const double C0;
        const double V0;
        const double Cb;
        const double pH_min;
        const double pH_max;
        const double pH_ini;
        double       pH_usr;
        chemsys      cs;
        
        explicit Fluoresceine( const double C_user, const double V_user, const double C_base ) :
        C0(C_user),
        V0(V_user),
        Cb(C_base),
        pH_min(0),
        pH_max(0),
        pH_ini(0),
        pH_usr(7),
        cs(*this,1e-7)
        {
            //==================================================================
            //-- create library
            //==================================================================
            add("H+",   1);
            add("HO-", -1);
            add("FlH",  0);
            add("Fl-", -1);
            add("Na+",  1);
            add("Cl-", -1);
            
            
            //==================================================================
            // Create the chemical system
            //==================================================================
            {
                const double pKw   = 14;
                equilibrium &water = cs.create("water", Pow(10.0, -pKw) );
                water.add("H+",1);
                water.add("HO-",1);
            }
            
            {
                const double pKa  = 6.4;
                equilibrium &fluo = cs.create("fluoresceine", Pow(10.0,-pKa));
                fluo.add("FlH",-1);
                fluo.add("H+",1);
                fluo.add("Fl-",1);
            }
            
            cs.build();
            
#if 0
            for( chemsys::iterator i=cs.begin(); i != cs.end(); ++i)
            {
                const equilibrium &eq = **i;
                std::cerr << eq << std::endl;
            }
#endif

            
            
            (double &)pH_max = compute_pH(V0);
            (double &)pH_min = compute_pH(-V0);
            (double &)pH_ini = compute_pH(0);
        }
        
        double compute_pH( const double Vb )
        {
         
            if( Fabs(Vb) > V0)
                throw exception("|Vb|=%g exceeds V0=%g", Fabs(Vb), V0);
                       
            
            //==================================================================
            // Create the initializer
            //==================================================================
            initializer ini( *this );
            
            //------------------------------------------------------------------
            //-- electroneutrality
            //------------------------------------------------------------------
            ini.electroneutrality();
            
            //------------------------------------------------------------------
            //-- mass conservation
            //------------------------------------------------------------------
            {
                constraint &fluo = ini.create(C0);
                fluo.add("Fl-", 1);
                fluo.add("FlH",1);
            }
            
            //------------------------------------------------------------------
            // Base/Acid ?
            //------------------------------------------------------------------

            if(Vb<0)
            {
                const double ConcCl = (-Vb) * Cb / V0;
                constraint &Cl = ini.create(ConcCl);
                Cl.add("Cl-", 1);
                
                constraint &Na = ini.create(0);
                Na.add("Na+", 1);
            }
            else
            {
                const double ConcNa = Vb * Cb / V0;
                constraint &Na = ini.create(ConcNa);
                Na.add("Na+", 1);
                
                constraint &Cl = ini.create(0);
                Cl.add("Cl-", 1);
            }
            
            
            
            //==================================================================
            // Solve
            //==================================================================
            solution s(*this);
            
            //------------------------------------------------------------------
            //-- initialize cs.C at time=0
            //------------------------------------------------------------------
            ini(cs,0);
            
            //------------------------------------------------------------------
            //-- fetch
            //------------------------------------------------------------------
            s.get(cs.C);
            
            //std::cerr << s << std::endl;
            //std::cerr << "pH=" << s.pH() << std::endl;
            return s.pH();
        }
        
        double zero_pH( const double Vb )
        {
            return compute_pH(Vb) - pH_usr;
        }
        
        virtual ~Fluoresceine() throw()
        {
            
        }
        
        
        double compute_volume( const double pH )
        {
            if(pH<pH_min)
                throw exception("pH=%g<pH_min=%g",pH,pH_min);
            
            if(pH>pH_max)
                throw exception("pH=%g>pH_max=%g",pH,pH_max);
            
            if(pH<=pH_min)
                return -V0;
            
            if(pH>=pH_max)
                return  V0;
            
            pH_usr = pH;
            numeric<double>::function zpH( this, & Fluoresceine::zero_pH);
            zfind<double>             solve(0);
            
            return solve(zpH,-V0,V0);
        }
        
        
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Fluoresceine);
    };
}


int main(int argc, char *argv[] )
{
    const char *progname = vfs::get_base_name(argv[0]);
    try
    {
        MakeWindow();
        MainWindow->show();
        
        Fluoresceine fl(1e-2,0.1,1e-2);
        std::cerr << "===> pH_min=" << fl.pH_min << std::endl;
        std::cerr << "===> pH_max=" << fl.pH_max << std::endl;
        std::cerr << "===> pH_ini=" << fl.pH_ini << std::endl;

        const double V9 = fl.compute_volume(9.0);
        std::cerr << "V9=" << V9 << std::endl;
        
        return Fl::run();
    }
    catch( const exception &e )
    {
        fl_message_title("Exception");
        fl_alert("%s: %s\n%s",progname,e.what(),e.when());
    }
    catch(...)
    {
        fl_message_title("Exception");
        fl_alert("%s: unhandled exception!", progname);
    }
    return 1;
}

