#ifndef YOCTO_CHEMICAL_LIBRARY_INCLUDED
#define YOCTO_CHEMICAL_LIBRARY_INCLUDED 1

#include "yocto/ptr/intr.hpp"
#include "yocto/counted-object.hpp"
#include "yocto/container/vslot.hpp"
#include "yocto/string.hpp"
#include "yocto/lang/parser.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        typedef lang::syntax::xnode xnode;
        
        //! a group=(residue|label) with label/count
        class group : public counted_object
        {
        public:
            typedef intr_ptr<string,group>     pointer;
            typedef set<string,group::pointer> database;
            
            const string  label; //!< atom or residue
            int           count; //!< count
            const string &key() const throw() { return label; };
            
            group(const string &grpLabel,
                  const size_t  grpCount) :
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
        
        //! managing groups algebra
        class groups : public group::database
        {
        public:
            virtual ~groups() throw() {}
            groups() : group::database()
            {
            }
            
            void add( const string &grpLabel, const int grpCount)
            {
                if(grpCount!=0)
                {
                    group::pointer *ppG = search(grpLabel);
                    if(ppG)
                    {
                        int &count = (**ppG).count;
                        count += grpCount;
                    }
                    else
                    {
                        const group::pointer grp( new group(grpLabel,grpCount) );
                        if(!insert(grp))
                        {
                            throw exception("add new group '%s' unexpected failure!",grpLabel.c_str());
                        }
                    }
                }
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(groups);
        };
        
        class species : public counted_object
        {
        public:
            explicit species();
            virtual ~species() throw();
            
            const string name;
            const int    z;
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(species);
            group::database groups;
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

