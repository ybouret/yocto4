#include "yocto/string.hpp"
#include "yocto/gfx/named-rgb.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/exception.hpp"

#include "rgb.h"

using namespace yocto;

#include <iostream>

static const array<string> *pnames = 0;

void OnClick()
{
    assert(pnames);
    const int i = browser->value();
    assert(i>0);
    assert(i<=int(pnames->size()));
    const string     name = (*pnames)[i];
    const gfx::rgb_t c = gfx::get_named_rgb( (*pnames)[i] );
    Fl_Color         C = fl_rgb_color(c.r, c.g, c.b);
    box->color(C);
    box->redraw();
}

int main( int argc, char *argv[] )
{
    
    try {
        
        MakeWindow()->show(argc,argv);
        vector<string> names;
        const size_t len = gfx::get_named_rgb(names);
        if(names.size()<=0)
            throw exception("No Colors..");
        for(size_t i=1;i<=names.size();++i)
        {
            string s = names[i] + " ";
            for(size_t j=len-names[i].size();j>0;--j) s += ' ';
            s += '#';
            browser->add( s.c_str() );
        }
        browser->select(1);
        pnames = &names;
        
        OnClick();
        return Fl::run();
    }
    catch(const exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...) {
        std::cerr << "Unhandled Exception!" << std::endl;
    }
    return -1;
}
