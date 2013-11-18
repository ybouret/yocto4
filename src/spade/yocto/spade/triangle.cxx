
namespace yocto 
{
    namespace spade
    {
        template <>
        triangle3D<ZTYPE>:: ~triangle3D() throw() {}
        
        template <>
        triangle3D<ZTYPE>::triangle3D() throw() : p0(), p1(), p2() {}
        
        template <>
        triangle3D<ZTYPE>::triangle3D( const triangle3D &f ) throw() : p0(f.p0), p1(f.p1), p2(f.p2) {}
        
        template <>
        triangle3D<ZTYPE> & triangle3D<ZTYPE>::operator=( const triangle3D &f ) throw() 
        {
            p0 = f.p0; p1 = f.p1; p2 = f.p2;
            return *this;
        }
        
        
    }
}
