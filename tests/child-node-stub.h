#ifndef CHILD_NODE_STUB_H_INCLUDED
#define CHILD_NODE_STUB_H_INCLUDED

#include "child-node.h"

template <typename charT>
class child_node_stub : public xml::basic_child_node<charT>
{
public:
    typedef          xml::basic_node_interface<charT> node_interface_t;
    typedef typename node_interface_t::type_t         type_t;

    typedef          xml::basic_child_node<charT> child_t;
    typedef typename child_t::child_pointer_t     child_pointer_t;
    typedef typename child_t::child_move_t        child_move_t;

    typedef typename child_t::parent_t            parent_t;
    typedef typename child_t::parent_pointer_t    parent_pointer_t;
    typedef typename child_t::parent_reference_t  parent_reference_t;

    child_node_stub(parent_pointer_t parent = nullptr)
    :
        child_t(parent),
        mId(sIdCreator++),
        mCopyConstructed(false),
        mNumCopy(0),
        mMoveConstructed(false),
        mHasBeenMoved(false)
    {
        ++sObjectNumber;
    }

    child_node_stub(const child_node_stub<charT>& rhs)
    :
        child_t(rhs.mParent),
        mId(rhs.mId),
        mCopyConstructed(true),
        mNumCopy(0),
        mMoveConstructed(false),
        mHasBeenMoved(false)
    {
        rhs.mNumCopy++;
        ++sObjectNumber;
    }

    child_node_stub(child_node_stub<charT>&& rhs)
    :
        child_t(rhs),
        mId(rhs.mId),
        mCopyConstructed(false),
        mNumCopy(0),
        mMoveConstructed(true),
        mHasBeenMoved(false)
    {
        rhs.mHasBeenMoved = true;
    }

    virtual ~child_node_stub()
    {
        if (!mHasBeenMoved)
            --sObjectNumber;
    }

    virtual type_t type () const
    {
        return node_interface_t::stringToType("child-node-stub");
    }

    virtual child_pointer_t clone() const
    {
        return new child_node_stub<charT>(*this);
    }

    virtual child_pointer_t clone(child_move_t rhs) const
    {
        return new child_node_stub<charT>(static_cast<child_node_stub<charT>&&>(rhs));
    }

    parent_pointer_t parent()
    {
        return child_t::mParent;
    }

    child_pointer_t previous()
    {
        return child_t::mPrevious;
    }

    child_pointer_t next()
    {
        return child_t::mNext;
    }

    int id() const
    {
        return mId;
    }

    bool copyConstructed() const
    {
        return mCopyConstructed;
    }

    int numCopy() const
    {
        return mNumCopy;
    }

    bool moveConstructed() const
    {
        return mMoveConstructed;
    }

    bool hasBeenMoved() const
    {
        return mHasBeenMoved;
    }

    static int objectNumber ()
    {
        return sObjectNumber;
    }

private:
    const int mId;

    mutable bool mCopyConstructed; // Whether this element is constructed from a copy
    mutable int  mNumCopy;         // How many times this element has been copied

    mutable bool mMoveConstructed; // Whether this element is constructed from a move
    mutable bool mHasBeenMoved;    // Whether this element has been moved

    static int sIdCreator;
    static int sObjectNumber;
};

template <typename charT>
int child_node_stub<charT>::sIdCreator = 0;

template <typename charT>
int child_node_stub<charT>::sObjectNumber = 0;


#endif /* CHILD_NODE_STUB_H_INCLUDED */