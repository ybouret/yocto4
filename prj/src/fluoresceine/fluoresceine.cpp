#include "ui.h"


namespace
{
    
    class Fluoresceine : public library
    {
    public:
        
        const double C0;
        const double V0;
        const double Cb;
        explicit Fluoresceine( const double C_user, const double V_user, const double C_base ) :
        C0(C_user),
        V0(V_user),
        Cb(C_base)
        {
            //-- create library
            add("H+",   1);
            add("HO-", -1);
            add("FlH",  0);
            add("Fl-", -1);
            add("Na+",  1);
            add("Cl-", -1);
        }
        
        double compute_pH( const double Vb )
        {
            chemsys cs( *this, 1e-7 );
            
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
            for( chemsys::iterator i=cs.begin(); i != cs.end(); ++i)
            {
                const equilibrium &eq = **i;
                std::cerr << eq << std::endl;
            }
            
            
            initializer ini( *this );
            
            //-- electroneutrality
            ini.electroneutrality();
            
            //-- mass conservation
            {
                constraint &fluo = ini.create(C0);
                fluo.add("Fl-", 1);
                fluo.add("FlH",1);
            }
            
            //-- no chloride
            {
                constraint &Cl = ini.create(0);
                Cl.add("Cl-", 1);
            }
            
            //-- base => Na+
            {
                const double ConcNa = Vb * Cb / V0;
                constraint &Na = ini.create(ConcNa);
                Na.add("Na+", 1);
            }
            
            std::cerr << ini << std::endl;
            
            //-- create empty solution
            solution s(*this);
            
            //-- initialize cs.C
            ini(cs,0);
            
            //-- fetch
            s.get(cs.C);
            
            std::cerr << s << std::endl;
            std::cerr << "pH=" << s.pH() << std::endl;
            return 1;
        }
        
        virtual ~Fluoresceine() throw()
        {
            
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
        fl.compute_pH(0);
        
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

