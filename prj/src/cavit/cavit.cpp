#include "yocto/fs/local-fs.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/core/list.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/ocstream.hpp"

#include "yocto/math/types.hpp"
#include "yocto/math/ode/explicit/driver-ck.hpp"

#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-maths.hpp"

using namespace yocto;
using namespace math;

typedef v2d<double> vtx_t;

enum content_t
{
    is_liquid,
    is_gaz
};


class cavity : public object
{
public:
    const size_t i;
    const size_t j;
    vtx_t        r;
    content_t    content;
    double       lambda;
    double       t_boum;
    size_t       links;
    cavity      *link[8];
    
    typedef shared_ptr<cavity> pointer;
    
    explicit cavity(size_t ii, size_t jj) throw() :
    i(ii),
    j(jj),
    r(),
    content(is_liquid),
    lambda(1),
    t_boum(-1),
    links(0),
    link()
    {
        for(size_t i=0;i<sizeof(link)/sizeof(link[0]);++i)
        {
            link[i] = 0;
        }
    }
    
    virtual ~cavity() throw() {}
    
    void attach( cavity &oc ) throw()
    {
        assert(links<sizeof(link)/sizeof(link[0]));
#if !defined(NDEBUG)
        for(size_t i=0;i<links;++i)
        {
            assert( link[i] != &oc );
        }
#endif
        link[links++] = &oc;
    }
    
    class pnode
    {
    public:
        pnode  *prev;
        pnode  *next;
        cavity &cv;
        
        explicit pnode( cavity &ref ) throw() : prev(0), next(0), cv(ref) {}
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(pnode);
        ~pnode() throw();
    };
    
    class plist : public core::list_of<pnode>
    {
    public:
        explicit plist() throw() {}
        virtual ~plist() throw()
        {
            while(size)
            {
                object::release1<pnode>( pop_back() );
            }
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(plist);
    };
    
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(cavity);
};




class device : public vector< cavity::pointer >
{
public:
    const size_t                          nx;
    const size_t                          ny;
    uniform_generator<double,rand32_kiss> ran;
    double                                t_last;
    cavity::plist                         liquid;
    cavity::plist                         bubble;
    numeric<double>::function             lambda;
    numeric<double>::function             Lambda;
    
private:
    ode::Field<double>::Equation          eqdiff;
    vector<double>                        Y;
    ode::driverCK<double>::type           odeint;
    lua_State                            *L;
    Lua::Function<double>                 lambda0;
    Lua::Function<double>                 lambda1;

public:
    
    explicit device( size_t Nx, size_t Ny, lua_State *luaVM ) :
    nx(Nx),
    ny(Ny),
    ran(),
    t_last(0),
    liquid(),
    bubble(),
    lambda( this, & device::compute_lambda),
    Lambda( this, & device::compute_Lambda),
    eqdiff( this, & device::lambdaODE     ),
    Y(1,0.0),
    odeint(1e-7),
    L(luaVM),
    lambda0(L,"lambda0"),
    lambda1(L,"lambda1")
    {
        //-- memory
        reserve(nx*ny);
        
        //-- storage
        for(size_t i=0;i<nx;++i)
        {
            for(size_t j=0;j<ny;++j)
            {
                cavity               *cv = new cavity(i,j);
                const cavity::pointer p( cv );
                push_back(p);
                liquid.push_back( new (object::acquire1<cavity::pnode>()) cavity::pnode(*cv) );
            }
        }
        odeint.start(1);
        
        //-- link neighbors
        for(size_t k=size();k>0;--k)
        {
            cavity &cv = *(*this)[k];
            assert(0==cv.links);
            const size_t i=cv.i;
            const size_t j=cv.j;
            
            const size_t im=i-1;
            const size_t ip=i+1;
            const size_t jm=j-1;
            const size_t jp=j+1;
            
            for(size_t l=size();l>0;--l)
            {
                if( l != k )
                {
                    cavity &oc = *(*this)[l];
                    // on the left side
                    if( im == oc.i )
                    {
                        if(j==oc.j)
                        {
                            // left
                            cv.attach(oc);
                            continue;
                        }
                    }
                    
                    // on the right size
                    if( ip == oc.i )
                    {
                        if(j==oc.j)
                        {
                            //right
                            cv.attach(oc);
                            continue;
                        }
                    }
                    
                    // on the below side
                    if( jm == oc.j )
                    {
                        if(i==oc.i)
                        {
                            // below
                            cv.attach(oc);
                            continue;
                        }
                    }
                    
                    // on the above side
                    if( jp == oc.j )
                    {
                        if(i==oc.i)
                        {
                            // above
                            cv.attach(oc);
                            continue;
                        }
                    }
                }
            }
        }
        
#if 0
        //-- display
        for(size_t k=1;k<=size();++k)
        {
            const cavity &cv = *(*this)[k];
            std::cerr << "@(" << cv.i << "," << cv.j << "): #" << cv.links << std::endl;
        }
#endif
        reset();
    }
    
    virtual ~device() throw()
    {
    }
    
    
    void reset() throw()
    {
        t_last  = 0;
        size_t k=0;
        for(size_t i=0;i<nx;++i)
        {
            double x = i;
            for(size_t j=0;j<ny;++j)
            {
                double y = j;
                cavity &c = *(*this)[++k];
                c.r.x = x;
                c.r.y = y;
                c.content = is_liquid;
            }
        }
        while(bubble.size)
        {
            liquid.push_back(bubble.pop_back());
        }
    }
    
    
    cavity::pnode *find_next()
    {
        if(liquid.size<=0)
            return NULL;
        
        const double xi    =  ran();
        const double delta = -Log(xi);
        double       tau = 0;
        
        // compute next time !
        while(true)
        {
            const double num  = delta - Lambda(t_last+tau);
            const double lam  = lambda(t_last+tau);
            const double dtau = num/lam;
            tau+=dtau;
            if( fabs(lam*dtau)<1e-7)
                break;
        }
        
        // find the cavity
        t_last += tau;
        const double level = ran() * lambda(t_last);
        
        // find the guy
        cavity::pnode *node = liquid.head;
        double         curr_lam = 0;
        while(node->next)
        {
            const double next_lam = curr_lam + node->cv.lambda;
            if( level >= curr_lam && level < next_lam )
                break;
            curr_lam = next_lam;
            node = node->next;
        }
        
        node->cv.content = is_gaz;
        node->cv.t_boum  = t_last;
        bubble.push_back( liquid.unlink(node) );
        
        return node;
    }
    
    void save_xyz( const string &config, bool append) const
    {
        ios::ocstream cf(config,append);
        cf("%u\n", unsigned(size()));
        cf("t=%g\n", t_last);
        for(size_t i=1;i<=size();++i)
        {
            const cavity &cv = *(*this)[i];
            const char   *id = "";
            switch(cv.content)
            {
                case is_liquid: id = "Na"; break;
                case is_gaz:    id = "Cl"; break;
            }
            cf("%s %e %e 0\n", id, cv.r.x, cv.r.y);
        }
        
    }
    
    void run1( const string &rootname )
    {
        //vfs &fs = local_fs::instance();
        const string output = rootname + ".dat";
        const string config = rootname + ".xyz";
        reset();
        ios::ocstream fp(output,false);
        fp("0 0\n");
        save_xyz(config, false);
        while( find_next() )
        {
            fp("%e %e\n", t_last, double(bubble.size)/size());
            save_xyz(config, true);
        }
        
        
    }
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(device);
    
    inline double compute_lambda(double t)
    {
        assert(liquid.size>0);
        
        //-- the spontaneous probability
        const double lam0 = lambda0(t); assert(lam0>0);
        for( cavity::pnode *node = liquid.head; node; node=node->next )
        {
            // constant part
            cavity &cv = node->cv;
            cv.lambda  = lam0;
        }
        
        
        //-- the effect of neighbors
        for( cavity::pnode *node=bubble.head;node;node=node->next)
        {
            cavity &cv = node->cv;
            assert(t>=cv.t_boum);
            const double tau = t-cv.t_boum;
            for(size_t i=0;i<cv.links;++i)
            {
                cavity &oc = *cv.link[i];
                if(is_liquid == oc.content)
                {
                    const vtx_t r(cv.r,oc.r);
                    const double d    = r.norm();
                    const double lam1 = lambda1(tau,d);
                    assert(lam1>=0);
                    oc.lambda += lam1;
                }
            }
        }
        
        // final sum
        double sum = 0;
        for( cavity::pnode *node = liquid.head; node; node=node->next )
        {
            sum += node->cv.lambda;
        }
        
        if(sum<=0)
            throw exception("invalid lambda");
        
        return sum;
    }
    
    inline void lambdaODE( array<double> &dYdt, double t, const array<double> & )
    {
        dYdt[1] = compute_lambda(t);
    }
    
    inline double compute_Lambda(double t)
    {
        double ctrl = 1e-4;
        Y[1] = 0;
        odeint.operator()(eqdiff, Y, t_last, t, ctrl, 0);
        return Y[1];
    }
    
    
    
};



int main(int argc, char *argv[])
{
    const char * prog = vfs::get_base_name(argv[0]);
    
    try
    {
        if(argc<=1)
            throw exception("Usage: %s config.lua",prog);
        Lua::State VM;
        lua_State *L = VM();
        Lua::Config::DoFile(L, argv[1]);
        
        device D(20,30,L);
        std::cerr << "#liquid=" << D.liquid.size << std::endl;
        for(int iter=1;iter<=1;++iter)
        {
            D.run1( vformat("run%d", iter));
        }
        
        return 0;
    }
    catch(const exception &e )
    {
        std::cerr << "*** " << e.what() << std::endl;
        std::cerr << "*** " << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "exception in " << prog << std::endl;
    }
    return -1;
}