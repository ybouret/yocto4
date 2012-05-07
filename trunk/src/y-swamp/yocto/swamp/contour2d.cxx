
namespace yocto 
{
    
    namespace swamp
    {
        template <>
        void contour2D<ZTYPE>:: compute(const array2D<ZTYPE> &d, const array1D<ZTYPE> &x, const array1D<ZTYPE> &y, const layout2D &sub, const level_set<ZTYPE> &levels, callback &proc)
        {
            assert( d.has( sub.lower ) );
			assert( d.has( sub.upper ) );
			assert( sub.lower.x >= x.lower );
			assert( sub.upper.x <= x.upper );
			assert( sub.lower.y >= y.lower );
			assert( sub.upper.y <= y.upper );
			assert( levels.size() > 0 );
			
			const unit_t jub=sub.upper.y;
			const unit_t jlb=sub.lower.y;
			const unit_t iub=sub.upper.x;
			const unit_t ilb=sub.lower.x;
			const size_t nc =levels.size();
			
			
#if !defined(NDEBUG)
			for( size_t iz=1; iz < nc; ++iz )
			{
				assert(levels[iz].value<levels[iz+1].value);
			}
#endif
			
#if !defined(xsect)
#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])
#endif
			int m1,m2,m3,case_value;
			ZTYPE x1=0,x2=0,y1=0,y2=0;
			ZTYPE h[5];
			int    sh[5];
			ZTYPE xh[5],yh[5];
			static const int im[4] = {0,1,1,0},jm[4]={0,0,1,1};
			static const int castab[3][3][3] = {
				{ {0,0,8},{0,2,5},{7,6,9} },
				{ {0,3,4},{1,3,1},{4,3,0} },
				{ {9,6,7},{5,2,0},{8,0,0} }
			};
			
			
			for (int j=(jub-1);j>=jlb;j--) 
			{
				for (int i=ilb;i<=iub-1;i++) 
				{
					const ZTYPE dmin  = min_of(min_of(d[j][i],d[j+1][i]),min_of(d[j][i+1],d[j+1][i+1]));
					const ZTYPE dmax  = max_of(max_of(d[j][i],d[j+1][i]),max_of(d[j][i+1],d[j+1][i+1]));
					
					if (dmax < levels[1].value || dmin > levels[nc].value )
						continue;
					
					for (size_t k=1;k<=nc;k++) 
					{
						if ( levels[k].value < dmin || levels[k].value > dmax)
							continue;
						for (int m=4;m>=0;m--)
						{
							if (m > 0) 
							{
								h[m]  = d[j+jm[m-1]][i+im[m-1]]-levels[k].value;
								xh[m] = x[i+im[m-1]];
								yh[m] = y[j+jm[m-1]];
							} 
							else
							{
								h[0]  = ZTYPE(0.25) * (h[1]+h[2]+h[3]+h[4]);
								xh[0] = ZTYPE(0.50) * (x[i]+x[i+1]);
								yh[0] = ZTYPE(0.50) * (y[j]+y[j+1]);
							}
							if (h[m] > 0.0)
								sh[m] = 1;
							else if (h[m] < 0.0)
								sh[m] = -1;
							else
								sh[m] = 0;
						}
						
						/*
						 Note: at this stage the relative heights of the corners and the
						 centre are in the h array, and the corresponding coordinates are
						 in the xh and yh arrays. The centre of the box is indexed by 0
						 and the 4 corners by 1 to 4 as shown below.
						 Each triangle is then indexed by the parameter m, and the 3
						 vertices of each triangle are indexed by parameters m1,m2,and m3.
						 It is assumed that the centre of the box is always vertex 2
						 though this isimportant only when all 3 vertices lie exactly on
						 the same contour level, in which case only the side of the box
						 is drawn.
						 vertex 4 +-------------------+ vertex 3
						 | \               / |
						 |   \    m-3    /   |
						 |     \       /     |
						 |       \   /       |
						 |  m=2    X   m=2   |       the centre is vertex 0
						 |       /   \       |
						 |     /       \     |
						 |   /    m=1    \   |
						 | /               \ |
						 vertex 1 +-------------------+ vertex 2
						 */
						/* Scan each triangle in the box */
						for (int m=1;m<=4;m++) 
						{
							m1 = m;
							m2 = 0;
							if (m != 4)
								m3 = m + 1;
							else
								m3 = 1;
							if ((case_value = castab[sh[m1]+1][sh[m2]+1][sh[m3]+1]) == 0)
								continue;
							switch (case_value) {
								case 1: /* Line between vertices 1 and 2 */
									x1 = xh[m1];
									y1 = yh[m1];
									x2 = xh[m2];
									y2 = yh[m2];
									break;
								case 2: /* Line between vertices 2 and 3 */
									x1 = xh[m2];
									y1 = yh[m2];
									x2 = xh[m3];
									y2 = yh[m3];
									break;
								case 3: /* Line between vertices 3 and 1 */
									x1 = xh[m3];
									y1 = yh[m3];
									x2 = xh[m1];
									y2 = yh[m1];
									break;
								case 4: /* Line between vertex 1 and side 2-3 */
									x1 = xh[m1];
									y1 = yh[m1];
									x2 = xsect(m2,m3);
									y2 = ysect(m2,m3);
									break;
								case 5: /* Line between vertex 2 and side 3-1 */
									x1 = xh[m2];
									y1 = yh[m2];
									x2 = xsect(m3,m1);
									y2 = ysect(m3,m1);
									break;
								case 6: /* Line between vertex 3 and side 1-2 */
									x1 = xh[m3];
									y1 = yh[m3];
									x2 = xsect(m1,m2);
									y2 = ysect(m1,m2);
									break;
								case 7: /* Line between sides 1-2 and 2-3 */
									x1 = xsect(m1,m2);
									y1 = ysect(m1,m2);
									x2 = xsect(m2,m3);
									y2 = ysect(m2,m3);
									break;
								case 8: /* Line between sides 2-3 and 3-1 */
									x1 = xsect(m2,m3);
									y1 = ysect(m2,m3);
									x2 = xsect(m3,m1);
									y2 = ysect(m3,m1);
									break;
								case 9: /* Line between sides 3-1 and 1-2 */
									x1 = xsect(m3,m1);
									y1 = ysect(m3,m1);
									x2 = xsect(m1,m2);
									y2 = ysect(m1,m2);
									break;
								default:
									break;
							}
							
							/* Finally process the line */
							const vertex v1(x1,y1);
							const vertex v2(x2,y2);
							proc(v1,v2,levels[k]);
							
						} /* m */
					} /* k - contour */
				} /* i */
			} /* j */

        }
        
    }

}
