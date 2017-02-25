#ifndef CONVERT_H_INCLUDED
#define CONVERT_H_INCLUDED

#include <cstring>

namespace xml {
    namespace strings {
        template <const char* str, typename charT, int len, char ... Args>
        struct inner : inner<str, charT, len - 1, str[len - 1], Args ...> {};

        template <const char* str, typename charT, char ... Args>
        struct inner<str, charT, 0, Args ...>
        {
            static const charT value[];
        };

        template <const char* str, typename charT, char ... Args>
        constexpr const charT inner<str, charT, 0, Args ...>::value[] = {(charT)Args ..., 0};


        template <const char* str, typename charT>
        struct converter : inner<str, charT, strlen(str)> {};


        static constexpr const char xml_decl_start[] = "<?xml";
        static constexpr const char xml_decl_end[] = "?>";

        static constexpr const char version[] = "version";
        static constexpr const char encoding[] = "encoding";
        static constexpr const char standalone[] = "standalone";
    }
}

#endif /* CONVERT_H_INCLUDED */