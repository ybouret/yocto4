#include "yocto/math/alg/delaunay.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/types.hpp"
#include "yocto/ptr/auto-arr.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/sort/heap.hpp"
#include "yocto/exception.hpp"

#include "yocto/ios/ocstream.hpp"


#include <cstdlib>

namespace yocto
{
    namespace math
    {
        
        namespace
        {
            
            static inline
            real_t DistSq(const real_t x1,
                          const real_t y1,
                          const real_t x2,
                          const real_t y2
                          ) throw()
            {
                const real_t dx = x2 - x1;
                const real_t dy = y2 - y1;
                return dx*dx + dy*dy;
            }
            
#define EPSILON         REAL(0.000001)
            
            /*
             Return TRUE if a point (xp,yp) is inside the circumcircle made up
             of the points (x1,y1), (x2,y2), (x3,y3)
             The circumcircle centre is returned in (xc,yc) and the radius r
             NOTE: A point on the edge is inside the circumcircle
             */
            static inline
            bool CircumCircle(const real_t xp,
                              const real_t yp,
                              const real_t x1,
                              const real_t y1,
                              const real_t x2,
                              const real_t y2,
                              const real_t x3,
                              const real_t y3,
                              real_t &xc,
                              real_t &yc,
                              real_t &rsqr) throw()
            {
                const real_t fabsy1y2 = Fabs(y1-y2);
                const real_t fabsy2y3 = Fabs(y2-y3);
                
                /* Check for coincident points */
                if (fabsy1y2 < EPSILON && fabsy2y3 < EPSILON)
                    return(false);
                
                if (fabsy1y2 < EPSILON)
                {
                    const real_t m2 = - (x3-x2) / (y3-y2);
                    const real_t mx2 = (x2 + x3) / REAL(2.0);
                    const real_t my2 = (y2 + y3) / REAL(2.0);
                    xc = (x2 + x1) / REAL(2.0);
                    yc = m2 * (xc - mx2) + my2;
                }
                else if (fabsy2y3 < EPSILON)
                {
                    const real_t m1  = - (x2-x1) / (y2-y1);
                    const real_t mx1 = (x1 + x2) / REAL(2.0);
                    const real_t my1 = (y1 + y2) / REAL(2.0);
                    xc  = (x3 + x2) / REAL(2.0);
                    yc  = m1 * (xc - mx1) + my1;
                }
                else
                {
                    const real_t m1 = - (x2-x1) / (y2-y1);
                    const real_t m2 = - (x3-x2) / (y3-y2);
                    const real_t mx1 = (x1 + x2) / REAL(2.0);
                    const real_t mx2 = (x2 + x3) / REAL(2.0);
                    const real_t my1 = (y1 + y2) / REAL(2.0);
                    const real_t my2 = (y2 + y3) / REAL(2.0);
                    xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
                    if (fabsy1y2 > fabsy2y3)
                    {
                        yc = m1 * (xc - mx1) + my1;
                    } else
                    {
                        yc = m2 * (xc - mx2) + my2;
                    }
                }
                
                
                rsqr               = DistSq(x2, y2, xc, yc);
                const real_t drsqr = DistSq(xp, yp, xc, yc);
                
                //return((drsqr <= rsqr) ? TRUE : FALSE);
                return((drsqr - rsqr) <= EPSILON ? true : false);
            }
            
            
            struct IEDGE
            {
                int p1, p2;
            };
            
            struct XYZ
            {
                real_t x,y,z;
            };
            
            struct ITRIANGLE
            {
                size_t p1, p2, p3;
            };
            
            /*
             Triangulation subroutine
             Takes as input NV vertices in array pxyz
             Returned is a list of ntri triangular faces in the array v
             These triangles are arranged in a consistent clockwise order.
             The triangle array 'v' should be malloced to 3 * nv
             The vertex array pxyz must be big enough to hold 3 more points
             The vertex array must be sorted in increasing x values
             */
            static inline
            size_t Triangulate(const size_t     nv,
                               XYZ             *pxyz,
                               ITRIANGLE       *v)
            {
                
                assert(nv>0);
                static const size_t edges_init = 256;
                static const size_t edges_step = 128;
                
                real_t xp,yp,x1,y1,x2,y2,x3,y3,xc,yc,r;
                real_t xmin,xmax,ymin,ymax,xmid,ymid;
                real_t dx,dy,dmax;
                
                /* Allocate memory for the completeness list, flag for each triangle */
                const size_t   trimax = 4 * nv; // or 3 ?
                auto_arr<bool> complete(trimax);
                
                /* Allocate memory for the edge list */
                auto_arr<IEDGE> edges( edges_init );
                
                /*
                 Find the maximum and minimum vertex bounds.
                 This is to allow calculation of the bounding triangle
                 */
                xmin = pxyz[0].x;
                ymin = pxyz[0].y;
                xmax = xmin;
                ymax = ymin;
                for(size_t i=1;i<nv;i++) {
                    if (pxyz[i].x < xmin) xmin = pxyz[i].x;
                    if (pxyz[i].x > xmax) xmax = pxyz[i].x;
                    if (pxyz[i].y < ymin) ymin = pxyz[i].y;
                    if (pxyz[i].y > ymax) ymax = pxyz[i].y;
                }
                dx = xmax - xmin;
                dy = ymax - ymin;
                dmax = (dx > dy) ? dx : dy;
                xmid = (xmax + xmin) / REAL(2.0);
                ymid = (ymax + ymin) / REAL(2.0);
                
                /*
                 Set up the supertriangle
                 This is a triangle which encompasses all the sample points.
                 The supertriangle coordinates are added to the end of the
                 vertex list. The supertriangle is the first triangle in
                 the triangle list.
                 */
                pxyz[nv+0].x = xmid - REAL(20.0) * dmax;
                pxyz[nv+0].y = ymid - dmax;
                pxyz[nv+0].z = REAL(0.0);
                pxyz[nv+1].x = xmid;
                pxyz[nv+1].y = ymid + REAL(20.0) * dmax;
                pxyz[nv+1].z = REAL(0.0);
                pxyz[nv+2].x = xmid + REAL(20.0) * dmax;
                pxyz[nv+2].y = ymid - dmax;
                pxyz[nv+2].z = REAL(0.0);
                v[0].p1      = int(nv);
                v[0].p2      = int(nv+1);
                v[0].p3      = int(nv+2);
                complete[0]  = false;
                size_t ntri  = 1;
                
#if 0
                {
                    ios::ocstream fp("supertri.dat",false);
                    fp("%g %g %g\n", pxyz[v[0].p1].x,pxyz[v[0].p1].y,pxyz[v[0].p1].z);
                    fp("%g %g %g\n", pxyz[v[0].p2].x,pxyz[v[0].p2].y,pxyz[v[0].p2].z);
                    fp("%g %g %g\n", pxyz[v[0].p3].x,pxyz[v[0].p3].y,pxyz[v[0].p3].z);
                    fp("%g %g %g\n", pxyz[v[0].p1].x,pxyz[v[0].p1].y,pxyz[v[0].p1].z);
                }
#endif
                /*
                 Include each point one at a time into the existing mesh
                 */
                for(size_t i=0;i<nv;i++)
                {
                    
                    xp = pxyz[i].x;
                    yp = pxyz[i].y;
                    size_t nedge = 0;
                    
                    /*
                     Set up the edge buffer.
                     If the point (xp,yp) lies inside the circumcircle then the
                     three edges of that triangle are added to the edge buffer
                     and that triangle is removed.
                     */
                    for(size_t j=0;j<ntri;j++)
                    {
                        if (complete[j])
                            continue;
                        x1 = pxyz[v[j].p1].x;
                        y1 = pxyz[v[j].p1].y;
                        x2 = pxyz[v[j].p2].x;
                        y2 = pxyz[v[j].p2].y;
                        x3 = pxyz[v[j].p3].x;
                        y3 = pxyz[v[j].p3].y;
                        xc=yc=r=0;
                        const bool inside = CircumCircle(xp,yp,x1,y1,x2,y2,x3,y3,xc,yc,r);
                        if (xc < xp && ((xp-xc)*(xp-xc)) > r)
                            complete[j] = true;
                        if (inside)
                        {
                            /* Check that we haven't exceeded the edge list size */
                            if (nedge+3 >= edges.size)
                            {
                                edges.reserve( edges_step );
                            }
                            
                            /* register the edges */
                            edges[nedge+0].p1 = v[j].p1;
                            edges[nedge+0].p2 = v[j].p2;
                            edges[nedge+1].p1 = v[j].p2;
                            edges[nedge+1].p2 = v[j].p3;
                            edges[nedge+2].p1 = v[j].p3;
                            edges[nedge+2].p2 = v[j].p1;
                            nedge += 3;
                            v[j] = v[ntri-1];
                            complete[j] = complete[ntri-1];
                            --ntri;
                            --j;
                        }
                    }
                    
                    /*
                     Tag multiple edges
                     Note: if all triangles are specified anticlockwise then all
                     interior edges are opposite pointing in direction.
                     */
                    for(size_t j=0;j<nedge-1;j++)
                    {
                        for(size_t k=j+1;k<nedge;k++)
                        {
                            if ((edges[j].p1 == edges[k].p2) && (edges[j].p2 == edges[k].p1))
                            {
                                edges[j].p1 = -1;
                                edges[j].p2 = -1;
                                edges[k].p1 = -1;
                                edges[k].p2 = -1;
                            }
                            
                            /* Shouldn't need the following, see note above */
                            if ((edges[j].p1 == edges[k].p1) && (edges[j].p2 == edges[k].p2))
                            {
                                edges[j].p1 = -1;
                                edges[j].p2 = -1;
                                edges[k].p1 = -1;
                                edges[k].p2 = -1;
                            }
                        }
                    }
                    
                    /*
                     Form new triangles for the current point
                     Skipping over any tagged edges.
                     All edges are arranged in clockwise order.
                     */
                    for(size_t j=0;j<nedge;j++)
                    {
                        if (edges[j].p1 < 0 || edges[j].p2 < 0)
                            continue;
                        
                        if(ntri >= trimax)
                        {
                            throw exception("Delaunay: #triangles exceeds theoretical max!");
                        }
                        
                        v[ntri].p1 = edges[j].p1;
                        v[ntri].p2 = edges[j].p2;
                        v[ntri].p3 = i;
                        complete[ntri] = false;
                        ++ntri;
                    }
                }
                
                /*
                 Remove triangles with supertriangle vertices
                 These are triangles which have a vertex number greater than nv
                 */
                for(size_t i=0;i<ntri;i++)
                {
                    if (v[i].p1 >= nv || v[i].p2 >= nv || v[i].p3 >= nv)
                    {
                        v[i] = v[ntri-1];
                        --ntri;
                        --i;
                    }
                }
                
#if 0
                {
                    ios::ocstream fp("tri2d.dat",false);
                    for(size_t i=0;i<ntri;i++)
                    {
                        const ITRIANGLE &tr = v[i];
                        fp("%g %g %g\n", pxyz[tr.p1].x, pxyz[tr.p1].y, pxyz[tr.p1].z);
                        fp("%g %g %g\n", pxyz[tr.p2].x, pxyz[tr.p2].y, pxyz[tr.p2].z);
                        fp("%g %g %g\n", pxyz[tr.p3].x, pxyz[tr.p3].y, pxyz[tr.p3].z);
                        fp("%g %g %g\n", pxyz[tr.p1].x, pxyz[tr.p1].y, pxyz[tr.p1].z);
                        fp("\n");
                    }
                }
#endif
                
                return ntri;
            }
            
        }
        
        static inline
        int XYZCompare(const XYZ &p1, const XYZ &p2) throw()
        {
            const real_t x1 = p1.x;
            const real_t x2 = p2.x;
            return x1 < x2 ? -1 : ( x2 < x1 ? 1 : 0 );
        }
        
        
        static inline
        void __make_delaunay( sequence<iTriangle> &trlist, auto_arr<XYZ> &pxyz, auto_arr<size_t> &indx )
        {
            assert(0 == trlist.size() );
            const size_t nv = indx.size;
            assert(nv+3==pxyz.size);
            
            //------------------------------------------------------------------
            //-- preparing /allocating data for Triangulate
            //------------------------------------------------------------------
            co_hsort(pxyz.base(),indx.base(),nv,XYZCompare);
            auto_arr<ITRIANGLE> vtri(3*nv);
            
            //------------------------------------------------------------------
            //-- call the algorithm
            //------------------------------------------------------------------
            const size_t ntri = Triangulate( nv, pxyz.base(), vtri.base() );
            
            //------------------------------------------------------------------
            //-- build user's triangles list
            //------------------------------------------------------------------
            for(size_t i=0; i<ntri; ++i)
            {
                ITRIANGLE &src = vtri[i];
                (void)src;
                const iTriangle tgt( indx[src.p1], indx[src.p2], indx[src.p3] );
                trlist.push_back(tgt);
            }
            
        }
        
        template <>
        void delaunay<real_t>:: build( sequence<iTriangle> &trlist, const array<vtx2d> &vertices )
        {
            //------------------------------------------------------------------
            // preparing data for triangulation
            //------------------------------------------------------------------
            trlist.free();
            const size_t nv = vertices.size();
            if(nv<3) return;
            auto_arr<XYZ>       pxyz(nv+3);
            auto_arr<size_t>    indx(nv);
            
            assert(pxyz.size==nv+3);
            for( size_t i=1,j=0;i<=nv;++i,++j)
            {
                indx[j]        = i;
                XYZ         &p = pxyz[j];
                const vtx2d &v = vertices[i];
                p.x = v.x;
                p.y = v.y;
                p.z = 0;
            }
            
            //------------------------------------------------------------------
            // processing
            //------------------------------------------------------------------
            __make_delaunay(trlist, pxyz, indx);
        }
        
        template <>
        void delaunay<real_t>:: build( sequence<iTriangle> &trlist, const array<vtx3d> &vertices )
        {
            //------------------------------------------------------------------
            // preparing data for triangulation
            //------------------------------------------------------------------
            trlist.free();
            const size_t nv = vertices.size();
            if(nv<3) return;
            auto_arr<XYZ>       pxyz(nv+3);
            auto_arr<size_t>    indx(nv);
            
            assert(pxyz.size==nv+3);
            for( size_t i=1,j=0;i<=nv;++i,++j)
            {
                indx[j]        = i;
                XYZ         &p = pxyz[j];
                const vtx3d &v = vertices[i];
                p.x = v.x;
                p.y = v.y;
                p.z = v.z;
            }
            
            //------------------------------------------------------------------
            // processing
            //------------------------------------------------------------------
            __make_delaunay(trlist, pxyz, indx);
        }
        
        template <>
        real_t delaunay<real_t>::area( const array<size_t> &h, const array<vtx2d> &v)
        {
            real_t A = 0;
            const size_t n=h.size();
            if(n>2)
            {
                for(size_t i=n;i>0;--i)
                {
                    const vtx2d &p = v[h[i]];
                    const vtx2d &q = v[h[i<n ? i+1:1]];
                    A += p.x * q.y - p.y * q.x;
                }
                
            }
            return Fabs( REAL(0.5) * A);
        }
        
        template <>
        real_t delaunay<real_t>::area( const array<size_t> &h, const array<vtx3d> &v)
        {
            real_t A = 0;
            const size_t n=h.size();
            if(n>2)
            {
                for(size_t i=n;i>0;--i)
                {
                    const vtx3d &p = v[h[i]];
                    const vtx3d &q = v[h[i<n ? i+1:1]];
                    A += p.x * q.y - q.x * p.y;
                }
            }
            return Fabs( REAL(0.5) * A);
        }
        
        template <>
        void delaunay<real_t>:: save_vtk(const string &filename,
                                         const string &title,
                                         const array<iTriangle> &tr,
                                         const array<vtx2d> &vtx)
        {
            ios::ocstream fp(filename,false);
            
            //-- prolog
            fp("# vtk DataFile Version 3.0\n");
			fp("%s\n", title.c_str());
			fp("ASCII\n");
            
            //-- triangle
            const size_t nt = tr.size();
            fp("DATASET POLYDATA\n");
            fp("POINTS %u float\n", unsigned(nt) * 3 );
            for(size_t i=1; i <= nt; ++i )
            {
                const iTriangle &t = tr[i];
                const vtx2d &v1 = vtx[ t.p1 ];
                const vtx2d &v2 = vtx[ t.p2 ];
                const vtx2d &v3 = vtx[ t.p3 ];
                fp("%g %g 0.0\n", v1.x, v1.y);
                fp("%g %g 0.0\n", v2.x, v2.y);
                fp("%g %g 0.0\n", v3.x, v3.y);
            }
            fp("POLYGONS %u %u\n", unsigned(nt), unsigned(4 * nt) );
            unsigned count = 0;
            for(size_t i=1; i <= nt; ++i, count += 3)
            {
                fp("3 %u %u %u\n", count, count+1, count+2 );
            }
            
        }
        
        template <>
        void delaunay<real_t>:: save_vtk(const string &filename,
                                         const string &title,
                                         const array<iTriangle> &tr,
                                         const array<vtx3d> &vtx)
        {
            ios::ocstream fp(filename,false);
            
            //-- prolog
            fp("# vtk DataFile Version 3.0\n");
			fp("%s\n", title.c_str());
			fp("ASCII\n");
            
            //-- triangle
            const size_t nt = tr.size();
            fp("DATASET POLYDATA\n");
            fp("POINTS %u float\n", unsigned(nt) * 3 );
            for(size_t i=1; i <= nt; ++i )
            {
                const iTriangle &t = tr[i];
                const vtx3d &v1 = vtx[ t.p1 ];
                const vtx3d &v2 = vtx[ t.p2 ];
                const vtx3d &v3 = vtx[ t.p3 ];
                fp("%g %g %g\n", v1.x, v1.y, v1.z);
                fp("%g %g %g\n", v2.x, v2.y, v2.z);
                fp("%g %g %g\n", v3.x, v3.y, v3.z);
            }
            
            fp("POLYGONS %u %u\n", unsigned(nt), unsigned(4 * nt) );
            unsigned count = 0;
            for(size_t i=1; i <= nt; ++i, count += 3)
            {
                fp("3 %u %u %u\n", count, count+1, count+2 );
            }
            
        }
        
        
        
    }
}
