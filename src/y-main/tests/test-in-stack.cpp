#include "yocto/utest/run.hpp"
#include "yocto/code/in-stack.hpp"
#include "yocto/string/vfs-utils.hpp"
#include <cstdio>

using namespace yocto;
#define REQUIRE( A )    do{ if (!(A)) printf( "Assertion failed at %s(%i): %s \n", _vfs::get_base_name(__FILE__), __LINE__ , #A);} while(false)

//------------------------------------------------------------------------

class MyClass
{
};

//------------------------------------------------------------------------

static void func()
{
    char testArray[123] = {0};
    int size = __stack::used();
    printf( "usedStackSize in %s(%i) is %i\n", _vfs::get_base_name(__FILE__), __LINE__, size );
    
    REQUIRE( __stack::owns(&size) );
    REQUIRE( __stack::owns(testArray) );
}

static void test()
{
    char testArray[20] = {0};
    MyClass  a;
    MyClass* b = new MyClass;
    MyClass* c = new MyClass;
    MyClass  d;
    
    func();
    REQUIRE(  __stack::owns(testArray) );
    REQUIRE(  __stack::owns(&a) );
    REQUIRE( !__stack::owns(b)  );
    REQUIRE( !__stack::owns(c)  );
    REQUIRE(  __stack::owns(&d) );
    
    delete b;
    delete c;
}


YOCTO_UNIT_TEST_IMPL(in_stack)
{
    __stack::init();
    test();
}
YOCTO_UNIT_TEST_DONE()
