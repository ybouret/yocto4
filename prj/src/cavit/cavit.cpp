#include "yocto/math/fcn/intg.hpp"
#include "yocto/fs/vfs.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/core/list.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/exception.hpp"

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
    vtx_t     r;
    content_t content;
    double    lambda;
    double    t_boum;
    
    typedef shared_ptr<cavity> pointer;
    
    explicit cavity() throw() :
    r(),
    content(is_liquid),
    lambda(1),
    t_boum(-1)
    {}
    
    virtual ~cavity() throw() {}

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
            while(size) object::release1<pnode>( pop_back() );
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
    const size_t nx;
    const size_t ny;
    double       t_last;
    
    explicit device( size_t Nx, size_t Ny ) :
    nx(Nx),
    ny(Ny),
    t_last(0),
    liquid(),
    bubble(),
    lambda( this, & device::compute_lambda),
    Lambda( this, & device::compute_Lambda),
    ran(),
    intg()
    {
        reserve(nx*ny);
        for(size_t i=0;i<nx;++i)
        {
            for(size_t j=0;j<ny;++j)
            {
                cavity               *cv = new cavity();
                const cavity::pointer p( cv );
                push_back(p);
                liquid.push_back( new (object::acquire1<cavity::pnode>()) cavity::pnode(*cv) );
            }
        }
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
        while(bubble.size) liquid.push_back(bubble.pop_back());
    }
    
    cavity::plist liquid;
    cavity::plist bubble;
    
    cavity::pnode *find_next()
    {
        if(liquid.size<=0)
            return NULL;
        
        const double xi    = ran.get<double>();
        const double delta = -Log(xi);
        double       tau = 0;
        
        size_t count = 0;
        while(true)
        {
            const double num = delta - Lambda(tau);
            const double den = lambda(tau);
            const double dtau = num/den;
            tau+=dtau;
            std::cerr << "tau=" << tau << "; dtau=" <<dtau << std::endl;
            
            if(++count>10)
                break;
        }
        
        return 0;
    }
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(device);
    numeric<double>::function lambda;
    numeric<double>::function Lambda;

    rand32_kiss               ran;
    integrator<double>        intg;
    
    double compute_lambda(double t)
    {
        double sum = 0;
        assert(liquid.size>0);
        for( cavity::pnode *node = liquid.head; node; node=node->next )
        {
            // constant part
            cavity &cv = node->cv;
            cv.lambda = 1;
            
            sum += cv.lambda;
        }
        if(sum<=0)
            throw exception("invalid lambda");
        
        return sum;
    }
    
    double compute_Lambda(double t)
    {
        return intg(t_last,t, lambda, 1e-7 );
    }

    
    
};



int main(int argc, char *argv[])
{
    const char * prog = vfs::get_base_name(argv[0]);
    
    try
    {
        
        device D(12,15);
        std::cerr << "#liquid=" << D.liquid.size << std::endl;
        
        D.find_next();
        
        return 0;
    }
    catch(...)
    {
        std::cerr << "exception in " << prog << std::endl;
    }
    return -1;
}