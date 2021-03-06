#ifndef basic_parent_node_H_INCLUDED
#define basic_parent_node_H_INCLUDED

#include <cassert>
#include <iterator>
#include <initializer_list>

namespace xml {
    template <typename charT>
    class basic_parent_node;
}

#include <node-interface.h>
#include <child-node.h>
#include <iterator.h>

namespace xml {
    //! \brief An abstract XML node that has children.
    /*!
     *  This class represents an abstract XML node that has children.
     *
     *  \sa xml::basic_child_node
     *
     *  \tparam charT The type of character used in the XML node.
     *                By default, char and wchar_t are supported.
     */
    template <typename charT>
    class basic_parent_node : public virtual basic_node_interface<charT> {

    public:
        //! \name Member types
        //!@{
        typedef basic_node_interface<charT> node_interface_t; //!< The base type of this node.

        typedef basic_parent_node<charT>   parent_t;                 //!< The type of parent this node is.
        typedef parent_t*                  parent_pointer_t;         //!< Pointer to \c parent_t.
        typedef parent_t&                  parent_reference_t;       //!< Reference to \c parent_t.
        typedef const parent_t&            parent_const_reference_t; //!< Constant reference to \c parent_t.
        typedef parent_t&&                 parent_move_t;            //!< Move a \c parent_t.

        typedef          basic_child_node<charT>          child_t;                 //!< The type of children this node has.
        typedef typename child_t::child_pointer_t         child_pointer_t;         //!< Pointer to \c child_t.
        typedef typename child_t::child_reference_t       child_reference_t;       //!< Reference to \c child_t.
        typedef typename child_t::child_const_reference_t child_const_reference_t; //!< Constant reference to \c child_t.
        typedef typename child_t::child_move_t            child_move_t;            //!< Move a \c child_t.

        //!@}

        //! \name Iterator types
        //!@{

        //! Iterator type.
        /*!
         *  \tparam classT The type of object to iterate through.
         *
         *  \sa basic_iterator
         *  \sa child_t
         */
        template <class classT = child_t>
        using iterator = basic_iterator<charT, classT>;

        //! Constant iterator type.
        /*!
         *  \tparam classT The type of object to iterate through.
         *
         *  \sa basic_iterator
         *  \sa child_t
         */
        template <class classT = child_t>
        using const_iterator = basic_iterator<charT, const classT>;

        //! Reverse itrator type.
        /*!
         *  \tparam classT The type of object to iterate through.
         *
         *  \sa basic_iterator
         *  \sa child_t
         */
        template <class classT = child_t>
        using reverse_iterator = std::reverse_iterator<iterator<classT> >;

        //! Constant reverse iterator type.
        /*!
         *  \tparam classT The type of object to iterate through.
         *
         *  \sa basic_iterator
         *  \sa child_t
         */
        template <class classT = child_t>
        using const_reverse_iterator = std::reverse_iterator<const_iterator<classT> >;

        //!@}

        //! \brief Default constructor
        /*!
         *  This constructor initialise the internals of a parent node
         *  (i.e. its first and last child node).
         */
        basic_parent_node ()
        :
            node_interface_t(),
            mSize(0),
            mFirst(nullptr),
            mLast(nullptr)
        {}

        //! \brief Copy constructor
        /*!
         *  This constructor initialise the internals of a parent node
         *  (i.e. its first and last child node), and creates a copy of all
         *  \c rhs children and insert them into the newly created copy.
         *
         *  \param [in] rhs A constant reference to a \c parent_t.
         */
        basic_parent_node (parent_const_reference_t rhs)
        :
            node_interface_t(rhs),
            mSize(0),
            mFirst(nullptr),
            mLast(nullptr)
        {
            insert(cbegin(), rhs.cbegin(), rhs.cend());
        }

        //! \brief Move constructor
        /*!
         *  This constructor initialise the internals of a parent node
         *  (i.e. its first and last child node), and takes the place of \c rhs.
         *
         *  \param [in] rhs A rvalue reference to a \c parent_t.
         */
        basic_parent_node (parent_move_t rhs)
        :
            node_interface_t(rhs),
            mSize(0),
            mFirst(nullptr),
            mLast(nullptr)
        {
            while (rhs.size() > 0)
            {
                auto ptr = rhs.mFirst;

                remove(ptr);
                insert(cend(), ptr);
            }
        }

        //! \brief Default destructor
        /*!
         *  This destructor reset all the internal values.
         */
        virtual ~basic_parent_node ()
        {
            clear();

            mSize  = 0;
            mFirst = nullptr;
            mLast  = nullptr;
        }

        //! \brief Return iterator to beginning.
        /*!
         *  Returns an \c iterator pointing to the first element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        iterator<classT> begin () noexcept
        {
            return iterator<classT>(mFirst);
        }

        //! \brief Return iterator to beginning.
        /*!
         *  Returns a \c const_iterator pointing to the first element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        const_iterator<classT> begin () const noexcept
        {
            return const_iterator<classT>(mFirst);
        }

        //! \brief Return iterator to past-the-end.
        /*!
         *  Returns an \c iterator pointing to the past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        iterator<classT> end () noexcept
        {
            return iterator<classT>(nullptr);
        }

        //! \brief Return iterator to past-the-end.
        /*!
         *  Returns a \c const_iterator pointing to the past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        const_iterator<classT> end () const noexcept
        {
            return const_iterator<classT>(nullptr);
        }

        //! \brief Return reverse iterator to reverse beginning.
        /*!
         *  Returns a \c reverse_iterator pointing to the last element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        reverse_iterator<classT> rbegin () noexcept
        {
            return reverse_iterator<classT>(nullptr);
        }

        //! \brief Return reverse iterator to reverse beginning.
        /*!
         *  Returns a \c const_reverse_iterator pointing to the last element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        const_reverse_iterator<classT> rbegin () const noexcept
        {
            return const_reverse_iterator<classT>(nullptr);
        }

        //! \brief Return reverse iterator to reverse past-the-end.
        /*!
         *  Returns a \c reverse_iterator pointing to the reverse past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        reverse_iterator<classT> rend () noexcept
        {
            return reverse_iterator<classT>(mFirst);
        }

        //! \brief Return reverse iterator to reverse past-the-end.
        /*!
         *  Returns a \c const_reverse_iterator pointing to the reverse past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        const_reverse_iterator<classT> rend () const noexcept
        {
            return const_reverse_iterator<classT>(mFirst);
        }

        //! \brief Return iterator to beginning.
        /*!
         *  Returns a \c const_iterator pointing to the first element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        const_iterator<classT> cbegin () const noexcept
        {
            return begin();
        }

        //! \brief Return iterator to past-the-end.
        /*!
         *  Returns a \c const_iterator pointing to the past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        const_iterator<classT> cend () const noexcept
        {
            return end();
        }

        //! \brief Return reverse iterator to reverse beginning.
        /*!
         *  Returns a \c const_reverse_iterator pointing to the last element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        const_reverse_iterator<classT> crbegin () const noexcept
        {
            return rbegin();
        }

        //! \brief Return reverse iterator to reverse past-the-end.
        /*!
         *  Returns a \c const_reverse_iterator pointing to the reverse past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_t>
        const_reverse_iterator<classT> crend () const noexcept
        {
            return rend();
        }

        //! \brief Get the number of children.
        /*!
         *  This function returns the number of children this node has.
         *  It is equivalent to `std::distance(begin(), end());`
         *
         *  \return The number of children.
         */
        size_t size () const
        {
            return mSize;
        }

        //! \brief Whether the node is empty.
        /*!
         *  This function checks if the current node is empty.
         *  This is equivalent to `size() == 0`.
         *
         *  \return \c true if the current node is empty, \c false otherwise.
         */
        bool empty() const noexcept
        {
            return size() == 0;
        }

        //! \brief Access first element
        /*!
         *  Returns a reference to the first child of this node.
         *  Unlike member basic_parent_node::begin, which returns an iterator
         *  to this same element, this function returns a direct reference.
         *  Calling this function on an empty node causes undefined behaviour.
         *
         *  \return A reference to the first child of this node.
         */
        child_reference_t front()
        {
            return *mFirst;
        }

        //! \brief Access first element
        /*!
         *  Returns a reference to the first child of this node.
         *  Unlike member basic_parent_node::begin, which returns an iterator
         *  to this same element, this function returns a direct reference.
         *  Calling this function on an empty node causes undefined behaviour.
         *
         *  \return A constant reference to the first child of this node.
         */
        child_const_reference_t front() const
        {
            return *mFirst;
        }

        //! \brief Access last element
        /*!
         *  Returns a reference to the last child of this node.
         *  Unlike member basic_parent_node::end, which returns an iterator
         *  just past this element, this function returns a direct reference.
         *  Calling this function on an empty node causes undefined behaviour.
         *
         *  \return A reference to the last child of this node.
         */
        child_reference_t back()
        {
            return *mLast;
        }

        //! \brief Access last element
        /*!
         *  Returns a reference to the last child of this node.
         *  Unlike member basic_parent_node::end, which returns an iterator
         *  just past this element, this function returns a direct reference.
         *  Calling this function on an empty node causes undefined behaviour.
         *
         *  \return A constant reference to the last child of this node.
         */
        child_const_reference_t back() const
        {
            return *mLast;
        }

    protected:
        //! \brief Copy a \c child_t into the inserted elements.
        /*!
         *  The \c child_t will be copied and inserted as children of this
         *  \c basic_parent_node before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be inserted.
         *  \param [in] val      The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> insert (iterator<classT> position, child_const_reference_t val)
        {
            return insert(position, val.clone());
        }

        //! \brief Copy a \c child_t \c n times into the inserted elements.
        /*!
         *  The \c child_t will be copied and inserted as children of this
         *  \c basic_parent_node \c n times before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be inserted \c n times.
         *  \param [in] n        The number of time \c val should be inserted.
         *  \param [in] val      The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the first newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> insert (iterator<classT> position, size_t n, child_const_reference_t val)
        {
            if (n == 0)
                return position;

            return insert(insert(position, n - 1, val), val);
        }

        //! \brief Copy a set of \c child_t into the inserted elements.
        /*!
         *  Each value in between \c first (included) and last (excluded) will be
         *  copied an inserted as children of this \c basic_parent_node before the item
         *  pointed by the \c const_iterator.
         *
         *  \tparam InputIterator \c basic_iterator to elements of a \c basic_parent_node
         *
         *  \param [in] position A \c const_iterator before which the new element should be inserted.
         *  \param [in] first    The first value to be inserted.
         *  \param [in] last     The first value that should not be inserted before \c first.
         *
         *  \return An \c iterator pointing to the first newly inserted element.
         */
        template <class InputIterator, class classT = child_t>
        iterator<classT> insert (iterator<classT> position, InputIterator first, InputIterator last)
        {
            if (first == last)
                return position;

            auto it = first->clone();

            return insert(insert(position, ++first, last), it);
        }

        //! \brief Move a \c child_t into the inserted elements.
        /*!
         *  The value will be moved into the list of children of this
         *  \c basic_parent_node before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be moved.
         *  \param [in] val      The value to be moved.
         *
         *  \return An \c iterator pointing to the first newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> insert (iterator<classT> position, child_move_t val)
        {
            return insert(position, val.clone(std::move(val)));
        }

        //! \brief Copy a list of \c child_t into the inserted elements.
        /*!
         *  Each value in the \c initializer_list will be copied and inserted
         *  as children of this \c basic_parent_node before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which the elements of the list should be inserted.
         *  \param [in] il       The list of elements to be inserted.
         *
         *  \return An \c iterator pointing to the first newly inserted element.
         */
        template <class classT = child_t, class classU>
        iterator<classT> insert (iterator<classT> position, std::initializer_list<classU> il)
        {
            return insert(position, il.begin(), il.end());
        }

        //! \brief Copy a \c child_t and insert it before the first element.
        /*!
         *  The \c child_t will be copied and inserted as children of this
         *  \c basic_parent_node in front of the first element.
         *
         *  \param [in] val The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_front (child_const_reference_t val)
        {
            return insert(cbegin(), val);
        }

        //! \brief Move a \c child_t before the first element.
        /*!
         *  The \c child_t will be moved as children of this
         *  \c basic_parent_node in front of the first element.
         *
         *  \param [in] val The value to be moved.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_front (child_move_t val)
        {
            return insert(cbegin(), std::move(val));
        }

        //! \brief Copy a \c child_t and insert it after the last element.
        /*!
         *  The \c child_t will be copied and inserted as children of this
         *  \c basic_parent_node after the last element.
         *
         *  \param [in] val The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_back (child_const_reference_t val)
        {
            return insert(cend(), val);
        }

        //! \brief Move a \c child_t after the last element.
        /*!
         *  The \c child_t will be moved as children of this
         *  \c basic_parent_node after the last element.
         *
         *  \param [in] val The value to be moved.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_back (child_move_t val)
        {
            return insert(cend(), std::move(val));
        }

        //! \brief Allocate a \c child_t and insert it into the inserted elements.
        /*!
         *  The \c child_t will be allocated and inserted as children of this
         *  \c basic_parent_node before the item pointed by the \c const_iterator.
         *
         *  \tparam classT The class to be instantiated
         *  \tparam Args   The argument types used to instantiate \c classT
         *
         *  \param [in] position A \c const_iterator before which \c val should be inserted.
         *  \param [in] args     The arguments used to instantiate \c classT.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classU, typename ... Args, class classT = child_t>
        iterator<classT> emplace (iterator<classT> position, Args&& ... args)
        {
            return insert(position, new classU(std::forward<Args>(args) ...));
        }

        //! \brief Allocate a \c child_t and insert it before the first element.
        /*!
         *  The \c child_t will be allocated and inserted as children of this
         *  \c basic_parent_node before the first element.
         *
         *  \tparam classT The class to be instantiated
         *  \tparam Args   The argument types used to instantiate \c classT
         *
         *  \param [in] args     The arguments used to instantiate \c classT.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classU, typename ... Args, class classT = child_t>
        iterator<classT> emplace_front (Args&& ... args)
        {
            return emplace<classU>(cbegin(), std::forward<Args>(args) ...);
        }

        //! \brief Allocate a \c child_t and insert it after the last element.
        /*!
         *  The \c child_t will be allocated and inserted as children of this
         *  \c basic_parent_node after the last element.
         *
         *  \tparam classT The class to be instantiated
         *  \tparam Args   The argument types used to instantiate \c classT
         *
         *  \param [in] args     The arguments used to instantiate \c classT.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classU, typename ... Args, class classT = child_t>
        iterator<classT> emplace_back (Args&& ... args)
        {
            return emplace<classU>(cend(), std::forward<Args>(args) ...);
        }

        //! \brief Erase element.
        /*!
         *  The element pointed by the \c iterator will be erased.
         *
         *  \param [in] position An iterator to the element to erase.
         *
         *  \return An iterator pointing to the element that followed the erased element.
         */
        template <class classT = child_t>
        iterator<classT> erase (iterator<classT> position)
        {
            auto it = remove(position.mPtr);

            delete position.mPtr;

            return it;
        }

        //! \brief Erase a range of elements.
        /*!
         *  The elements within the range of \c iterator will be erased.
         *
         *  \param [in] first The first element to erase.
         *  \param [in] last  The first element not to erase after first.
         *
         *  \return An iterator pointing to the element that followed the erased element.
         */
        template <class classT = child_t>
        iterator<classT> erase (iterator<classT> first, iterator<classT> last)
        {
            if (first == last)
                return last;

            return erase(erase(first), last);
        }

        //! \brief Delete first element.
        /*!
         *  Removes the first element and destroy it.
         */
        void pop_front ()
        {
            erase(begin());
        }

        //! \brief Delete last element.
        /*!
         *  Removes the last element and destroy it.
         */
        void pop_back ()
        {
            erase(rbegin().base());
        }

        //! \brief Delete all elements.
        /*!
         *  Removes all elements and destroy them.
         */
        void clear () noexcept
        {
            erase(cbegin(), cend());
        }

    private:
        //! \brief Insert a \c child_t into the inserted elements.
        /*!
         *  The pointer will be inserted into the list of children of this
         *  \c basic_parent_node before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be moved.
         *  \param [in] val      The pointer to insert.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> insert (iterator<classT> position, child_pointer_t ptr)
        {
            child_pointer_t after = position.mPtr;
            child_pointer_t before = after == nullptr ? mLast : after->mPrevious;

            ptr->mParent = this;

            ptr->mNext = after;
            if (after != nullptr)
                after->mPrevious = ptr;
            else
                mLast = ptr;

            ptr->mPrevious = before;
            if (before != nullptr)
                before->mNext = ptr;
            else
                mFirst = ptr;

            ++mSize;

            return iterator<classT>(ptr);
        }

        //! \brief Remove element.
        /*!
         *  The element pointed by the \c child_pointer_t will be remove.
         *  The element is not deleted.
         *
         *  \param [in] ptr The pointer to remove.
         *
         *  \return An iterator pointing to the element that followed the erased element.
         */
        template <class classT = child_t>
        iterator<classT> remove (child_pointer_t ptr)
        {
            assert(ptr->mParent == this);

            child_pointer_t next = ptr->mNext;
            child_pointer_t previous = ptr->mPrevious;

            if (next)
                next->mPrevious = previous;
            else
                mLast = previous;

            if (previous)
                previous->mNext = next;
            else
                mFirst = next;

            --mSize;

            return iterator<classT>(next);
        }

    protected:
        size_t mSize; //!< The number of children owned by this node.

        child_pointer_t mFirst; //!< A pointer to the first element.
        child_pointer_t mLast;  //!< A pointer to the last element.

        friend class basic_child_node<charT>;
    };

    typedef basic_parent_node<char>    parent_node;  //!< A specialized \c basic_parent_node for char.
    typedef basic_parent_node<wchar_t> wparent_node; //!< A specialized \c basic_parent_node for wchar_t.
}

#endif /* basic_parent_node_H_INCLUDED */