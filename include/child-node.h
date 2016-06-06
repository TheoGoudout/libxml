#ifndef basic_child_node_H_INCLUDED
#define basic_child_node_H_INCLUDED

#include <node-interface.h>
#include <parent-node.h>

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
        typedef          basic_parent_node<charT>  parent_t;        //!< The parent node type.
        typedef typename parent_t::child_pointer_t child_pointer_t; //!< The child node type.

        typedef          parent_t*                 parent_pointer_t;   //!< A pointer to the parent type.
        typedef          parent_t&                 parent_reference_t; //!< A reference to the parent type.

        //!@}

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

    private:
        parent_pointer_t mParent;  //!< A pointer to the current node's parent

        child_pointer_t mPrevious; //!< A pointer to the current previous node sibling. Might be \c null
        child_pointer_t mNext;     //!< A pointer to the current next node sibling. Might be \c null

        friend class basic_parent_node<charT>;
    };

    typedef basic_child_node<char>    child_node;  //!< A specialized \c basic_child_node for char.
    typedef basic_child_node<wchar_t> wchild_node; //!< A specialized \c basic_child_node for wchar_t.
}

#endif /* basic_child_node_H_INCLUDED */