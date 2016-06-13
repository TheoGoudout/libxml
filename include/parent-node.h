#ifndef basic_parent_node_H_INCLUDED
#define basic_parent_node_H_INCLUDED

#include <iterator>
#include <initializer_list>

#include <node-interface.h>
#include <child-node.h>
#include <iterator.h>

namespace xml {
    template <typename charT>
    class basic_child_node;

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
        typedef basic_node_interface<charT> node_interface_t; //! The base type of this node.

        typedef          basic_child_node<charT>    child_t;           //!< The type of children this node has.
        typedef typename child_t::child_pointer_t   child_pointer_t;   //!< Pointer to \c child_t.
        typedef typename child_t::child_reference_t child_reference_t; //!< Reference to \c child_t.

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
         *  This constructor initialise the internals of a parent node, i.e.
         *  its first and last child node.
         */
        basic_parent_node ()
        :
            node_interface_t(),
            mFirst(nullptr),
            mLast(nullptr)
        {}

        //! \brief Default destructor
        /*!
         *  This destructor reset all the internal values.
         */
        virtual ~basic_parent_node ()
        {
            clear();

            mFirst = nullptr;
            mLast = nullptr;
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
            return std::distance(cbegin(), cend());
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
        iterator<classT> insert (const_iterator<classT> position, const child_t& val)
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
        iterator<classT> insert (const_iterator<classT> position, size_t n, const child_t& val)
        {
            if (n == 0)
                return iterator<classT>(position.mPtr);

            const_iterator<classT> it = insert(position, n - 1, val);

            return insert(it, val);
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
        iterator<classT> insert (const_iterator<classT> position, InputIterator first, InputIterator last)
        {
            if (first == last)
                return iterator<classT>(position.mPtr);

            const_iterator<classT> it = insert(position, first++, last);

            return insert(it, *first);
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
        iterator<classT> insert (const_iterator<classT> position, child_t&& val)
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
        template <class classT = child_t>
        iterator<classT> insert (const_iterator<classT> position, std::initializer_list<child_t> il)
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
        iterator<classT> push_front (const child_t& val)
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
        iterator<classT> push_front (child_t&& val)
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
        iterator<classT> push_back (const child_t& val)
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
        iterator<classT> push_back (child_t&& val)
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
        template <class classT = child_t, class classU, typename ... Args>
        iterator<classT> emplace (const_iterator<classT> position, Args&& ... args)
        {
            return insert(position, new classU(std::forward(args ...)));
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
        template <class classT = child_t, class classU, typename ... Args>
        iterator<classT> emplace_front (Args&& ... args)
        {
            return emplace(cbegin(), std::forward(args ...));
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
        template <class classT = child_t, class classU, typename ... Args>
        iterator<classT> emplace_back (Args&& ... args)
        {
            return emplace(cend(), std::forward(args ...));
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
            child_pointer_t ptr = position.mPtr;

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

            delete ptr;

            return iterator<classT>(next);
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

            const_iterator<classT> it = erase(first);

            return erase(it, last);
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
         *  \param [in] val      The pointer to be insert.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> insert (const_iterator<classT> position, child_pointer_t ptr)
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

            return iterator<classT>(ptr);
        }

    protected:
        child_pointer_t mFirst; //!< A pointer to the first element.
        child_pointer_t mLast;  //!< A pointer to the last element.
    };

    typedef basic_parent_node<char>    parent_node;  //!< A specialized \c basic_parent_node for char.
    typedef basic_parent_node<wchar_t> wparent_node; //!< A specialized \c basic_parent_node for wchar_t.
}

#endif /* basic_parent_node_H_INCLUDED */