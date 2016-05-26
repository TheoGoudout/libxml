#ifndef basic_parent_node_H_INCLUDED
#define basic_parent_node_H_INCLUDED

#include <iterator>
#include <initializer_list>

#include <node-interface.h>

namespace xml {
    template <typename charT>
    class basic_child_node;

    //! \brief An abstract XML node that has children.
    /*!
     *  This class represents an abstract XML node that has children.
     *
     *  \sa basic_child_node<charT>
     *
     *  \tparam charT The type of character used in the XML node.
     *                By default, char and wchar_t are supported.
     */
    template <typename charT>
    class basic_parent_node : public virtual basic_node_interface<charT> {

    public:
        //! \name Member types
        //!@{
        typedef basic_child_node<charT> child_type; //!< The type of children this node has.
        typedef child_type*             pointer;    //!< Pointer to \c child_type.
        typedef child_type&             reference;  //!< Reference to \c child_type.
        typedef size_t                  size_type;  //!< An unsigned integer representing a size.

        //!@}


        //! \brief An iterator class over \c basic_parent_node children.
        /*!
         *  This is a bidirectional iterator class that will iterate over
         *  \c basic_parent_node children. This is basically a wrapper since
         *  each child is aware of its siblings.
         *
         *  This class allows us to browse through the children of a given type.
         *  The first template argument is defined so that when you want to
         *  go to the next element, this iterator will search for the next
         *  element of type \c classT.
         *
         *  This class might be overridden in order to bond the iterations.
         *  It allows us to use iterators to insert element within a specific spot.
         *
         *  \sa xml::basic_child_node
         *
         *  \tparam classT The type of children we want to browse through.
         *  \tparam baseT  The base type of all children.
         */
        template <class classT, class baseT>
        class basic_iterator : public std::iterator<std::bidirectional_iterator_tag, classT> {
        public:
            //! \name Member types
            //!@{
            typedef typename std::iterator<std::bidirectional_iterator_tag, classT>::pointer pointer;      //!< Pointer to a \c classT
            typedef typename std::iterator<std::bidirectional_iterator_tag, classT>::reference reference;  //!< Reference to a \c classT

            //!@}

            //! \brief A \c basic_iterator constructor.
            /*!
             *  Build a \c basic_iterator holding a pointer to \c ptr.
             *
             *  \param [in] ptr A pointer this \c basic_iterator will point to.
             */
            basic_iterator(baseT* ptr = nullptr);

            //! \brief A \c basic_iterator copy-constructor.
            /*!
             *  Build a copy of \c rhs.
             *
             *  \param [in] rhs A reference to the \c basic_iterator to copy.
             */
            basic_iterator(const basic_iterator<classT, baseT>& rhs);

            //! \brief A \c basic_iterator destructor.
            /*!
             *  Destroy a \c basic_iterator
             */
            virtual ~basic_iterator();

            //! \brief A \c basic_iterator copy-assignment operator.
            /*!
             *  Make the current item a copy of \c rhs.
             *
             *  \param [in] rhs A reference to the \c basic_iterator to copy.
             */
            basic_iterator<classT, baseT>& operator=(const basic_iterator<classT, baseT>& rhs);

            //! \brief A \c basic_iterator equality operator.
            /*!
             *  Check whether two \c basic_iterator are pointing to the same item.
             *
             *  \param [in] rhs A reference to the \c basic_iterator to compare.
             *
             *  \return \c true if the two \c basic_iterator are pointing to the same item, \c false otherwise.
             *
             *  \sa operator!=()
             */
            bool operator==(const basic_iterator<classT, baseT>& rhs);

            //! \brief A \c basic_iterator inequality operator.
            /*!
             *  Check whether two \c basic_iterator are not pointing to the same item.
             *
             *  \param [in] rhs A reference to the \c basic_iterator to compare.
             *
             *  \return \c false if the two \c basic_iterator are pointing to the same item, \c true otherwise.
             *
             *  \sa operator==()
             */
            bool operator!=(const basic_iterator<classT, baseT>& rhs) { return !(*this == rhs); }

            //! \brief A \c basic_iterator indirection operator.
            /*!
             *  Get a \c reference to the item pointed by this \c basic_iterator.
             *
             *  \return A \c reference to the item pointed by this \c basic_iterator.
             *
             *  \sa operator->()
             */
            reference operator*() const;

            //! \brief A \c basic_iterator dereference operator.
            /*!
             *  Get a \c pointer to the item pointed by this \c basic_iterator.
             *
             *  \return A \c pointer to the item pointed by this \c basic_iterator.
             *
             *  \sa operator*()
             */
            pointer operator->() const;

            //! \brief A \c basic_iterator increment operator.
            /*!
             *  Make this \c basic_iterator to point to the next item.
             *
             *  \return A \c reference to the current \c basic_iterator to chain instructions.
             *
             *  \sa operator++(int)
             *  \sa operator--()
             */
            virtual basic_iterator<classT, baseT>& operator++();

            //! \brief A \c basic_iterator decrement operator.
            /*!
             *  Make this \c basic_iterator to point to the previous item.
             *
             *  \return A \c reference to the current \c basic_iterator to chain instructions.
             *
             *  \sa operator--(int)
             *  \sa operator++()
             */
            virtual basic_iterator<classT, baseT>& operator--();

            //! \brief A \c basic_iterator increment operator.
            /*!
             *  Make this \c basic_iterator to point to the next item,
             *  and returns a \c basic_iterator that is a copy of the original one.
             *
             *  \return A \c reference to a copy of the current \c basic_iterator.
             *
             *  \sa operator++()
             *  \sa operator--(int)
             */
            basic_iterator<classT, baseT>  operator++(int) { basic_iterator<classT, baseT> temp = *this; ++(*this); return temp; }

            //! \brief A \c basic_iterator decrement operator.
            /*!
             *  Make this \c basic_iterator to point to the previous item,
             *  and returns a \c basic_iterator that is a copy of the original one.
             *
             *  \return A \c reference to a copy of the current \c basic_iterator.
             *
             *  \sa operator--()
             *  \sa operator++(int)
             */
            basic_iterator<classT, baseT>  operator--(int) { basic_iterator<classT, baseT> temp = *this; --(*this); return temp; }
        private:
            pointer mPtr; //!< A pointer to the current item this \c basic_iterator points to.
        };

        //! \name Iterator types
        //!@{

        //! Iterator type.
        /*!
         *  \tparam classT The type of object to iterate through.
         *
         *  \sa basic_iterator
         *  \sa child_type
         */
        template <class classT = child_type>
        using iterator = basic_iterator<classT, child_type>;

        //! Constant iterator type.
        /*!
         *  \tparam classT The type of object to iterate through.
         *
         *  \sa basic_iterator
         *  \sa child_type
         */
        template <class classT = child_type>
        using const_iterator = basic_iterator<const classT, const child_type>;

        //! Reverse itrator type.
        /*!
         *  \tparam classT The type of object to iterate through.
         *
         *  \sa basic_iterator
         *  \sa child_type
         */
        template <class classT = child_type>
        using reverse_iterator = std::reverse_iterator<iterator<classT> >;

        //! Constant reverse iterator type.
        /*!
         *  \tparam classT The type of object to iterate through.
         *
         *  \sa basic_iterator
         *  \sa child_type
         */
        template <class classT = child_type>
        using const_reverse_iterator = std::reverse_iterator<const_iterator<classT> >;

        //!@}


        //! \brief Return iterator to beginning.
        /*!
         *  Returns an \c iterator pointing to the first element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        iterator<classT> begin() noexcept;

        //! \brief Return iterator to beginning.
        /*!
         *  Returns a \c const_iterator pointing to the first element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        const_iterator<classT> begin() const noexcept;

        //! \brief Return iterator to past-the-end.
        /*!
         *  Returns an \c iterator pointing to the past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        iterator<classT> end() noexcept;


        //! \brief Return iterator to past-the-end.
        /*!
         *  Returns a \c const_iterator pointing to the past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        const_iterator<classT> end() const noexcept;

        //! \brief Return reverse iterator to reverse beginning.
        /*!
         *  Returns a \c reverse_iterator pointing to the last element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        reverse_iterator<classT> rbegin() noexcept;

        //! \brief Return reverse iterator to reverse beginning.
        /*!
         *  Returns a \c const_reverse_iterator pointing to the last element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        const_reverse_iterator<classT> rbegin() const noexcept;

        //! \brief Return reverse iterator to reverse past-the-end.
        /*!
         *  Returns a \c reverse_iterator pointing to the reverse past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        reverse_iterator<classT> rend() noexcept;

        //! \brief Return reverse iterator to reverse past-the-end.
        /*!
         *  Returns a \c const_reverse_iterator pointing to the reverse past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        const_reverse_iterator<classT> rend() const noexcept;

        //! \brief Return iterator to beginning.
        /*!
         *  Returns a \c const_iterator pointing to the first element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        const_iterator<classT> cbegin() const noexcept { return begin(); }

        //! \brief Return iterator to past-the-end.
        /*!
         *  Returns a \c const_iterator pointing to the past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        const_iterator<classT> cend() const noexcept { return end(); }

        //! \brief Return reverse iterator to reverse beginning.
        /*!
         *  Returns a \c const_reverse_iterator pointing to the last element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        const_reverse_iterator<classT> crbegin() const noexcept { return rbegin(); }

        //! \brief Return reverse iterator to reverse past-the-end.
        /*!
         *  Returns a \c const_reverse_iterator pointing to the reverse past-the-end element.
         *
         *  \tparam classT The type of object to iterate through.
         */
        template <class classT = child_type>
        const_reverse_iterator<classT> crend() const noexcept { return rend(); }

    protected:
        //! \brief Copy a \c child_type into the inserted elements.
        /*!
         *  The \c child_type will be copied and inserted as children of this
         *  \c basic_parent_node before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be inserted.
         *  \param [in] val      The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        iterator<> insert(const_iterator<> position, const child_type& val);

        //! \brief Copy a \c child_type \c n times into the inserted elements.
        /*!
         *  The \c child_type will be copied and inserted as children of this
         *  \c basic_parent_node \c n times before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be inserted \c n times.
         *  \param [in] n        The number of time \c val should be inserted.
         *  \param [in] val      The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the first newly inserted element.
         */
        iterator<> insert(const_iterator<> position, size_type n, const child_type& val);

        //! \brief Copy a set of \c child_type into the inserted elements.
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
        template <class InputIterator>
        iterator<> insert (const_iterator<> position, InputIterator first, InputIterator last);

        //! \brief Move a \c child_type into the inserted elements.
        /*!
         *  The value will be moved into the list of children of this
         *  \c basic_parent_node before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be moved.
         *  \param [in] val      The value to be moved.
         *
         *  \return An \c iterator pointing to the first newly inserted element.
         */
        iterator<> insert (const_iterator<> position, child_type&& val);

        //! \brief Copy a list of \c child_type into the inserted elements.
        /*!
         *  Each value in the \c initializer_list will be copied and inserted
         *  as children of this \c basic_parent_node before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which the elements of the list should be inserted.
         *  \param [in] il       The list of elements to be inserted.
         *
         *  \return An \c iterator pointing to the first newly inserted element.
         */
        iterator<> insert (const_iterator<> position, std::initializer_list<child_type> il);

        //! \brief Copy a \c child_type and insert it before the first element.
        /*!
         *  The \c child_type will be copied and inserted as children of this
         *  \c basic_parent_node in front of the first element.
         *
         *  \param [in] val The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        iterator<> push_front(const child_type& val) { return insert(cbegin(), val); }

        //! \brief Move a \c child_type before the first element.
        /*!
         *  The \c child_type will be moved as children of this
         *  \c basic_parent_node in front of the first element.
         *
         *  \param [in] val The value to be moved.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        iterator<> push_front(child_type&& val) { return insert(cbegin(), val); }

        //! \brief Copy a \c child_type and insert it after the last element.
        /*!
         *  The \c child_type will be copied and inserted as children of this
         *  \c basic_parent_node after the last element.
         *
         *  \param [in] val The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        iterator<> push_back(const child_type& val) { return insert(cend(), val); }

        //! \brief Move a \c child_type after the last element.
        /*!
         *  The \c child_type will be moved as children of this
         *  \c basic_parent_node after the last element.
         *
         *  \param [in] val The value to be moved.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        iterator<> push_back(child_type&& val) { return insert(cend(), val); }

        //! \brief Allocate a \c child_type and insert it into the inserted elements.
        /*!
         *  The \c child_type will be allocated and inserted as children of this
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
        template <class classT, typename ... Args>
        iterator<> emplace(const_iterator<> position, Args&& ... args);

        //! \brief Allocate a \c child_type and insert it before the first element.
        /*!
         *  The \c child_type will be allocated and inserted as children of this
         *  \c basic_parent_node before the first element.
         *
         *  \tparam classT The class to be instantiated
         *  \tparam Args   The argument types used to instantiate \c classT
         *
         *  \param [in] args     The arguments used to instantiate \c classT.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT, typename ... Args>
        iterator<> emplace_front(Args&& ... args) { return emplace(cbegin(), args ...); }

        //! \brief Allocate a \c child_type and insert it after the last element.
        /*!
         *  The \c child_type will be allocated and inserted as children of this
         *  \c basic_parent_node after the last element.
         *
         *  \tparam classT The class to be instantiated
         *  \tparam Args   The argument types used to instantiate \c classT
         *
         *  \param [in] args     The arguments used to instantiate \c classT.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT, typename ... Args>
        iterator<> emplace_back(Args&& ... args) { return emplace(cend(), args ...); }

        //! \brief Erase element.
        /*!
         *  The element pointed by the \c iterator will be erased.
         *
         *  \param [in] position An iterator to the element to erase.
         *
         *  \return An iterator pointing to the element that followed the erased element. 
         */
        iterator<> erase (iterator<> position);

        //! \brief Erase a range of elements.
        /*!
         *  The elements within the range of \c iterator will be erased.
         *
         *  \param [in] first The first element to erase.
         *  \param [in] last  The first element not to erase after first.
         *
         *  \return An iterator pointing to the element that followed the erased element. 
         */
        iterator<> erase (iterator<> first, iterator<> last);

        //! \brief Delete first element.
        /*!
         *  Removes the first element and destroy it.
         */
        void pop_front() { erase(begin()); }

        //! \brief Delete last element.
        /*!
         *  Removes the last element and destroy it.
         */
        void pop_back() { erase(rbegin().base()); }

        //! \brief Delete all elements.
        /*!
         *  Removes all elements and destroy them.
         */
        void clear() noexcept;
    private:
        pointer mFirst; //!< A pointer to the first element.
        pointer mLast;  //!< A pointer to the last element.
    };

    typedef basic_parent_node<char>    parent_node;  //!< A specialized \c basic_parent_node for char.
    typedef basic_parent_node<wchar_t> wparent_node; //!< A specialized \c basic_parent_node for wchar_t.
}

#endif /* basic_parent_node_H_INCLUDED */