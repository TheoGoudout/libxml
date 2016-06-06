#ifndef ELEMENT_H_INCLUDED
#define ELEMENT_H_INCLUDED

#include <node.h>
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
        typedef          basic_parent_node<charT> parent_t;  //!< The parent type
        typedef typename parent_t::child_t        child_t;   //!< The default child type of an element

        typedef          basic_element<charT>     element_t; //!< The element type
        typedef          basic_text<charT>        text_t;    //!< The text type used in an element

        template <class classT = child_t>
        using iterator       = typename parent_t::template iterator<classT>;
        template <class classT = child_t>
        using const_iterator = typename parent_t::template const_iterator<classT>;

        //!@}

        virtual std::basic_string<charT> type() const;

        //! \brief Insert an element into this element children.
        /*!
         *  This function will insert \c val into this element children,
         *  before the element pointer by position.
         *
         *  \param[in] position An iterator to the element that will be before the on inserted.
         *  \param[in] val      The new element to insert.
         *
         *  \return An \c iterator to the newly inserted child.
         */
        iterator<> insert(const_iterator<> position, const element_t& val);

        //! \brief Move an element into this element children.
        /*!
         *  This function will move \c val into this element children,
         *  before the element pointer by position.
         *
         *  \param[in] position An iterator to the element that will be before the on inserted.
         *  \param[in] val      The new element to insert.
         *
         *  \return An \c iterator to the newly inserted child.
         */
        iterator<> insert(const_iterator<> position, element_t&& val);

        //! \brief Insert an text into this element children.
        /*!
         *  This function will insert \c val into this element children,
         *  before the element pointer by position.
         *
         *  \param[in] position An iterator to the element that will be before the on inserted.
         *  \param[in] val      The new text to insert.
         *
         *  \return An \c iterator to the newly inserted child.
         */
        iterator<> insert(const_iterator<> position, const text_t& val);

        //! \brief Move an text into this element children.
        /*!
         *  This function will move \c val into this element children,
         *  before the element pointer by position.
         *
         *  \param[in] position An iterator to the element that will be before the on inserted.
         *  \param[in] val      The new text to insert.
         *
         *  \return An \c iterator to the newly inserted child.
         */
        iterator<> insert(const_iterator<> position, text_t&& val);
    };

    typedef basic_element<char>    element;  //!< A specialized \c basic_element for char.
    typedef basic_element<wchar_t> welement; //!< A specialized \c basic_element for wchar_t.
}

#endif /* ELEMENT_H_INCLUDED */
