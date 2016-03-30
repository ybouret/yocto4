#include "yocto/lang/syntax/walker.hpp"
#include "walker.hxx"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace lang;
int main()
{
	the_walker_base a_walker;
	a_walker.graphviz("walker.dot");
	ios::graphviz_render("walker.dot");
	return 0;
}
