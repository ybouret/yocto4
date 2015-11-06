#include "yocto/chemical/library.hpp"
#include "yocto/lang/generator.hpp"

#include "yocto/ios/graphviz.hpp"

namespace yocto
{
    namespace chemical
    {
        library::~library() throw() {}


static const char species_grammar[] =
        {
#include "species.inc"
        };

#define Y_CHEMLIB_GEN(IDENT) \
lang::parser::generate( #IDENT, IDENT##_grammar, sizeof(IDENT##_grammar)/sizeof(IDENT##_grammar[0]))

        library:: library() :
        max_name_length(0),
        species_parser( Y_CHEMLIB_GEN(species) )
        {
            species_parser->gramviz("species.dot");
            ios::graphviz_render("species.dot");
        }


        xnode *library:: parse_species(ios::istream &fp)
        {
            //species_parser->restart();
            return species_parser->run(fp);
        }
    }
}