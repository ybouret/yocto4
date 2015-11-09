#include "yocto/lang/pattern/posix.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"
#include <cstdlib>
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace lang;

static inline
void test_posix_for(const char *name, pattern *p )
{
    auto_ptr<pattern> q(p);
    string bname = name;
    const string vizname = bname + ".dot";
    q->graphviz(vizname);
    ios::graphviz_render(vizname);
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

    DO_TEST(base64);
}
YOCTO_UNIT_TEST_DONE()

