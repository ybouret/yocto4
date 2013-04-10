#include "yocto/core/clist.hpp"
#include "yocto/geom/v2d.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/math/kernel/lu.hpp"

#include <iostream>

using namespace yocto;
using namespace math;

typedef geom::v2d<double> vertex;

class point
{
public:
    vertex v;
    bool   alive;
    point *next;
    point *prev;
    
    point() throw() : v(), alive(true),next(0),prev(0) {}
    ~point() throw() {}
    
    point(const point &p) : v(p.v), alive(p.alive),next(0),prev(0) {}
    
private:
    YOCTO_DISABLE_ASSIGN(point);
};

class snake : public core::clist_of<point>
{
public:
    matrix<double> mat;
    vector<double> b;
    lu<double>     LU;
    
    explicit snake() : mat(2,2), b(2,0.0), LU(2)
    {
    }
    
    void kill() throw()
    {
        while(size) delete pop_back();
    }
    
    virtual ~snake() throw()
    {
        kill();
    }
    
    void initialize( const array<point> &points )
    {
        assert(points.size()>0);
        double xmin = points[1].v.x;
        double xmax = xmin;
        double ymin = points[1].v.y;
        double ymax = ymin;
        
        {
            ios::ocstream fp("points.dat",false);
            for( size_t i=1;i<=points.size();++i)
            {
                const point  &p   = points[i]; assert(p.alive);
                const vertex &v   = p.v;
                if(v.x<xmin) xmin = v.x;
                if(v.x>xmax) xmax = v.x;
                if(v.y<ymin) ymin = v.y;
                if(v.y>ymax) ymax = v.y;
                fp("%g %g\n", v.x, v.y);
            }
        }
        
        {
            point *p = 0;
            p = new point; p->v.x = xmin; p->v.y=ymin; push_back(p);
            p = new point; p->v.x = xmin; p->v.y=ymax; push_back(p);
            p = new point; p->v.x = xmax; p->v.y=ymax; push_back(p);
            p = new point; p->v.x = xmax; p->v.y=ymin; push_back(p);
        }
        
    }
    
    void save( const string &fn ) const
    {
        ios::ocstream fp(fn,false);
        const point *p = root;
        for(size_t i=size;i>0;--i,p=p->next)
        {
            fp("%g %g\n", p->v.x, p->v.y);
        }
        fp("%g %g\n", p->v.x, p->v.y);
    }
    
    vertex get_G() const
    {
        vertex G;
        const point *p = root;
        for(size_t i=size;i>0;--i,p=p->next)
        {
            G += p->v;
        }
        return G/double(size);
    }
    
    bool refine( point *p, array<point> &points)
    {
        assert(p);
        assert( owns(p) );
        const vertex &P = p->v;
        point        *q = p->next;
        const vertex &Q = q->v;
        const vertex  G = get_G();
        const vertex  PQ(P,Q);
        const vertex  QG(Q,G);
        mat[1][1] = PQ.x;
        mat[1][2] = QG.x;
        mat[2][1] = PQ.y;
        mat[2][2] = QG.y;
        if( !LU.build(mat) )
        {
            std::cerr << "Singular matrix" << std::endl;
            return false;
        }
        
        for( size_t i=1; i <= points.size(); ++i )
        {
            point &tmp = points[i];
            const vertex PM(P,tmp.v);
            b[1] = PM.x;
            b[2] = PM.y;
            LU.solve(mat, b);
            const double lam = b[1];
            const double lammu = b[2];
            std::cerr << "lam=" << lam << ", lammu="  << lammu << std::endl;
        }
        
        return false;
    }
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(snake);
};

int main(int argc, char *argv[] )
{
    try
    {
        _rand.wseed();
        snake  s;
        const size_t  n = 10;
        vector<point> points(n,as_capacity);
        for( size_t i=0; i <n; ++i)
        {
            point p;
            p.v.x = 10*( alea<double>() - 0.5 );
            p.v.y = 10*( alea<double>() - 0.5 );
            points.push_back(p);
        }
        
        s.initialize(points);
        s.save("bb.dat");
        
        s.refine(s.root, points);
        
        return 0;
    }
    catch(...)
    {
        std::cerr << "Unhandled exception!" << std::endl;
    }
    return 1;
}