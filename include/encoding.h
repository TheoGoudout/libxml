#ifndef ENCODING_H_INCLUDED
#define ENCODING_H_INCLUDED

#include <string>

#include <strings.h>
#include <readable.h>

namespace xml
{
    //! \brief The encoding of a XML document
    /*!
     *  It stores the encoding name of a XML document.
     */
    template <typename charT>
    class basic_encoding {
    public:
        typedef basic_encoding<charT> encoding_t;
        typedef basic_readable<charT> readable_t; //!< The stream input type.
        typedef std::basic_string<charT> string_t; //!< The string type.

        basic_encoding(
            string_t encoding)
        :
            mEncoding(encoding)
        {}


        const string_t& encoding () const
        {
            return mEncoding;
        }


        static encoding_t* read(readable_t& input, bool throwLastError = false)
        {
            string_t encoding(strings::converter<strings::encoding, charT>::value);
            string_t value;

            bool ret = input.read_name_and_quoted_value(
                true,
                &readable_t::match_name,    encoding,
                &readable_t::read_encoding, value
            );

            if (ret)
                return new encoding_t(value);
    
            if (throwLastError)
                input.throw_last_error();            

            return nullptr;
        }

    private:
        string_t mEncoding; //!< The encoding string.
    };
}

#endif /* ENCODING_H_INCLUDED */