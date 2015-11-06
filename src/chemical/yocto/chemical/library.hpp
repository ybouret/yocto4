#ifndef YOCTO_CHEMICAL_LIBRARY_INCLUDED
#define YOCTO_CHEMICAL_LIBRARY_INCLUDED 1

#include "yocto/chemical/species.hpp"
#include "yocto/lang/parser.hpp"

namespace yocto
{
    namespace chemical
    {

        typedef lang::syntax::xnode xnode;


        class group : public counted_object
        {
        public:
            typedef arc_ptr<group> pointer;
            typedef set<string,group::pointer> database;
            
            const string label; //!< atom or residue
            size_t       count; //!< count
            const string key() const throw() { return label; };

            group(const string &grpLabel, const size_t grpCount) :
            label(grpLabel),
            count(grpCount)
            {
            }

            virtual ~group() throw()
            {
            }

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(group);
        };


        class library : public object
        {
        public:
            explicit library();
            virtual ~library() throw();

            const size_t max_name_length;

            xnode *parse_species(ios::istream &fp);

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
            auto_ptr<lang::parser> species_parser;
        };
    }
}

#endif

