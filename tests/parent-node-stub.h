#ifndef PARENT_NODE_STUB_H_INCLUDED
#define PARENT_NODE_STUB_H_INCLUDED

#include "parent-node.h"

template <typename charT>
class parent_node_stub : public xml::basic_parent_node<charT>
{
public:
    typedef          xml::basic_node_interface<charT> node_interface_t;
    typedef typename node_interface_t::type_t         type_t;

    typedef          xml::basic_parent_node<charT> parent_t;
    typedef typename parent_t::child_t             child_t;

    template <class classT = child_t>
    using iterator = typename parent_t::template iterator<classT>;

    template <class classT = child_t>
    using const_iterator = typename parent_t::template const_iterator<classT>;

    template <class classT = child_t>
    using reverse_iterator = typename parent_t::template reverse_iterator<classT>;

    template <class classT = child_t>
    using const_reverse_iterator = typename parent_t::template const_reverse_iterator<classT>;

    parent_node_stub()
    :
        parent_t(),
        mId(sIdCreator++)
    {}

    virtual ~parent_node_stub() {}

    virtual type_t type () const
    {
        return node_interface_t::stringToType("parent-node-stub");
    }

    child_t*& first ()
    {
        return parent_t::mFirst;
    }

    child_t*& last ()
    {
        return parent_t::mLast;
    }

    template <class classT = child_t>
    iterator<classT> insert(iterator<classT> position, const child_t& val)
    {
        return parent_t::insert(position, val);
    }

    template <class classT = child_t>
    iterator<classT> insert(iterator<classT> position, size_t n, const child_t& val)
    {
        return parent_t::insert(position, n, val);
    }

    template <class InputIterator, class classT = child_t>
    iterator<classT> insert (iterator<classT> position, InputIterator first, InputIterator last)
    {
        return parent_t::insert(position, first, last);
    }

    template <class classT = child_t>
    iterator<classT> insert(iterator<classT> position, child_t&& val)
    {
        return parent_t::insert(position, std::move(val));
    }

    template <class classT = child_t, class classU>
    iterator<classT> insert (iterator<classT> position, std::initializer_list<classU> il)
    {
        return parent_t::insert(position, il);
    }

    template <class classT = child_t>
    iterator<classT> push_front(const child_t& val)
    {
        return parent_t::push_front(val);
    }

    template <class classT = child_t>
    iterator<classT> push_front(child_t&& val)
    {
        return parent_t::push_front(std::move(val));
    }

    template <class classT = child_t>
    iterator<classT> push_back(const child_t& val)
    {
        return parent_t::push_back(val);
    }

    template <class classT = child_t>
    iterator<classT> push_back(child_t&& val)
    {
        return parent_t::push_back(std::move(val));
    }

    template <class classU, typename ... Args, class classT = child_t>
    iterator<classT> emplace (iterator<classT> position, Args&& ... args)
    {
        return parent_t::template emplace<classU>(position, std::forward<Args>(args) ...);
    }

    template <class classU, typename ... Args, class classT = child_t>
    iterator<classT> emplace_front (Args&& ... args)
    {
        return parent_t::template emplace_front<classU>(std::forward<Args>(args) ...);
    }

    template <class classU, typename ... Args, class classT = child_t>
    iterator<classT> emplace_back (Args&& ... args)
    {
        return parent_t::template emplace_back<classU>(std::forward<Args>(args) ...);
    }

    int id() const { return mId; }

private:
    parent_node_stub(int id)
    :
        parent_t(),
        mId(id)
    {}

    const int mId;

    static int sIdCreator;
};

template <typename charT>
int parent_node_stub<charT>::sIdCreator = 0;

#endif /* PARENT_NODE_STUB_H_INCLUDED */