#ifndef CHILD_NODE_STUB_H_INCLUDED
#define CHILD_NODE_STUB_H_INCLUDED

#include "child-node.h"

template <typename charT>
class child_node_stub : public xml::basic_child_node<charT>
{
public:
    typedef          xml::basic_node_interface<charT> node_interface_t;
    typedef typename node_interface_t::type_t         type_t;

    typedef          xml::basic_child_node<charT>     child_node_t;
    typedef typename child_node_t::parent_t           parent_t;
    typedef typename child_node_t::child_pointer_t    child_pointer_t;
    typedef typename child_node_t::parent_pointer_t   parent_pointer_t;
    typedef typename child_node_t::parent_reference_t parent_reference_t;

    child_node_stub(parent_pointer_t parent = nullptr)
    :
        child_node_t(parent)
    {}

    virtual type_t type () const
    {
        return node_interface_t::stringToType("child-node-stub");
    }

    parent_pointer_t parent()
    {
        return child_node_t::mParent;
    }

    child_pointer_t previous()
    {
        return child_node_t::mPrevious;
    }

    child_pointer_t next()
    {
        return child_node_t::mNext;
    }
};


#endif /* CHILD_NODE_STUB_H_INCLUDED */