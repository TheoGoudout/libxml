#ifndef ATTRIBUTE_H_INCLUDED
#define ATTRIBUTE_H_INCLUDED

#include <string>
#include <istream>
#include <sstream>

namespace xml {

    //! \brief A XML attribute.
    /*!
     *  This class represents an XML attribute that has a name and a value.
     *
     *  \tparam charT The type of character used in the name and value.
     *                By default, char and wchar_t are supported.
     */
    template <typename charT>
    class basic_attribute {
    public:
        //! \name Member types
        //!@{
        typedef std::basic_string<charT>  string_t;  //!< The type of string to parse

        typedef basic_attribute<charT> attribute_t;                 //!< The type of attribute.
        typedef attribute_t*           attribute_pointer_t;         //!< Pointer to \c attribute_t.
        typedef attribute_t&           attribute_reference_t;       //!< Reference to \c attribute_t.
        typedef const attribute_t&     attribute_const_reference_t; //!< Constant reference to \c attribute_t.
        typedef attribute_t&&          attribute_move_t;            //!< Move a \c attribute_t.

        //!@}

        //! \brief Constructor.
        /*!
         *  This constructor build a new attribute with a name and a value.
         *
         *  \param [in] name  The name of the attribute.
         *  \param [in] value The value of the attribute.
         */
        basic_attribute(
            const string_t& name,
            const string_t& value)
        :
            mName(name),
            mValue(value)
        {}

        //! \brief Copy constructor.
        /*!
         *  This constructor creates a copy of an existing attribute.
         *
         *  \param [in] rhs A constant reference to a \c attribute_t.
         */
        basic_attribute(attribute_const_reference_t rhs)
        :
            mName(rhs.mName),
            mValue(rhs.mValue)
        {}

        //! \brief Move constructor.
        /*!
         *  This constructor moves the internals of an attribute.
         *
         *  \param [in] rhs A rvalue reference to a \c attribute_t.
         */
        basic_attribute(attribute_move_t rhs)
        :
            mName(std::move(rhs.mName)),
            mValue(std::move(rhs.mValue))
        {}

        //! \brief Destructor.
        /*!
         *  This destructor does nothing.
         */
        virtual ~basic_attribute()
        {}

        //! \brief Get the name of an attribute.
        /*!
         *  This function returns a constant reference to the name of the
         *  \c basic_attribute.
         *
         *  \return A constant reference to the name of the \c basic_attribute.
         */
        const string_t& name() const
        {
            return mName;
        }

        //! \brief Get the name of an attribute.
        /*!
         *  This function returns a reference to the name of the
         *  \c basic_attribute.
         *
         *  \return A reference to the name of the \c basic_attribute.
         */
        string_t& name()
        {
            return mName;
        }

        //! \brief Get the value of an attribute.
        /*!
         *  This function returns a constant reference to the value of the
         *  \c basic_attribute.
         *
         *  \return A constant reference to the value of the \c basic_attribute.
         */
        const string_t& value() const
        {
            return mValue;
        }

        //! \brief Get the value of an attribute.
        /*!
         *  This function returns a reference to the value of the
         *  \c basic_attribute.
         *
         *  \return A reference to the value of the \c basic_attribute.
         */
        string_t& value()
        {
            return mValue;
        }

    private:
        string_t  mName; //!< The name of an attribute.
        string_t mValue; //!< The value of an attribute.
    };

    typedef basic_attribute<char>    attribute;  //!< A specialized \c basic_attribute for char.
    typedef basic_attribute<wchar_t> wattribute; //!< A specialized \c basic_attribute for wchar_t.
}

#endif /* ATTRIBUTE_H_INCLUDED */