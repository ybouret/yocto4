#include "yocto/utest/run.hpp"
#include "./main.hpp"

#include "yocto/spade/array-db.hpp"
#include "yocto/spade/array1d.hpp"
#include "yocto/spade/array2d.hpp"

YOCTO_UNIT_TEST_IMPL(arrays)
{
    array_db adb;
    {
        const layout1D L( RandCoord1D(), RandCoord1D());
        linear *info = 0;
        void   *addr = array1D<float>::ctor( L, &info );
        varray::ptr p( new varray("a1", typeid(array1D<float>), typeid(float), addr, info, array1D<float>::dtor));
        adb.insert(p);
    }
    
    {
        const layout2D L( RandCoord2D(), RandCoord2D());
        linear *info = 0;
        void   *addr = array2D<double>::ctor( L, &info );
        varray::ptr p( new varray("b2", typeid(array2D<double>), typeid(double), addr, info, array2D<double>::dtor));
        adb.insert(p);
    }
    
    array1D<float> &A = adb["a1"].as< array1D<float> >();
    A.ldz();
    
    array2D<double> &B = adb["b2"].as< array2D<double> >();
    B.ldz();
    
}
YOCTO_UNIT_TEST_DONE()
