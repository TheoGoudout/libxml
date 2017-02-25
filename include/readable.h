#ifndef READABLE_H_INCLUDED
#define READABLE_H_INCLUDED

#include <string>
#include <istream>
#include <list>
#include <tuple>
#include <stack>
#include <algorithm>

#include <exception.h>

namespace xml {
    template <typename charT>
    class basic_readable {
    public:
        typedef std::streamoff line_t;
        typedef size_t         column_t;
        typedef std::streampos index_t;

        typedef charT                      char_t;
        typedef std::basic_string<charT>   string_t;
        typedef std::basic_istream<charT>  istream_t;
        typedef std::list<string_t>        string_list_t;

        typedef basic_readable<charT> readable_t; //!< The type of readable.

        basic_readable(istream_t& input)
        :
            mInput(input),
            mCurrentState(0, 0, mInput.tellg()),
            mLastError(parsing_exception::create_parsing_exception("Unknown error"))
        {
            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof() || mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }
        }

        ~basic_readable()
        {
            if (nullptr != mLastError)
                delete mLastError;
        }

        void push()
        {
            mSavedStates.emplace(mCurrentState);
        }

        void pop()
        {
            mCurrentState = mSavedStates.top();
            seek(std::get<2>(mCurrentState));
            drop();
        }

        void drop()
        {
            mSavedStates.pop();
        }

        line_t get_current_line()
        {
            return std::get<0>(mCurrentState) + 1;
        }

        column_t get_current_col()
        {
            return std::get<1>(mCurrentState) + 1;
        }

        const parsing_exception& get_last_error()
        {
            return *mLastError;
        }

        void throw_last_error()
        {
            throw mLastError;
        }

        void set_error(parsing_exception* error)
        {
            if (nullptr != mLastError)
                delete mLastError;

            mLastError = error;
        }

    private:
        typedef std::tuple<line_t, column_t, index_t> state_t;
        typedef std::stack<state_t>                   state_stack_t;

        static istream_t empty_stream;

        istream_t& mInput;

        state_stack_t mSavedStates;
        state_t       mCurrentState;

        parsing_exception* mLastError;

        index_t tell()
        {
            index_t res = mInput.tellg();

            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof() || mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }

            return res;
        }

        void seek(index_t i)
        {
            mInput.seekg(i);

            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof() || mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }
        }

        char_t read()
        {
            // Read character
            char_t res = mInput.get();

            // Check for stream error
            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof()) {
                return istream_t::traits_type::eof();
            } else if (mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }

            // Update current state
            if (res == '\r' && peek() == '\n') { // Handle CRLF
                std::get<1>(mCurrentState)++;
                return read();
            } else if (res == '\n') {            // Update line & column indexes
                std::get<0>(mCurrentState)++;
                std::get<1>(mCurrentState) = 0;
            } else {                             // Update column index
                std::get<1>(mCurrentState)++;
            }

            // Update stream index
            std::get<2>(mCurrentState) = tell();

            return res;
        }

        char_t peek()
        {
            char_t c = mInput.peek();

            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof()) {
                return istream_t::traits_type::eof();
            } else if (mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }

            return c;
        }

    public:
        bool match(const char_t c, char_t& res, bool saveError = true)
        {
            if (peek() != c) {
                if (saveError)
                    set_error(
                        parsing_exception::create_parsing_exception(
                            "Expected '%c' character (at line %d col %d)",
                            c,
                            get_current_line(),
                            get_current_col()
                        )
                    );

                return false;
            }

            res = read();
            return true;
        }

        bool match(const char_t c, bool saveError = true)
        {
            char_t res;
            return match(c, res, saveError);
        }

        bool match(const string_t& str, string_t& res, bool saveError = true)
        {
            char_t c;
            res.clear();
            push();

            for(auto it = str.begin(); it != str.end(); ++it) {
                if (!match(*it, c, false)) {
                    pop();
                    if (saveError)
                        set_error(
                            parsing_exception::create_parsing_exception(
                                "Expected '%s' string (at line %d col %d)",
                                string_t(str.begin(), str.end()),
                                get_current_line(),
                                get_current_col()
                            )
                        );

                    return false;
                } else {
                    res += c;
                }
            }

            drop();
            return true;
        }

        bool match(const string_t& str, bool saveError = true)
        {
            string_t res;
            return match(str, res, saveError);
        }

        bool match_name(string_t& name, bool saveError = true)
        {
            if (!match(name, false))
                goto error;

            return true;

        error:
            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected name to be '%s' (at line %d col %d)",
                        std::string(name.begin(), name.end()).c_str(),
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }


        bool match_in(const char_t c1, const char_t c2, char_t& res, bool saveError = true)
        {
            char_t p = peek();
            if (p >= c1 && p <= c2) {
                res = read();
                return true;
            }

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected char between '%c' and '%c' (at line %d col %d)",
                        c1,
                        c2,
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool match_in(const string_t& arr, char_t& res, bool saveError = true)
        {
            char_t p = peek();
            for(auto it = arr.begin(); it != arr.end(); ++it) {
                if (p == *it) {
                    res = read();
                    return true;
                }
            }

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected char in '%s' (at line %d col %d)",
                        string_t(arr.begin(), arr.end()),
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool match_not(const char_t c, bool saveError = true)
        {
            if (peek() != c)
                return true;

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected char not '%c' (at line %d col %d)",
                        c,
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool match_not(const string_t& str, bool saveError = true)
        {
            char_t c;
            push();

            for(auto it = str.begin(); it != str.end(); ++it) {
                if (!match(*it, c)) {
                    pop();
                    return true;
                }
            }

            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected string not '%s' (at line %d col %d)",
                        string_t(str.begin(), str.end()),
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool match_not_in(const char_t c1, const char_t c2, char_t& res, bool saveError = true)
        {
            char_t p = peek();
            if (p < c1 || p > c2) {
                res = read();
                return true;
            }

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected char not between '%c' and '%c' (at line %d col %d)",
                        c1,
                        c2,
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool match_not_in(const string_t& arr, char_t& res, bool saveError = true)
        {
            char_t p = peek();
            for(auto it = arr.begin(); it != arr.end(); ++it) {
                if (p == *it) {
                    if (saveError)
                        set_error(
                            parsing_exception::create_parsing_exception(
                                "Expected char not in '%s' (at line %d col %d)",
                                string_t(arr.begin(), arr.end()),
                                get_current_line(),
                                get_current_col()
                            )
                        );

                    return false;
                }
            }

            res = read();
            return true;
        }

        bool read_eof(bool saveError = true)
        {
            char_t c;
            if (match('\0', c, false))
                return true;

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected EOF (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_upper_letter(char_t& c, bool saveError = true)
        {
            if (match_in('A', 'Z', c, false))
                return true;

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected upper case letter (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_lower_letter(char_t& c, bool saveError = true)
        {
            if (match_in('a', 'z', c, false))
                return true;

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected lower case letter (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_digit(char_t& c, bool saveError = true)
        {
            if (match_in('0', '9', c, false))
                return true;

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected digit (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_hexa_char(char_t& c, bool saveError = true)
        {
            bool ret =
                match_in('0', '9', c, false) ||
                match_in('a', 'f', c, false) ||
                match_in('A', 'F', c, false);

            if (ret)
                return true;

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected hexa char (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_quote(char_t& c, bool saveError = true)
        {
            if (match_in({'\'', '"'}, c, false))
                return true;

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected quote (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }


        bool read_char(char_t& c, bool saveError = true)
        {
            if (match_in({0x9, 0xA, 0xD}, c, false))
                return true;

            if (match_in(0x20, 0xFF, c, false))
                return true;

            if (std::numeric_limits<char_t>::max() <= 0xFF)
                goto error;

            if (match_in((char_t)0x100, (char_t)0xD7FF, c, false))
                return true;

            if (match_in((char_t)0xE000, (char_t)0xFFFD, c, false))
                return true;

            if (std::numeric_limits<char_t>::max() <= 0xFFFF)
                goto error;

            if (match_in((char_t)0x10000, (char_t)0x10FFFF, c, false))
                return true;

        error:
            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML char (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_space(char_t& c, bool saveError = true)
        {
            if (match_in({0x9, 0xA, 0xD, 0x20}, c, false))
                return true;

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected space (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_name_start_char(char_t& c, bool saveError = true)
        {
            if (match(':', c, false))
                return true;

            if (read_upper_letter(c, false))
                return true;

            if (match('_', c, false))
                return true;

            if (read_lower_letter(c, false))
                return true;

            if (match_in(0xC0, 0xD6, c, false))
                return true;

            if (match_in(0xD8, 0xF6, c, false))
                return true;

            if (match_in(0xF8, 0xFF, c, false))
                return true;

            if (std::numeric_limits<char_t>::max() <= 0xFF)
                goto error;

            if (match_in((char_t)0x100, (char_t)0x2FF, c, false))
                return true;

            if (match_in((char_t)0x370, (char_t)0x37D, c, false))
                return true;

            if (match_in((char_t)0x37F, (char_t)0x1FFF, c, false))
                return true;

            if (match_in((char_t)0x200C, (char_t)0x200D, c, false))
                return true;

            if (match_in((char_t)0x2070, (char_t)0x218F, c, false))
                return true;

            if (match_in((char_t)0x2C00, (char_t)0x2FEF, c, false))
                return true;

            if (match_in((char_t)0x3001, (char_t)0xD7FF, c, false))
                return true;

            if (match_in((char_t)0xF900, (char_t)0xFDCF, c, false))
                return true;

            if (match_in((char_t)0xFDF0, (char_t)0xFFFD, c, false))
                return true;

            if (std::numeric_limits<char_t>::max() <= 0xFFFF)
                goto error;

            if (match_in((char_t)0x10000, (char_t)0xEFFFF, c, false))
                return true;

        error:
            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML name start char (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_name_char(char_t& c, bool saveError = true)
        {
            if(read_name_start_char(c, false))
                return true;

            if(match('-', c, false))
                return true;

            if(match('.', c, false))
                return true;

            if(read_digit(c, false))
                return true;

            if(match(0xB7, c, false))
                return true;

            if (std::numeric_limits<char_t>::max() <= 0xFF)
                goto error;

            if(match_in((char_t)0x300, (char_t)0x36F, c, false))
                return true;

            if(match_in((char_t)0x203F, (char_t)0x2040, c, false))
                return true;

        error:
            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML name char (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_public_id_char(char_t& c, bool saveError = true)
        {
            bool ret = 
                match(0x20, c, false) ||
                match(0xD, c, false) ||
                match(0xA, c, false) ||
                read_upper_letter(c, false) ||
                read_lower_letter(c, false) ||
                read_digit(c, false) ||
                match_in(
                    {
                        '-',
                        '\'',
                        '(',
                        ')',
                        '+',
                        ',',
                        '.',
                        '/',
                        ':',
                        '=',
                        '?',
                        ';',
                        '!',
                        '*',
                        '#',
                        '@',
                        '$',
                        '_',
                        '%',
                        '"'
                    }, c, false);

            if (ret)
                return true;

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML public ID char (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_spaces(bool saveError = true)
        {
            char_t c;
            if (!read_space(c, false))
            {
                if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected spaces (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

                return false;
            }

            while(read_space(c, false));

            return true;
        }

        bool read_eq(bool saveError = true)
        {
            char_t c;

            push();

            read_spaces(false);

            if (!match('=', c, false))
                goto error;

            read_spaces(false);

            drop();
            return true;

        error:
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected '=' symbol (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );
            return false;
        }

        bool read_number(uint8_t& number, bool saveError = true)
        {
            char_t c;
            number = 0;

            push();

            if (!read_digit(c, false))
                goto error;
            number = c - '0';

            while(read_digit(c, false))
                number = (number * 10) + (c - '0');

            drop();
            return true;

        error:
            number = 0;
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected number (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_version(const char quote, uint8_t& major, uint8_t& minor, bool saveError = true)
        {
            push();

            if (!read_number(major, false))
                goto error;

            if (!match('.', false))
                goto error;

            if (!read_number(minor, false))
                goto error;

            drop();
            return true;

        error:
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected version (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );
            return false;
        }

        bool read_encoding(const char_t quote, string_t& str, bool saveError = true)
        {
            char_t c;
            str.clear();
            push();

            if (!read_lower_letter(c, false) && !read_upper_letter(c, false))
                goto error;
            str += c;

            while (
                read_lower_letter(c, false) || 
                read_upper_letter(c, false) ||
                read_digit(c, false) ||
                match('.', c, false) ||
                match('_', c, false) ||
                match('-', c, false))
                str += c;

            drop();
            return true;

        error:
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML encoding name (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_yes_no(const char_t quote, bool& yesOrNo, bool saveError = true)
        {
            if (match(string_t({'y', 'e', 's'}), false))
            {
                yesOrNo = true;
                return true;
            }

            if (match(string_t({'n', 'o'}), false))
            {
                yesOrNo = false;
                return true;
            }

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected 'yes' or 'no' value (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_name(string_t& name, bool saveError = true)
        {
            char_t c;
            name.clear();
            push();

            if (!read_name_start_char(c, false))
                goto error;
            name += c;

            while(read_name_char(c, false))
                name += c;

            drop();
            return true;

        error:
            name.clear();
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML name (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_names(string_list_t& names, bool saveError = true)
        {
            char_t c;
            string_t name;
            names.clear();
            push();

            if (!read_name(name, false))
                goto error;
            names.push_back(name);

            while (match(0x20, c, false) && read_name(name, false))
                names.push_back(name);

            drop();
            return true;

        error:
            names.clear();
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML names (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_token(string_t& token, bool saveError = true)
        {
            char_t c;
            token.clear();
            push();

            if (!read_name_char(c, false))
                goto error;
            token += c;

            while(read_name_char(c, false))
                token += c;

            drop();
            return true;

        error:
            token.clear();
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML token (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;

        }

        bool read_tokens(string_list_t& tokens, bool saveError = true)
        {
            char_t c;
            string_t token;
            tokens.clear();
            push();

            if (!read_token(token, false))
                goto error;
            tokens.push_back(token);

            while (match(0x20, c, false) && read_token(token, false))
                tokens.push_back(token);

            drop();
            return true;

        error:
            tokens.clear();
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML tokens (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_character_data(string_t& data, bool saveError = true)
        {
            char_t c;
            data.clear();

            while (match_not({']', ']', '>'}, false) && match_not_in({'<', '&'}, c), false)
                data += c;

            return true;
        }

        bool read_comment(string_t& comment, bool saveError = true)
        {
            char_t c;
            comment.clear();
            push();

            if(!match({'<', '!', '-', '-'}, false))
                goto error;

            while(match_not({'-', '-', '>'}, false) && read_char(c, false))
                if (c == '-' && !match_not({'-', '-', '>'}, false))
                    goto error;
                else
                    comment += c;

            if(!match({'-', '-', '>'}, false))
                goto error;

            drop();
            return true;

        error:
            comment.clear();
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML comment (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_processing_instructions_content(string_t& content, bool saveError = true)
        {
            char_t c;
            content.clear();

            while (match_not({'?', '>'}, false) && read_char(c, false))
                content += c;

            return true;
        }

        bool read_processing_instructions_target(string_t& target, bool saveError = true)
        {
            target.clear();
            push();

            if (!read_name(target, false))
                return false;

            string_t xml({'x', 'm', 'l'});

            // Lambda that checks if two character are the same when ignoring case.
            auto equalsIgnoreCase =
                [](char_t ch1, char_t ch2) {
                    return std::toupper(ch1) == std::toupper(ch2);
                };

            // Search for the XML string in the target name.
            const bool validTargetName = std::search(
                target.begin(),
                target.end(),
                xml.begin(),
                xml.end(),
                equalsIgnoreCase) == target.end();

            if (!validTargetName)
                goto error;

            drop();
            return true;

        error:
            target.clear();
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML processing instructions target (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_cdata(string_t& cdata, bool saveError = true)
        {
            char_t c;
            cdata.clear();

            while (match_not({']', ']', '>'}, false) && read_char(c, false))
                cdata += c;

            return true;
        }

        bool read_reference(string_t& ref, bool saveError = true)
        {
            return read_entity_reference(ref, saveError) || read_char_reference(ref, saveError);
        }

        bool read_entity_reference(string_t& ref, bool saveError = true)
        {
            char_t c;
            string_t name;
            ref.clear();
            push();

            if (!match('&', c, false))
                goto error;
            ref += c;

            if (!read_name(name, false))
                goto error;
            ref += name;

            if (!match(';', c, false))
                goto error;
            ref += c;

            drop();
            return true;

        error:
            ref.clear();
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML entity reference (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_char_reference(string_t& ref, bool saveError = true)
        {
            char_t c;
            string_t str;
            ref.clear();
            push();

            if (!match({'&', '#'}, str, false))
                goto hexa_reference;
            ref += str;

            if (!read_digit(c, false))
                goto hexa_reference;
            ref += c;

            while(read_digit(c, false))
                ref += c;

            if (!match(';', c, false))
                goto hexa_reference;
            ref += c;

            drop();
            return true;

        hexa_reference:
            pop();
            ref.clear();
            push();

            if (!match({'&', '#', 'x'}, str, false))
                goto error;
            ref += str;

            if (!read_hexa_char(c, false))
                goto error;
            ref += c;

            while(read_hexa_char(c, false))
                ref += c;

            if (!match(';', c, false))
                goto error;
            ref += c;

            drop();
            return true;

        error:
            ref.clear();
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML char reference (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_parameter_entity_reference(string_t& ref, bool saveError = true)
        {
            char_t c;
            string_t name;
            ref.clear();
            push();

            if (!match('%', c, false))
                goto error;
            ref += c;

            if (!read_name(name, false))
                goto error;
            ref += name;

            if (!match(';', c, false))
                goto error;
            ref += c;

            drop();
            return true;

        error:
            ref.clear();
            pop();

            if (saveError)
                set_error(
                    parsing_exception::create_parsing_exception(
                        "Expected valid XML entity reference (at line %d col %d)",
                        get_current_line(),
                        get_current_col()
                    )
                );

            return false;
        }

        bool read_entity_value(const char_t quote, string_t& entity, bool saveError = true)
        {
            char_t c;
            string_t ref;
            entity.clear();

            while (true) {
                if (match_not_in({'%', '&', quote}, c, false))
                    entity += c;
                else if (read_parameter_entity_reference(ref, false))
                    entity += ref;
                else if (read_reference(ref, false))
                    entity += ref;
                else
                    break;
            }

            return true;
        }

        bool read_attribute_value(const char_t quote, string_t& attribute, bool saveError = true)
        {
            char_t c;
            string_t ref;
            attribute.clear();

            while (true) {
                if (match_not_in({'<', '&', quote}, c, false))
                    attribute += c;
                else if (read_reference(ref, false))
                    attribute += ref;
                else
                    break;
            }

            return true;
        }

        bool read_system_literal(const char_t quote, string_t& id, bool saveError = true)
        {
            id.clear();

            while (match_not(quote, false))
                id += read();

            return true;
        }

        bool read_public_id_literal(const char_t quote, string_t& id, bool saveError = true)
        {
            char_t c;
            id.clear();

            while (match_not(quote, false) && read_public_id_char(c, false))
                id += c;

            return true;
        }

        template <typename funcT, typename ... retT>
        bool read_quoted_value(bool saveError, funcT func, retT& ... values)
        {
            char_t quote;
            push();

            if(!read_quote(quote, saveError))
                goto error;

            if(!(*this.*func)(quote, values ..., saveError))
                goto error;

            if(!match(quote, quote, saveError))
                goto error;

            drop();
            return true;

        error:
            pop();
            return false;
        }

        template <typename funcT1, typename funcT2, typename ... retT>
        bool read_name_and_quoted_value(bool saveError, funcT1 func1, string_t& name, funcT2 func2, retT& ... values)
        {
            char_t c;
            push();

            if(!(*this.*func1)(name, saveError))
                goto error;

            if (!read_eq(saveError))
                goto error;

            if(!read_quoted_value<funcT2, retT ...>(saveError, func2, values ...))
                goto error;

            drop();
            return true;

        error:
            name.clear();
            pop();
            return false;
        }
    };
}

#endif /* READABLE_H_INCLUDED */