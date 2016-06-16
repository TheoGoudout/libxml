#ifndef basic_child_node_H_INCLUDED
#define basic_child_node_H_INCLUDED

namespace xml {
    template <typename charT>
    class basic_child_node;
}

#include <node-interface.h>
#include <child-node.h>
#include <iterator.h>

namespace xml {
    //! \brief An abstract XML node that has a parent and siblings.
    /*!
     *  This class represents an abstract XML node that has a parent
     *  and possibly two siblings : one before and one after.
     *
     *  \sa xml::basic_parent_node
     *
     *  \tparam charT The type of character used in the XML node.
     *                By default, char and wchar_t are supported.
     */
    template <typename charT>
    class basic_child_node : public virtual basic_node_interface<charT> {
    public:
        //! \name Member types
        //!@{
        typedef basic_node_interface<charT> node_interface_t; //!< The base type of this node.

        typedef basic_child_node<charT>   child_t;                 //!< The type of children this node is.
        typedef child_t*                  child_pointer_t;         //!< Pointer to \c child_t.
        typedef child_t&                  child_reference_t;       //!< Reference to \c child_t.
        typedef const child_t&            child_const_reference_t; //!< Constant reference to \c child_t.
        typedef child_t&&                 child_move_t;            //!< Move a \c child_t.

        typedef          basic_parent_node<charT>     parent_t;           //!< The parent node type.
        typedef typename parent_t::parent_pointer_t   parent_pointer_t;   //!< A pointer to the parent type.
        typedef typename parent_t::parent_reference_t parent_reference_t; //!< A reference to the parent type.

        //!@}

        //! \brief Default constructor
        /*!
         *  This constructor initialise the internals of a child node
         *  (i.e. its parent, and its next and previous child node).
         *
         *  \param[in] parent The parent node of this one.
         */
        basic_child_node(parent_pointer_t parent = nullptr)
        :
            node_interface_t(),
            mParent(parent),
            mPrevious(nullptr),
            mNext(nullptr)
        {}

        //! \brief Copy constructor
        /*!
         *  This constructor initialise the internals of a child node
         *  (i.e. its parent, and its next and previous child node).
         *
         *  \param [in] rhs A constant reference to a \c child_t.
         */
        basic_child_node(child_const_reference_t rhs)
        :
            node_interface_t(rhs),
            mParent(nullptr),
            mPrevious(nullptr),
            mNext(nullptr)
        {}

        //! \brief Move constructor
        /*!
         *  This constructor initialise the internals of a child node
         *  (i.e.  its parent, and its next and previous child node),
         *  and takes the place of \c rhs.
         *
         *  \param [in] rhs A rvalue reference to a \c child_t.
         */
        basic_child_node(child_move_t rhs)
        :
            node_interface_t(rhs),
            mParent(rhs.mParent),
            mPrevious(rhs.mPrevious),
            mNext(rhs.mNext)
        {
            if (mPrevious)
                mPrevious->mNext = this;
            else
                mParent->mFirst = this;

            if (mNext)
                mNext->mPrevious = this;
            else
                mParent->mLast = this;
        }

        //! \brief Default destructor
        /*!
         *  This destructor reset all the internal values.
         */
        virtual ~basic_child_node()
        {
            mNext = nullptr;
            mPrevious = nullptr;
            mParent = nullptr;
        }

        //! \brief Clone the current \c basic_child_node.
        /*!
         *  This function creates a deep copy of this \c basic_child_node,
         *  and returns a pointer to it. It is used when copying element in
         *  parent node.
         */
        virtual child_pointer_t clone() const = 0;

        //! \brief Clone the given \c basic_child_node using move syntax.
        /*!
         *  This function creates a deep copy of the given\c basic_child_node,
         *  and returns a pointer to it. It is used when copying element in
         *  parent node.
         */
        virtual child_pointer_t clone(child_move_t rhs) const = 0;

        //! \brief Returns the node's parent.
        /*!
         *  This function returns a \c const reference to the node's parent.
         *  Note that a \c basic_child_node belongs to its parent node, and
         *  that the parent node will always be defined.
         *
         *  \return A \c const reference to the node's parent.
         */
        const parent_reference_t parent() const { return *mParent; }

        //! \brief Returns the node's parent.
        /*!
         *  This function returns a reference to the node's parent.
         *  Note that a \c basic_child_node belongs to its parent node, and
         *  that the parent node will always be defined.
         *
         *  \return A reference to the node's parent.
         */
        parent_reference_t parent() { return *mParent; }

    protected:
        parent_pointer_t mParent;  //!< A pointer to the current node's parent

        child_pointer_t mPrevious; //!< A pointer to the current previous node sibling. Might be \c null
        child_pointer_t mNext;     //!< A pointer to the current next node sibling. Might be \c null

        template <typename charU, class classT>
        friend class xml::basic_iterator;

        friend class basic_parent_node<charT>;
    };

    typedef basic_child_node<char>    child_node;  //!< A specialized \c basic_child_node for char.
    typedef basic_child_node<wchar_t> wchild_node; //!< A specialized \c basic_child_node for wchar_t.
}

#endif /* basic_child_node_H_INCLUDED */