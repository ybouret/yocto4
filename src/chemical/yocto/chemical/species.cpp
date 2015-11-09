#include "yocto/chemical/species.hpp"
#include "yocto/exception.hpp"
#include "yocto/hashing/perfect.hpp"

#include <iostream>

namespace yocto
{


    namespace  chemical
    {


        species:: ~species() throw() {}




        namespace
        {

            static const char *core_words[] =
            {
                "link",
                "residue"
            };

            static const char *extra_words[] =
            {
                "minus",
                "plus",
                "tag"
            };

            class builder
            {
            public:
                const hashing::perfect coreH;
                const hashing::perfect extraH;

                inline builder() :
                coreH( YOCTO_PERFECT_HASHER_FOR(core_words) ),
                extraH(YOCTO_PERFECT_HASHER_FOR(extra_words))
                {
                }

                inline virtual ~builder() throw()
                {
                }


            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(builder);
            };

            class nameBuilder : public builder
            {
            public:
                string name;

                inline nameBuilder(const xnode *molecule)
                {
                    assert(molecule);
                    assert("molecule"==molecule->label);
                    assert(1<=molecule->size());
                    assert(molecule->size()<=3);
                    const xnode *link = molecule->head();
                    onLink(link);

                    for(const xnode *extra=link->next;extra;extra=extra->next)
                    {
                        const string str = extra->content();
                        switch(extraH(extra->label))
                        {
                            case 0: // minus
                                assert("minus"==extra->label);
                                std::cerr << "minus:" << str << std::endl;
                                break;

                            case 1: // plus
                                assert("plus"==extra->label);
                                std::cerr << "plus :" << str << std::endl;
                                break;

                            case 2: // tag
                                assert("tag"==extra->label);
                                std::cerr << "tag  :" << str << std::endl;
                                break;

                            default:
                                throw exception("no possible extra '%s'", extra->label.c_str());
                        }
                        name += str;
                    }
                }

                inline virtual ~nameBuilder() throw()
                {
                }

                inline void onLink( const xnode *link )
                {
                    assert(link);
                    assert("link"==link->label);
                    for(const xnode *stoi = link->head(); stoi; stoi=stoi->next)
                    {
                        onStoi(stoi);
                    }
                }

                inline void onStoi( const xnode *stoi )
                {
                    assert(stoi);
                    assert("stoi"==stoi->label);
                    assert(1==stoi->size()||2==stoi->size());
                    const xnode *core = stoi->head();
                    const xnode *nn   = core->next; // optional
                    string       ns   = "1";
                    if(nn) { ns = nn->content(); }
                    const int    n    = atoi( ns.c_str() );

                    //std::cerr << "got core=" << core->label << std::endl;
                    switch(coreH(core->label))
                    {
                        case 0:
                            //__________________________________________________
                            //
                            // link
                            //__________________________________________________
                            name += '(';
                            onLink(core);
                            name += ')';
                            break;


                        case 1:
                            //__________________________________________________
                            //
                            // residue
                            //__________________________________________________
                            assert("residue"==core->label);
                        {
                            const string residue = core->content();
                            name += residue;
                        }
                            break;

                        default:
                            //__________________________________________________
                            //
                            // a registered atom
                            //__________________________________________________
                        {
                            const string atom = core->label;
                            name += atom;
                        }
                    }
                    
                    if(n>1)
                    {
                        name += ns;
                    }
                    
                    
                    
                    
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(nameBuilder);
            };
        }
        
        string species:: name_of( const xnode *molecule )
        {
            nameBuilder nb(molecule);
            return nb.name;
        }
        
    }
}