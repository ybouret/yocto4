#include "yocto/pack/huffman.hpp"
#include <iostream>
#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace pack
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // Item
        //
        ////////////////////////////////////////////////////////////////////////
        
        Huffman::Item:: Item(CharType Ch) throw() :
        Char(Ch),
        Freq(0),
        Code(0),
        Bits(0)
        {
            
        }
        
        Huffman::Item:: ~Item() throw() {}
        
        Huffman::Item:: Item( const Item &other ) throw() :
        Char( other.Char ),
        Freq( other.Freq ),
        Code( other.Code ),
        Bits( other.Bits )
        {
            
        }
        
        
        void Huffman:: Item:: emit(ios::bitio &bio) const
        {
            assert(Bits>0);
            bio.push<CodeType>(Code, Bits);
        }
        
        std::ostream & operator<<( std::ostream &os, const Huffman::Item &it )
        {
            if(it.Char>=32 && it.Char<127)
            {
                os << "'" << char(it.Char) << "' " ;
            }
            else
            {
                char buff[8];
                snprintf(buff, 7, "x%03x", int(it.Char) );
                os << buff;
            }
            os << ": #=" << it.Freq << " | bits=" << it.Bits;
            // code
            os << " | [";
            for( size_t i=it.Bits;i>0;--i)
            {
                const Huffman::CodeType b = 1 << (i-1);
                if(b&it.Code)
                    os << "1";
                else
                    os << "0";
            }
            os <<  "]";
            return os;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Alphabet
        //
        ////////////////////////////////////////////////////////////////////////
        Huffman:: Alphabet:: Alphabet() :
        numch(0),
        count( AlphabetSize ),
        items( memory::kind<Memory>::acquire_as<Item>(count) )
        {
            reset();
        }
        
        Huffman:: Alphabet:: ~Alphabet() throw()
        {
            memory::kind<Memory>::release_as<Item>(items,count);
        }
        
        size_t Huffman:: Alphabet::  chars() const throw()
        {
            return numch;
        }
        
        void Huffman:: Alphabet:: reset() throw()
        {
            numch = 0;
            for(CharType i=0;i<256; ++i)
            {
                Item *it = new ( &items[i] ) Item(i);
                it->Code = CodeType(i);
                it->Bits = 8;
                assert(0==it->Freq);
            }
            
            for(CharType i=256;i<CharType(AlphabetSize);++i)
            {
                Item *it = new ( &items[i] ) Item(i);
                it->Freq = 1;
                assert(0==it->Code);
                assert(0==it->Bits);
            }
        }
        
        
        const Huffman:: Item & Huffman:: Alphabet:: operator[](CharType Ch) const  throw()
        {
            assert(Ch>=0);
            assert(Ch<AlphabetSize);
            return items[Ch];
        }
        
        void Huffman:: Alphabet:: increase(uint8_t b) throw()
        {
            Item &item = items[b];
            if(item.Freq++<=0)
                ++numch;
            //std::cerr << "+ " << int(b) << std::endl;
        }
        
        void Huffman:: Alphabet:: rescale() throw()
        {
            for(CharType i=0;i<256; ++i)
            {
                FreqType &freq = items[i].Freq;
                if(freq>0)
                {
                    freq >>= 1;
                    freq  &= 1;
                }
            }
        }
        
        void Huffman:: Alphabet:: check_nyt() throw()
        {
            if(numch>=256) items[NYT].Freq = 0;
        }
        
        std::ostream & operator<<(std::ostream &os, const Huffman::Alphabet &alpha)
        {
            for(size_t i=0;i<Huffman::AlphabetSize;++i)
            {
                const Huffman::Item &item = alpha.items[i];
                if(item.Freq>0)
                {
                    os << item << std::endl;
                }
            }
            os << "#used=" << alpha.chars();
            return os;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Node
        //
        ////////////////////////////////////////////////////////////////////////
        Huffman::Node:: Node( CharType Ch, FreqType Fr ) throw() :
        parent(0),
        left(0),
        right(0),
        Char(Ch),
        Freq(Fr),
        cbit(0)
        {
            
        }
        
        Huffman::Node:: ~Node() throw() {}
        
        
        int  Huffman:: Node::Comparator:: operator()(const Node &lhs,const Node &rhs) throw()
        {
            const FreqType L = lhs.Freq;
            const FreqType R = rhs.Freq;
            return L < R ? 1 : (R<L?-1 : 0) ;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Tree
        //
        ////////////////////////////////////////////////////////////////////////
        Huffman:: Tree:: Tree() :
        root(0),
        h(NodesMax,as_capacity),
        count(NodesMax),
        nodes( memory::kind<Memory>::acquire_as<Node>(count))
        {
        }
        
        Huffman:: Tree:: ~Tree() throw()
        {
            memory::kind<Memory>::release_as<Node>(nodes,count);
        }
        
        const Huffman:: Node * Huffman:: Tree:: get_root() const throw()
        {
            return root;
        }
        
        bool Huffman::Tree:: try_build_for(Alphabet &alpha) throw()
        {
            size_t inode = 0;
            h.free();
            
            //__________________________________________________________________
            //
            // Fill in the tree
            //__________________________________________________________________
            for(CharType i=0;i<CharType(AlphabetSize);++i)
            {
                const Item &item = alpha[i];
                assert(inode<NodesMax);
                if(item.Freq>0)
                {
                    Node *node = new ( & nodes[inode++] ) Node(item.Char,item.Freq);
                    h.__push(node);
                }
            }
            const size_t nch = inode;
            assert(inode>0);
            assert(h.size()==nch);
            
            //__________________________________________________________________
            //
            // Build the tree
            //__________________________________________________________________
            while(h.size()>1)
            {
                Node          *left  = h.pop(); // smallest frequency
                Node          *right = h.pop(); // second smallest frequency
                const FreqType Fr    = left->Freq + right->Freq; // TODO: Check
                assert(inode<NodesMax);
                Node *node      = new ( & nodes[inode++] ) Node(INS,Fr);
                node->left      = left;
                node->right     = right;
                left->parent    = node;
                right->parent   = node;
                right->cbit     = 1;
                h.__push(node);
            }
            
            //__________________________________________________________________
            //
            // Get the root
            //__________________________________________________________________
            root = h.pop();
            
            if(false)
            {
                ios::ocstream fp("huff.dot",false);
                fp << "digraph G {\n";
                for(size_t i=0;i<inode;++i)
                {
                    const Node *node = &nodes[i];
                    fp("\tnode%u [label=\"", unsigned(i));
                    const CharType C = node->Char;
                    if(C>=0)
                    {
                        if(C>=32&&C<127&&C!='\'' && C != '\"')
                        {
                            fp("%c", char(C));
                        }
                        else
                            fp("x%02x", int(C) );
                    }
                    else
                    {
                        fp("INS");
                    }
                    fp("\"];\n");
                }
                for(size_t i=0;i<inode;++i)
                {
                    const Node *node = &nodes[i];
                    if(node->left)
                    {
                        const ptrdiff_t j = node->left - nodes;
                        fp("\tnode%u -> node%u [label=\"0\"];\n", unsigned(i), unsigned(j) );
                    }
                    if(node->right)
                    {
                        const ptrdiff_t j = node->right - nodes;
                        fp("\tnode%u -> node%u [label=\"1\"];\n", unsigned(i), unsigned(j) );
                    }
                }
                
                fp << "}\n";
            }
            
            
            //__________________________________________________________________
            //
            // Build Codes
            //__________________________________________________________________
            for(size_t i=0;i<nch;++i)
            {
                Node     *node = &nodes[i]; assert(node->Char>=0); assert(node->Char<CharType(AlphabetSize));
                Item     &item = (Item &) alpha[node->Char];
                CodeType &code = item.Code;
                BitsType &bits = item.Bits;
                code = 0;
                bits = 0;
                Node *parent = node->parent;
                while(parent)
                {
                    /*
                     if(node==parent->right)
                     code |= (1<<bits);
                     */
                    code |= (node->cbit<<bits);
                    ++bits;
                    if(bits>RootBits)
                        return false;
                    node = parent;
                    parent = node->parent;
                }
            }
            
            return true;
        }
        
        void Huffman:: Tree:: build_for( Alphabet &alpha ) throw()
        {
            alpha.check_nyt();
            while( !try_build_for(alpha) )
            {
                std::cerr << "RESCALING" << std::endl;
                alpha.rescale();
            }
        }
        
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // base class Codec
        //
        ////////////////////////////////////////////////////////////////////////
        Huffman::Codec:: Codec() :
        alpha(),
        tree(),
        bio()
        {
            tree.build_for(alpha);
        }
        
        Huffman:: Codec:: ~Codec() throw()
        {
            
        }
        
        const Huffman:: Alphabet & Huffman:: Codec:: alphabet() const throw()
        {
            return alpha;
        }
        
        void Huffman::Codec:: cleanup() throw()
        {
            Q.free();
            bio.free();
            alpha.reset();
            tree.build_for(alpha);
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // encoder
        //
        ////////////////////////////////////////////////////////////////////////
        Huffman::encoder:: encoder() :
        Codec()
        {
        }
        
        Huffman:: encoder:: ~encoder() throw()
        {
        }
        
        void Huffman::encoder::reset() throw()
        {
            cleanup();
        }
        
        void Huffman::encoder:: write(char C)
        {
            CharType    Ch   = uint8_t(C);
            const Item &item = alpha[Ch];
            if(item.Freq<=0 && alpha.chars()>0 )
            {
                alpha[NYT].emit(bio);
            }
            item.emit(bio);
            while(bio.size()>=8)
            {
                Q.push_back( bio.pop_full<uint8_t>() );
            }
            alpha.increase(C);
            tree.build_for(alpha);
        }
        
        void Huffman::encoder::flush()
        {
            std::cerr << "Emitting END!" << std::endl;
            alpha[END].emit(bio);
            bio.fill_to_byte_with(false);
            while(bio.size()>=8)
            {
                Q.push_back( bio.pop_full<uint8_t>() );
            }
            assert(0==bio.size());
        }
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // decoder
        //
        ////////////////////////////////////////////////////////////////////////
        Huffman:: decoder:: decoder() :
        Codec(),
        flag(wait_for8),
        node(0)
        {
        }
        
        Huffman:: decoder:: ~decoder() throw()
        {
        }
        
        void Huffman:: decoder:: reset() throw()
        {
            cleanup();
            flag = wait_for8;;
            node = 0;
        }
        
        void Huffman:: decoder:: emit(uint8_t b)
        {
            Q.push_back(b);
            alpha.increase(b);
            tree.build_for(alpha);
            node = tree.get_root();
            flag = wait_for1;
        }
        
        void Huffman:: decoder:: write(char C)
        {
            bio.push_full<uint8_t>(C);
            std::cerr << "Decoding with [" << int(uint8_t(C)) << "]" << std::endl;
            while(true)
            {
                switch(flag)
                {
                    case wait_for8:
                        std::cerr << "Wait8" << std::endl;
                        if( bio.size() >= 8 )
                        {
                            const uint8_t b = bio.pop_full<uint8_t>();
                            std::cerr << "Emitting " << int(b) << std::endl;
                            emit(b);
                        }
                        else
                        {
                            std::cerr << "No8" << std::endl;
                            return;
                        }
                        break;
                        
                    case wait_for1:
                        assert(node!=0);
                        assert(node->Char==INS);
                        assert(node->left!=0);
                        assert(node->right!=0);
                        if(bio.size()>0)
                        {
                            const bool bit = bio.pop();
                            node = bit ? node->right : node->left;
                            if(node->Char>=0)
                            {
                                switch(node->Char)
                                {
                                    case NYT:
                                        node = 0;
                                        flag = wait_for8;
                                        break;
                                        
                                    case END:
                                        //TODO
                                        std::cerr << std::endl << "TODO: END" << std::endl;
                                        break;
                                        
                                    default:
                                        emit(node->Char);
                                }
                            }
                        }
                        else
                        {
                            return;
                        }
                        break;
                }
            }
        }
        
        void Huffman:: decoder:: flush()
        {
            // nothing to do ?
        }
        
    }
}