#include "yocto/utest/run.hpp"
#include "yocto/json/json.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(value)
{
    
    JSON::Value v( JSON::IsObject );
    v["x"] = 2.0;
    v["y"] = "world";
    
    std::cerr << v.type_name() << std::endl;
    std::cerr << "length=" << v.length() << std::endl;
    
    for( JSON::Object::iterator i = v.as<JSON::Object>().begin(); i != v.as<JSON::Object>().end(); ++i )
    {
        std::cerr << (*i).name << std::endl;
        
    }
}
YOCTO_UNIT_TEST_DONE()
