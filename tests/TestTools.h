#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string>
#include <sstream>

#define CPPUNIT_ASSERT_STRING_EQUAL(str1, str2)  \
{                                                \
    if (!equals(str1, str2))                     \
    {                                            \
        std::stringstream sstr;                  \
        sstr << "Expected : \"" << str1 << "\" " \
             << "Actual : \"" << str2 << "\"";   \
        CPPUNIT_FAIL(sstr.str());                \
    }                                            \
}

template <typename charT>
std::basic_string<charT> convert(std::basic_string<char> input)
{
    return std::basic_string<charT>(input.begin(), input.end());
}

template <typename charT>
std::basic_stringstream<charT> create_input_stream(std::basic_string<char> input)
{
    return std::basic_stringstream<charT>(convert<charT>(input));
}


template <typename charT>
bool equals(const charT *s1, const charT *s2)
{
    size_t i = 0;
    for (; s1[i] != '\0'; ++i)
    {
        if (s1[i] != s2[i])
            return false;
    }

    return s2[i] == '\0';
}

#endif /* UTILS_H_INCLUDED */