#include "ui.h"
#include "yocto/math/fcn/zfind.hpp"
#include "yocto/string/conv.hpp"
#include "yocto/chemical/boot.hpp"
#include "yocto/chemical/equilibria.hpp"

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
        equilibria   cs;
        
        explicit Fluoresceine( const double C_user, const double V_user, const double C_base ) :
        C0(C_user),
        V0(V_user),
        Cb(C_base),
        pH_min(0),
        pH_max(0),
        pH_ini(0),
        pH_usr(7),
        cs()
        {
            //==================================================================
            //-- create library
            //==================================================================
            add("H+",   1); // 1
            add("HO-", -1); // 2
            add("FlH",  0); // 3
            add("Fl-", -1); // 4
            add("Na+",  1); // 5
            add("Cl-", -1); // 6
            
            library &lib = *this;
            
            //==================================================================
            // Create the chemical system
            //==================================================================
            {
                const double pKw   = 14;
                equilibrium &water = cs.add("water", Pow(10.0, -pKw) );
                water.add( lib["H+"], 1);
                water.add( lib["HO-"],1);
            }
            
            {
                const double pKa  = 6.4;
                equilibrium &fluo = cs.add("fluoresceine", Pow(10.0,-pKa));
                fluo.add( lib["FlH"],-1);
                fluo.add( lib["H+"],  1);
                fluo.add( lib["Fl-"], 1);
            }
            
            cs.startup(lib);
            
            
            
            (double &)pH_max = compute_pH(V0);
            (double &)pH_min = compute_pH(-V0);
            (double &)pH_ini = compute_pH(0);
        }
        
        double compute_pH( const double Vb )
        {
            
            if( Fabs(Vb) > V0)
                throw exception("|Vb|=%g exceeds V0=%g", Fabs(Vb), V0);
            
            // TODO: reload pKa
            
            //==================================================================
            // Create the initializer
            //==================================================================
            boot  ini;
            
            //------------------------------------------------------------------
            //-- electroneutrality
            //------------------------------------------------------------------
            collection &lib = *this;
            ini.electroneutrality(lib);
            
            //------------------------------------------------------------------
            //-- mass conservation
            //------------------------------------------------------------------
            {
                ini.conserve( lib["Fl-"], lib["FlH"], C0);
            }
            
            //------------------------------------------------------------------
            // Base/Acid ?
            //------------------------------------------------------------------
            
            if(Vb<0)
            {
                const double ConcCl = (-Vb) * Cb / V0;
                ini.conserve( lib["Cl-"], ConcCl );
                ini.conserve( lib["Na+"], 0      );
            }
            else
            {
                const double ConcNa = Vb * Cb / V0;
                ini.conserve( lib["Na+"], ConcNa );
                ini.conserve( lib["Cl-"], 0 );
            }
            
            std::cerr << "ini=" << std::endl;
            std::cerr << ini << std::endl;
            
            
            //==================================================================
            // Solve
            //==================================================================
            solution s(*this);
            
            //------------------------------------------------------------------
            //-- initialize cs.C at time=0
            //------------------------------------------------------------------
            ini(s,lib,cs,0.0);
            
            
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
            zfind<double>             solve(1e-3);
            
            return solve(zpH,-V0,V0);
        }
        
        
        static Fluoresceine *load()
        {
            const double C_user = strconv::to<double>( input_C0->value(), "C0");
            const double V_user = strconv::to<double>( input_V0->value(), "V0");
            const double C_base = strconv::to<double>( input_Cb->value(), "Cb");
            
            return new Fluoresceine(C_user,V_user,C_base);
            
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Fluoresceine);
    };
}

#include "yocto/ptr/auto.hpp"

static inline
void set_limits( const Fluoresceine &fluo )
{
    {
        const string smin = vformat("%.2f", fluo.pH_min);
        output_min->value( smin.c_str() );
    }
    
    {
        const string sini = vformat("%.2f", fluo.pH_ini);
        output_ini->value( sini.c_str() );
    }
    
    {
        const string smax = vformat("%.2f", fluo.pH_max);
        output_max->value( smax.c_str() );
    }
}

void evaluate_limits()
{
    auto_ptr<Fluoresceine> fluo( Fluoresceine::load() );
    set_limits( *fluo );
}

void compute_volume()
{
    try
    {
        auto_ptr<Fluoresceine> fluo( Fluoresceine::load() );
        set_limits( *fluo );
        output_Vb->value("N/A");
        
        const double pH = strconv::to<double>( input_pH->value(), "pH" );
        const double Vb = fluo->compute_volume( pH );
        const string s  = vformat("%.3f", Vb);
        output_Vb->value( s.c_str() );
    }
    catch( const exception &e )
    {
        fl_message_title("Exception");
        fl_alert("compute_volumes: %s\n%s",e.what(),e.when());
    }
    catch(...)
    {
        throw; // fatal
    }
}

#include "yocto/code/rand.hpp"

int main(int argc, char *argv[] )
{
    const char *progname = vfs::get_base_name(argv[0]);
    try
    {
        MakeWindow();
        input_pKw->value("14");
        input_pKa->value("6.4");
        
        input_C0->value("0.01");
        input_V0->value("100");
        input_Cb->value("0.01");
        
        input_pH->value("7.00");
        output_Vb->value("N/A");
        
        output_min->value("N/A");
        output_ini->value("N/A");
        output_max->value("N/A");
        
        
        MainWindow->show();
        
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

