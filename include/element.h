#ifndef ELEMENT_H_INCLUDED
#define ELEMENT_H_INCLUDED

#include <set>

#include <node.h>
#include <attribute.h>
#include <text.h>

namespace xml {

    //! \brief A XML element.
    /*!
     *  This class represents an XML element. It can be an empty tag or it can
     *  have several children. It also has attributes.
     *
     *  \tparam charT The type of character used in the name and value.
     *                By default, char and wchar_t are supported.
     */
    template <typename charT>
    class basic_element : public basic_node<charT> {
    public:
        //! \name Member types
        //!@{
        typedef          basic_node_interface<charT> node_interface_t; //!< The base type of this node.
        typedef typename node_interface_t::type_t    type_t;           //!< The type of a node type.

        typedef          basic_parent_node<charT>   parent_t;         //!< The parent type.
        typedef typename parent_t::parent_pointer_t parent_pointer_t; //!< Pointer to \c parent_t.

        typedef          basic_child_node<charT>  child_t;         //!< The child type.
        typedef typename child_t::child_pointer_t child_pointer_t; //!< Pointer to \c child_t.
        typedef typename child_t::child_move_t    child_move_t;    //!< Move a \c child_t.

        typedef basic_node<charT> node_t; //! The node type.

        typedef basic_element<charT> element_t;                 //!< The type of element this node is.
        typedef element_t*           element_pointer_t;         //!< Pointer to \c element_t.
        typedef element_t&           element_reference_t;       //!< Reference to \c element_t.
        typedef const element_t&     element_const_reference_t; //!< Constant reference to \c element_t.
        typedef element_t&&          element_move_t;            //!< Move a \c element_t.

        typedef std::basic_string<charT> string_t; //!< The string type.

        typedef basic_attribute<charT> attribute_t;     //!< The attribute type of this element.
        typedef std::set<attribute_t > attribute_set_t; //!< A set of \c basic_attribute.

        typedef          basic_text<charT>              text_t;                 //!< The text type.
        typedef typename text_t::text_const_reference_t text_const_reference_t; //!< A pointer to \c text_t.
        typedef typename text_t::text_move_t            text_move_t;            //!< A reference to \c text_t.

        template <class classT = child_t>
        using iterator       = typename parent_t::template iterator<classT>;
        template <class classT = child_t>
        using const_iterator = typename parent_t::template const_iterator<classT>;

        //!@}

        //! \brief Constructor
        /*!
         *  This constructor initialise the internals of an element
         *  (i.e. its name)
         *
         *  \param[in] name   The name of this element.
         *  \param[in] parent The parent node of this element.
         */
        basic_element(
            string_t name,
            parent_pointer_t parent = nullptr)
        :
            node_t(parent),
            mName(name)
        {}

        //! \brief Copy constructor.
        /*!
         *  Creates a copy of an XML element.
         *
         *  \param [in] rhs A constant reference to a \c element_t.
         */
        basic_element(element_const_reference_t rhs)
        :
            node_t(rhs),
            mName(rhs.mName)
        {}

        //! \brief Move constructor.
        /*!
         *  Moves the internal of a \c element_t.
         *
         *  \param [in] rhs A rvalue reference to a \c element_t.
         */
        basic_element(element_move_t rhs)
        :
            node_t(rhs),
            mName(std::move(rhs.mName))
        {}

        //! \brief Destructor.
        /*!
         *  This destructor does nothing.
         */
        virtual ~basic_element()
        {}

        //! \brief Get the type of a \c element_t.
        /*!
         *  This function returns the type of a \c element_t.
         *
         *  \return The type of a \c element_t.
         */
        virtual type_t type() const
        {
            return node_interface_t::stringToType("element");
        }

        //! \brief Clone the current \c element_t.
        /*!
         *  This function creates a deep copy of this \c element_t,
         *  and returns a pointer to it. It is used when copying element in
         *  parent node.
         */
        virtual child_pointer_t clone() const
        {
            return new element_t(*this);
        }

        //! \brief Clone the given \c element_t using move syntax.
        /*!
         *  This function creates a deep copy of the given\c element_t,
         *  and returns a pointer to it. It is used when copying element in
         *  parent node.
         */
        virtual child_pointer_t clone(child_move_t rhs) const
        {
            return new element_t(static_cast<element_move_t>(rhs));
        }

        //! \brief Get the attributes of an element.
        /*!
         *  This function returns a constant reference to the attributes of the
         *  \c element_t.
         *
         *  \return A constant reference to the attributes of the \c element_t.
         */
        const attribute_set_t& attributes() const
        {
            return mAttributes;
        }

        //! \brief Get the attributes of an element.
        /*!
         *  This function returns a reference to the attributes of the
         *  \c element_t.
         *
         *  \return A reference to the attributes of the \c element_t.
         */
        attribute_set_t& attributes()
        {
            return mAttributes;
        }

        //! \brief Copy a \c element_t into the inserted elements.
        /*!
         *  The \c element_t will be copied and inserted as children of this
         *  \c basic_parent_node before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be inserted.
         *  \param [in] val      The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> insert (iterator<classT> position, element_const_reference_t val)
        {
            return parent_t::insert(position, val);
        }

        //! \brief Copy a \c element_t \c n times into the inserted elements.
        /*!
         *  The \c element_t will be copied and inserted as children of this
         *  \c basic_parent_node \c n times before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be inserted \c n times.
         *  \param [in] n        The number of time \c val should be inserted.
         *  \param [in] val      The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the first newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> insert (iterator<classT> position, size_t n, element_const_reference_t val)
        {
            return parent_t::insert(position, n, val);
        }

        //! \brief Copy a set of \c element_t into the inserted elements.
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
        template <template<class> class InputIterator, class classT = child_t>
        iterator<classT> insert (iterator<classT> position, InputIterator<element_t> first, InputIterator<element_t> last)
        {
            return parent_t::insert(position, first, last);
        }

        //! \brief Move a \c element_t into the inserted elements.
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
        iterator<classT> insert (iterator<classT> position, element_move_t val)
        {
            return parent_t::insert(position, std::move(val));
        }

        //! \brief Copy a list of \c element_t into the inserted elements.
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
        iterator<classT> insert (iterator<classT> position, std::initializer_list<element_t> il)
        {
            return parent_t::insert(position, il);
        }

        //! \brief Copy a \c element_t and insert it before the first element.
        /*!
         *  The \c element_t will be copied and inserted as children of this
         *  \c basic_parent_node in front of the first element.
         *
         *  \param [in] val The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_front (element_const_reference_t val)
        {
            return parent_t::push_front(val);
        }

        //! \brief Move a \c element_t before the first element.
        /*!
         *  The \c element_t will be moved as children of this
         *  \c basic_parent_node in front of the first element.
         *
         *  \param [in] val The value to be moved.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_front (element_move_t val)
        {
            return parent_t::push_front(std::move(val));
        }

        //! \brief Copy a \c element_t and insert it after the last element.
        /*!
         *  The \c element_t will be copied and inserted as children of this
         *  \c basic_parent_node after the last element.
         *
         *  \param [in] val The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_back (element_const_reference_t val)
        {
            return parent_t::push_back(val);
        }

        //! \brief Move a \c element_t after the last element.
        /*!
         *  The \c element_t will be moved as children of this
         *  \c basic_parent_node after the last element.
         *
         *  \param [in] val The value to be moved.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_back (element_move_t val)
        {
            return parent_t::push_back(std::move(val));
        }

        //! \brief Allocate a \c element_t and insert it into the inserted elements.
        /*!
         *  The \c element_t will be allocated and inserted as children of this
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
        template <typename ... Args, class classT = child_t>
        iterator<classT> emplace_element (iterator<classT> position, Args&& ... args)
        {
            return parent_t::template emplace<element_t>(position, std::forward<Args>(args) ...);
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
        template <typename ... Args, class classT = child_t>
        iterator<classT> emplace_element_front (Args&& ... args)
        {
            return parent_t::template emplace_front<element_t>(std::forward<Args>(args) ...);
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
        template <typename ... Args, class classT = child_t>
        iterator<classT> emplace_element_back (Args&& ... args)
        {
            return parent_t::template emplace_back<element_t>(std::forward<Args>(args) ...);
        }

        //! \brief Copy a \c text_t into the inserted elements.
        /*!
         *  The \c text_t will be copied and inserted as children of this
         *  \c basic_parent_node before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be inserted.
         *  \param [in] val      The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> insert (iterator<classT> position, text_const_reference_t val)
        {
            return parent_t::insert(position, val);
        }

        //! \brief Copy a \c text_t \c n times into the inserted elements.
        /*!
         *  The \c text_t will be copied and inserted as children of this
         *  \c basic_parent_node \c n times before the item pointed by the \c const_iterator.
         *
         *  \param [in] position A \c const_iterator before which \c val should be inserted \c n times.
         *  \param [in] n        The number of time \c val should be inserted.
         *  \param [in] val      The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the first newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> insert (iterator<classT> position, size_t n, text_const_reference_t val)
        {
            return parent_t::insert(position, n, val);
        }

        //! \brief Copy a set of \c text_t into the inserted elements.
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
        template <template<class> class InputIterator, class classT = child_t>
        iterator<classT> insert (iterator<classT> position, InputIterator<text_t> first, InputIterator<text_t> last)
        {
            return parent_t::insert(position, first, last);
        }

        //! \brief Move a \c text_t into the inserted elements.
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
        iterator<classT> insert (iterator<classT> position, text_move_t val)
        {
            return parent_t::insert(position, std::move(val));
        }

        //! \brief Copy a list of \c text_t into the inserted elements.
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
        iterator<classT> insert (iterator<classT> position, std::initializer_list<text_t> il)
        {
            return parent_t::insert(position, il);
        }

        //! \brief Copy a \c text_t and insert it before the first element.
        /*!
         *  The \c text_t will be copied and inserted as children of this
         *  \c basic_parent_node in front of the first element.
         *
         *  \param [in] val The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_front (text_const_reference_t val)
        {
            return parent_t::push_front(val);
        }

        //! \brief Move a \c text_t before the first element.
        /*!
         *  The \c text_t will be moved as children of this
         *  \c basic_parent_node in front of the first element.
         *
         *  \param [in] val The value to be moved.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_front (text_move_t val)
        {
            return parent_t::push_front(std::move(val));
        }

        //! \brief Copy a \c text_t and insert it after the last element.
        /*!
         *  The \c text_t will be copied and inserted as children of this
         *  \c basic_parent_node after the last element.
         *
         *  \param [in] val The value to be inserted. A copy of it will be made.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_back (text_const_reference_t val)
        {
            return parent_t::push_back(val);
        }

        //! \brief Move a \c text_t after the last element.
        /*!
         *  The \c text_t will be moved as children of this
         *  \c basic_parent_node after the last element.
         *
         *  \param [in] val The value to be moved.
         *
         *  \return An \c iterator pointing to the newly inserted element.
         */
        template <class classT = child_t>
        iterator<classT> push_back (text_move_t val)
        {
            return parent_t::push_back(std::move(val));
        }

        //! \brief Allocate a \c text_t and insert it into the inserted elements.
        /*!
         *  The \c text_t will be allocated and inserted as children of this
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
        template <typename ... Args, class classT = child_t>
        iterator<classT> emplace_text (iterator<classT> position, Args&& ... args)
        {
            return parent_t::template emplace<text_t>(position, std::forward<Args>(args) ...);
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
        template <typename ... Args, class classT = child_t>
        iterator<classT> emplace_text_front (Args&& ... args)
        {
            return parent_t::template emplace_front<text_t>(std::forward<Args>(args) ...);
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
        template <typename ... Args, class classT = child_t>
        iterator<classT> emplace_text_back (Args&& ... args)
        {
            return parent_t::template emplace_back<text_t>(std::forward<Args>(args) ...);
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
            return parent_t::erase(position);
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
            return parent_t::erase(first, last);
        }

        //! \brief Delete first element.
        /*!
         *  Removes the first element and destroy it.
         */
        void pop_front ()
        {
            return parent_t::pop_front();
        }

        //! \brief Delete last element.
        /*!
         *  Removes the last element and destroy it.
         */
        void pop_back ()
        {
            return parent_t::pop_back();
        }

        //! \brief Delete all elements.
        /*!
         *  Removes all elements and destroy them.
         */
        void clear () noexcept
        {
            return parent_t::clear();
        }

    private:
        string_t mName;

        attribute_set_t mAttributes;
    };

    typedef basic_element<char>    element;  //!< A specialized \c basic_element for char.
    typedef basic_element<wchar_t> welement; //!< A specialized \c basic_element for wchar_t.
}

#endif /* ELEMENT_H_INCLUDED */
