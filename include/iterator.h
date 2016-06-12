#ifndef ITERATOR_H_INCLUDED
#define ITERATOR_H_INCLUDED

#include <parent-node.h>
#include <child-node.h>

namespace xml {
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
     *  \tparam charT The type of character used in the XML node.
     *                By default, char and wchar_t are supported.
     *  \tparam classT The type of children we want to browse through.
     */
    template <typename charT, class classT>
    class basic_iterator : public std::iterator<std::bidirectional_iterator_tag, classT>
    {
    public:
        //! \name Member types
        //!@{
        typedef typename std::iterator<std::bidirectional_iterator_tag, classT>::pointer   pointer_t;    //!< Pointer to a \c classT
        typedef typename std::iterator<std::bidirectional_iterator_tag, classT>::reference reference_t;  //!< Reference to a \c classT

        typedef          basic_child_node<charT> child_t;
        typedef          child_t*                child_pointer_t;

        //!@}

        //! \brief A \c basic_iterator constructor.
        /*!
         *  Build a \c basic_iterator holding a pointer to \c ptr.
         *
         *  \param [in] ptr A pointer this \c basic_iterator will point to.
         */
        basic_iterator(child_pointer_t ptr)
        :
            mPtr(findNext(ptr))
        {}

        //! \brief A \c basic_iterator copy-constructor.
        /*!
         *  Build a copy of \c rhs.
         *
         *  \param [in] rhs A reference to the \c basic_iterator to copy.
         */
        basic_iterator(const basic_iterator<charT, classT>& rhs)
        :
            mPtr(rhs.mPtr)
        {}

        //! \brief A \c basic_iterator destructor.
        /*!
         *  Destroy a \c basic_iterator
         */
        virtual ~basic_iterator()
        {}

        //! \brief An implicit conversion function.
        /*!
         *  This function converts iterators from one type to another.
         *
         *  \return The \c basic_iterator with a new type.
         */
        template <class classU>
        operator basic_iterator<charT, classU> () const
        {
            return basic_iterator<charT, classU>(mPtr);
        }

        //! \brief A \c basic_iterator copy-assignment operator.
        /*!
         *  Make the current item a copy of \c rhs.
         *
         *  \param [in] rhs A reference to the \c basic_iterator to copy.
         */
        basic_iterator<charT, classT>& operator=(const basic_iterator<charT, classT>& rhs)
        {
            mPtr = rhs.mPtr;
            return *this;
        }

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
        bool operator==(const basic_iterator<charT, classT>& rhs)
        {
            return mPtr == rhs.mPtr;
        }

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
        bool operator!=(const basic_iterator<charT, classT>& rhs)
        {
            return !(*this == rhs);
        }

        //! \brief A \c basic_iterator indirection operator.
        /*!
         *  Get a \c reference to the item pointed by this \c basic_iterator.
         *
         *  \return A \c reference to the item pointed by this \c basic_iterator.
         *
         *  \sa operator->()
         */
        reference_t operator*() const
        {
            return static_cast<reference_t>(*mPtr);
        }

        //! \brief A \c basic_iterator dereference operator.
        /*!
         *  Get a \c pointer to the item pointed by this \c basic_iterator.
         *
         *  \return A \c pointer to the item pointed by this \c basic_iterator.
         *
         *  \sa operator*()
         */
        pointer_t operator->() const
        {
            return static_cast<pointer_t>(mPtr);
        }

        //! \brief A \c basic_iterator increment operator.
        /*!
         *  Make this \c basic_iterator to point to the next item.
         *
         *  \return A \c reference to the current \c basic_iterator to chain instructions.
         *
         *  \sa operator++(int)
         *  \sa operator--()
         */
        virtual basic_iterator<charT, classT>& operator++()
        {
            mPtr = findNext(mPtr->mNext);
            return *this;
        }

        //! \brief A \c basic_iterator decrement operator.
        /*!
         *  Make this \c basic_iterator to point to the previous item.
         *
         *  \return A \c reference to the current \c basic_iterator to chain instructions.
         *
         *  \sa operator--(int)
         *  \sa operator++()
         */
        virtual basic_iterator<charT, classT>& operator--()
        {
            mPtr = findPrevious(mPtr->mPrevious);
            return *this;
        }

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
        basic_iterator<charT, classT>  operator++(int)
        {
            basic_iterator<charT, classT> temp = *this;
            ++(*this);
            return temp;
        }

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
        basic_iterator<charT, classT>  operator--(int)
        {
            basic_iterator<charT, classT> temp = *this;
            --(*this);
            return temp;
        }
    private:

        //! Find the next \c pointer_t.
        /*!
         *  This function will find the next \c child_pointer_t that can be
         *  casted to a \c pointer_t. If it cannot be found, \c nullptr
         *  will be returned.
         *
         *  \param[in] cptr Pointer to a \c child_t element.
         *
         *  \return Next pointer to a \c pointer_t, or \c nullptr.
         */
        static child_pointer_t findNext(child_pointer_t cptr)
        {
            pointer_t ptr = static_cast<pointer_t>(cptr);

            while(cptr != nullptr && ptr == nullptr) {
                cptr = cptr->mNext;
                ptr = static_cast<pointer_t>(cptr);
            }

            return cptr;
        }

        //! Find the previous \c pointer_t.
        /*!
         *  This function will find the previous \c child_pointer_t that
         *  can be casted to a \c pointer_t. If it cannot be found,
         *  \c nullptr will be returned.
         *
         *  \param[in] cptr Pointer to a \c child_t element.
         *
         *  \return Previous pointer to a \c pointer_t, or \c nullptr.
         */
        static child_pointer_t findPrevious(child_pointer_t cptr)
        {
            pointer_t ptr = static_cast<pointer_t>(cptr);

            while(cptr != nullptr && ptr == nullptr) {
                cptr = cptr->mPrevious;
                ptr = static_cast<pointer_t>(cptr);
            }

            return cptr;
        }

        child_pointer_t mPtr; //!< A pointer to the current item this \c basic_iterator points to.

        friend class basic_parent_node<charT>;
    };
}

#endif /* ITERATOR_H_INCLUDED */