#ifndef PARENT_NODE_STUB_H_INCLUDED
#define PARENT_NODE_STUB_H_INCLUDED

#include "parent-node.h"

template <typename charT>
class parent_node_stub : public xml::basic_parent_node<charT>
{
    public:
        typedef          xml::basic_node_interface<charT> node_interface_t;
        typedef typename node_interface_t::type_t         type_t;

        typedef          xml::basic_parent_node<charT>  parent_node_t;
        typedef typename parent_node_t::child_pointer_t child_pointer_t;

        parent_node_stub()
        :
            parent_node_t()
        {}

        virtual type_t type () const
        {
            return node_interface_t::stringToType("parent-node-stub");
        }

        child_pointer_t first ()
        {
            return parent_node_t::mFirst;
        }

        child_pointer_t last ()
        {
            return parent_node_t::mLast;
        }
};


#endif /* PARENT_NODE_STUB_H_INCLUDED */