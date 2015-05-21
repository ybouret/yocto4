#include "yocto/lang/pattern/posix.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"
#include <cstdlib>

using namespace yocto;
using namespace lang;

static inline
void test_posix_for(const char *name, pattern *p )
{
    auto_ptr<pattern> q(p);
    string bname = name;
    const string vizname = bname + ".dot";
    const string pngname = bname + ".png";
    q->graphviz(vizname);
    const string cmd     = "dot -Tpng " + vizname + " -o " + pngname;
    (void)system( cmd.c_str() );
    std::cerr << name << " accepts emtpy = " << ( q->accept_empty() ? "TRUE" : "FALSE") << std::endl;
}


#define DO_TEST(NAME) test_posix_for( #NAME, posix::NAME() )

YOCTO_UNIT_TEST_IMPL(posix)
{
    DO_TEST(lower);
    DO_TEST(upper);
    DO_TEST(digit);
    DO_TEST(alpha);
    DO_TEST(alnum);
    DO_TEST(xdigit);
    DO_TEST(blank);
    DO_TEST(space);
    DO_TEST(punct);

    DO_TEST(word);
    DO_TEST(endl);
    DO_TEST(dot);
    DO_TEST(cstring);

}
YOCTO_UNIT_TEST_DONE()

