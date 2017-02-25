#ifndef ATTRIBUTE_H_INCLUDED
#define ATTRIBUTE_H_INCLUDED

#include <string>

#include <readable.h>
#include <writeable.h>

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
        typedef std::basic_string<charT> string_t;  //!< The type of string to parse

        typedef basic_attribute<charT> attribute_t; //!< The type of attribute.

        typedef basic_readable<charT> readable_t; //!< The type of readable.

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
        basic_attribute(const attribute_t& rhs)
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
        basic_attribute(attribute_t&& rhs)
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


        //! \brief Read an attribute_t from a readable_t
        /*!
         *  This function will try to parse an attribute from an input stream.
         *  In case of error, it might throw an exception.
         *
         *  \param [in] input The input stream to use to parse the attribute_t.
         *  \param [in] throwLastError Whether an execption should be thrown in
         *              case of error.
         *
         *  \throws A parsing_exception_t, if throwLastError is true. 
         *
         *  \return A newly allocated attribute_t in case of success; a nullptr
         *          in case of error.
         */
        static attribute_t* read(readable_t& input, bool throwLastError = false)
        {
            string_t name;
            string_t value;

            bool ret = input.read_name_and_quoted_value(
                true,
                &readable_t::read_name,            name,
                &readable_t::read_attribute_value, value
            );

            if (ret)
                return new basic_attribute(name, value);
    
            if (throwLastError)
                input.throw_last_error();

            return nullptr;
        }

    private:
        string_t  mName; //!< The name of an attribute.
        string_t mValue; //!< The value of an attribute.
    };

    typedef basic_attribute<char>    attribute;  //!< A specialized \c basic_attribute for char.
    typedef basic_attribute<wchar_t> wattribute; //!< A specialized \c basic_attribute for wchar_t.
}

#endif /* ATTRIBUTE_H_INCLUDED */