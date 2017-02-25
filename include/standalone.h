#ifndef STANDALONE_H_INCLUDED
#define STANDALONE_H_INCLUDED

#include <string>

#include <strings.h>
#include <readable.h>

namespace xml
{
    //! \brief Whether a XML document is standalone
    /*!
     *  It represents a XML document standalone status.
     *  It can be yes or no.
     */
    template <typename charT>
    struct basic_standalone {
    public:
        typedef basic_standalone<charT> standalone_t;
        typedef basic_readable<charT> readable_t; //!< The stream input type.
        typedef std::basic_string<charT> string_t; //!< The string type.

        basic_standalone(
            bool standalone)
        :
            mStandalone(standalone)
        {}


        const bool standalone () const
        {
            return mStandalone;
        }


        static standalone_t* read(readable_t& input, bool throwLastError = false)
        {
            string_t standalone(strings::converter<strings::standalone, charT>::value);
            bool value;

            bool ret = input.read_name_and_quoted_value(
                true,
                &readable_t::match_name,  standalone,
                &readable_t::read_yes_no, value
            );

            if (ret)
                return new standalone_t(value);
    
            if (throwLastError)
                input.throw_last_error();            

            return nullptr;
        }

    private:
        bool mStandalone; //!< The standalone status.
    };
}

#endif /* STANDALONE_H_INCLUDED */