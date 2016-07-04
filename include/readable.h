#ifndef READABLE_H_INCLUDED
#define READABLE_H_INCLUDED

#include <string>
#include <istream>
#include <list>
#include <tuple>
#include <stack>
#include <algorithm>

namespace xml {
    template <typename charT>
    class basic_readable {
    public:
        typedef charT                      char_t;
        typedef std::basic_string<charT>   string_t;
        typedef std::basic_istream<charT>  istream_t;
        typedef std::list<string_t>        string_list_t;

        typedef basic_readable<charT> readable_t;                 //!< The type of readable.
        typedef readable_t*           readable_pointer_t;         //!< Pointer to \c readable_t.
        typedef readable_t&           readable_reference_t;       //!< Reference to \c readable_t.
        typedef const readable_t&     readable_const_reference_t; //!< Constant reference to \c readable_t.
        typedef readable_t&&          readable_move_t;            //!< Move a \c readable_t.

        basic_readable(istream_t& input)
        :
            mInput(input),
            mCurrentState(0, 0, mInput.tellg())
        {
            if (mInput.fail() || mInput.bad()) {
                throw -1; // TODO : exception
            } else if (mInput.eof() || mInput.good()) {
                // Do nothing
            } else {
                throw -1; // TODO : assert
            }
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


    private:
        typedef std::streamoff line_t;
        typedef size_t         column_t;
        typedef std::streampos index_t;

        typedef std::tuple<line_t, column_t, index_t> state_t;
        typedef std::stack<state_t>                   state_stack_t;

        static istream_t empty_stream;

        istream_t& mInput;

        state_stack_t mSavedStates;
        state_t       mCurrentState;

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
        bool match(const char_t c, char_t& res)
        {
            if (peek() != c) {
                return false;
            }

            res = read();
            return true;
        }

        bool match(const string_t& str, string_t& res)
        {
            char_t c;
            res.clear();
            push();

            for(auto it = str.begin(); it != str.end(); ++it) {
                if (!match(*it, c)) {
                    pop();
                    res.clear();
                    return false;
                } else {
                    res += c;
                }
            }

            drop();
            return true;
        }

        bool match_in(const char_t c1, const char_t c2, char_t& res)
        {
            char_t p = peek();
            if (p < c1 || p > c2) {
                return false;
            }

            res = read();
            return true;
        }

        bool match_in(const string_t& arr, char_t& res)
        {
            char_t p = peek();
            for(auto it = arr.begin(); it != arr.end(); ++it) {
                if (p == *it) {
                    res = read();
                    return true;
                }
            }

            return false;
        }

        bool match_not(const char_t c)
        {
            if (peek() == c) {
                return false;
            }

            return true;
        }

        bool match_not(const string_t& str)
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
            return false;
        }

        bool match_not_in(const char_t c1, const char_t c2, char_t& res)
        {
            char_t p = peek();
            if (p >= c1 && p <= c2) {
                return false;
            }

            res = read();
            return true;
        }

        bool match_not_in(const string_t& arr, char_t& res)
        {
            char_t p = peek();
            for(auto it = arr.begin(); it != arr.end(); ++it) {
                if (p == *it) {
                    return false;
                }
            }

            res = read();
            return true;
        }

        bool read_eof()
        {
            char_t c;
            return match('\0', c);
        }

        bool read_upper_letter(char_t& c)
        {
            return match_in('A', 'Z', c);
        }

        bool read_lower_letter(char_t& c)
        {
            return match_in('a', 'z', c);
        }

        bool read_digit(char_t& c)
        {
            return match_in('0', '9', c);
        }

        bool read_hexa_char(char_t& c)
        {
            return
                match_in('0', '9', c) ||
                match_in('a', 'f', c) ||
                match_in('A', 'F', c);
        }

        bool read_quote(char_t& c)
        {
            return match_in({'\'', '"'}, c);
        }


        bool read_char(char_t& c)
        {
            if (match_in({0x9, 0xA, 0xD}, c))
                return true;

            if (match_in(0x20, 0xFF, c))
                return true;

            if (std::numeric_limits<char_t>::max() <= 0xFF)
                return false;

            if (match_in((char_t)0x100, (char_t)0xD7FF, c))
                return true;

            if (match_in((char_t)0xE000, (char_t)0xFFFD, c))
                return true;

            if (std::numeric_limits<char_t>::max() <= 0xFFFF)
                return false;

            if (match_in((char_t)0x10000, (char_t)0x10FFFF, c))
                return true;

            return false;
        }

        bool read_space(char_t& c)
        {
            return match_in({0x9, 0xA, 0xD, 0x20}, c);
        }

        bool read_name_start_char(char_t& c)
        {
            if (match(':', c))
                return true;

            if (read_upper_letter(c))
                return true;

            if (match('_', c))
                return true;

            if (read_lower_letter(c))
                return true;

            if (match_in(0xC0, 0xD6, c))
                return true;

            if (match_in(0xD8, 0xF6, c))
                return true;

            if (match_in(0xF8, 0xFF, c))
                return true;

            if (std::numeric_limits<char_t>::max() <= 0xFF)
                return false;

            if (match_in((char_t)0x100, (char_t)0x2FF, c))
                return true;

            if (match_in((char_t)0x370, (char_t)0x37D, c))
                return true;

            if (match_in((char_t)0x37F, (char_t)0x1FFF, c))
                return true;

            if (match_in((char_t)0x200C, (char_t)0x200D, c))
                return true;

            if (match_in((char_t)0x2070, (char_t)0x218F, c))
                return true;

            if (match_in((char_t)0x2C00, (char_t)0x2FEF, c))
                return true;

            if (match_in((char_t)0x3001, (char_t)0xD7FF, c))
                return true;

            if (match_in((char_t)0xF900, (char_t)0xFDCF, c))
                return true;

            if (match_in((char_t)0xFDF0, (char_t)0xFFFD, c))
                return true;

            if (std::numeric_limits<char_t>::max() <= 0xFFFF)
                return false;

            if (match_in((char_t)0x10000, (char_t)0xEFFFF, c))
                return true;

            return false;
        }

        bool read_name_char(char_t& c)
        {
            if(read_name_start_char(c))
                return true;

            if(match('-', c))
                return true;

            if(match('.', c))
                return true;

            if(read_digit(c))
                return true;

            if(match(0xB7, c))
                return true;

            if (std::numeric_limits<char_t>::max() <= 0xFF)
                return false;

            if(match_in((char_t)0x300, (char_t)0x36F, c))
                return true;

            if(match_in((char_t)0x203F, (char_t)0x2040, c))
                return true;

            return false;
        }

        bool read_public_id_char(char_t& c)
        {
            return
                match(0x20, c) ||
                match(0xD, c) ||
                match(0xA, c) ||
                read_upper_letter(c) ||
                read_lower_letter(c) ||
                read_digit(c) ||
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
                    }, c);
        }

        bool read_spaces()
        {
            char_t c;
            if (!read_space(c))
                return false;

            while(read_space(c));

            return true;
        }

        bool read_name(string_t& name)
        {
            char_t c;
            name.clear();
            push();

            if (!read_name_start_char(c))
                goto error;
            name += c;

            while(read_name_char(c))
                name += c;

            drop();
            return true;

        error:
            name.clear();
            pop();
            return false;
        }

        bool read_names(string_list_t& names)
        {
            char_t c;
            string_t name;
            names.clear();
            push();

            if (!read_name(name))
                goto error;
            names.push_back(name);

            while (match(0x20, c) && read_name(name))
                names.push_back(name);

            drop();
            return true;

        error:
            names.clear();
            pop();
            return false;
        }

        bool read_token(string_t& token)
        {
            char_t c;
            token.clear();
            push();

            if (!read_name_char(c))
                goto error;
            token += c;

            while(read_name_char(c))
                token += c;

            drop();
            return true;

        error:
            token.clear();
            pop();
            return false;

        }

        bool read_tokens(string_list_t& tokens)
        {
            char_t c;
            string_t token;
            tokens.clear();
            push();

            if (!read_token(token))
                goto error;
            tokens.push_back(token);

            while (match(0x20, c) && read_token(token))
                tokens.push_back(token);

            drop();
            return true;

        error:
            tokens.clear();
            pop();
            return false;
        }

        bool read_character_data(string_t& data)
        {
            char_t c;
            data.clear();

            while (match_not({']', ']', '>'}) && match_not_in({'<', '&'}, c))
                data += c;

            return true;
        }

        bool read_comment(string_t& comment)
        {
            char_t c;
            string_t dummy;
            comment.clear();
            push();

            if(!match({'<', '!', '-', '-'}, dummy))
                goto error;

            while(match_not({'-', '-', '>'}) && read_char(c))
                if (c == '-' && !match_not({'-', '-', '>'}))
                    goto error;
                else
                    comment += c;

            if(!match({'-', '-', '>'}, dummy))
                goto error;

            drop();
            return true;

        error:
            comment.clear();
            pop();
            return false;
        }

        bool read_processing_instructions_content(string_t& content)
        {
            char_t c;
            content.clear();

            while (match_not({'?', '>'}) && read_char(c))
                content += c;

            return true;
        }

        bool read_processing_instructions_target(string_t& target)
        {
            target.clear();
            push();

            if (!read_name(target))
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
            return false;
        }

        bool read_cdata(string_t& cdata)
        {
            char_t c;
            cdata.clear();

            while (match_not({']', ']', '>'}) && read_char(c))
                cdata += c;

            return true;
        }

        bool read_reference(string_t& ref)
        {
            return read_entity_reference(ref) || read_char_reference(ref);
        }

        bool read_entity_reference(string_t& ref)
        {
            char_t c;
            string_t name;
            ref.clear();
            push();

            if (!match('&', c))
                goto error;
            ref += c;

            if (!read_name(name))
                goto error;
            ref += name;

            if (!match(';', c))
                goto error;
            ref += c;

            drop();
            return true;

        error:
            ref.clear();
            pop();
            return false;
        }

        bool read_char_reference(string_t& ref)
        {
            char_t c;
            string_t str;
            ref.clear();
            push();

            if (!match({'&', '#'}, str))
                goto hexa_reference;
            ref += str;

            if (!read_digit(c))
                goto hexa_reference;
            ref += c;

            while(read_digit(c))
                ref += c;

            if (!match(';', c))
                goto hexa_reference;
            ref += c;

            drop();
            return true;

        hexa_reference:
            pop();
            ref.clear();
            push();

            if (!match({'&', '#', 'x'}, str))
                goto error;
            ref += str;

            if (!read_hexa_char(c))
                goto error;
            ref += c;

            while(read_hexa_char(c))
                ref += c;

            if (!match(';', c))
                goto error;
            ref += c;

            drop();
            return true;

        error:
            ref.clear();
            pop();
            return false;
        }

        bool read_parameter_entity_reference(string_t& ref)
        {
            char_t c;
            string_t name;
            ref.clear();
            push();

            if (!match('%', c))
                goto error;
            ref += c;

            if (!read_name(name))
                goto error;
            ref += name;

            if (!match(';', c))
                goto error;
            ref += c;

            drop();
            return true;

        error:
            ref.clear();
            pop();
            return false;
        }

        bool read_entity_value(const char_t quote, string_t& entity)
        {
            char_t c;
            string_t ref;
            entity.clear();

            while (true) {
                if (match_not_in({'%', '&', quote}, c))
                    entity += c;
                else if (read_parameter_entity_reference(ref))
                    entity += ref;
                else if (read_reference(ref))
                    entity += ref;
                else
                    break;
            }

            return true;
        }

        bool read_attribute_value(const char_t quote, string_t& attribute)
        {
            char_t c;
            string_t ref;
            attribute.clear();

            while (true) {
                if (match_not_in({'<', '&', quote}, c))
                    attribute += c;
                else if (read_reference(ref))
                    attribute += ref;
                else
                    break;
            }

            return true;
        }

        bool read_system_literal(const char_t quote, string_t& id)
        {
            id.clear();

            while (match_not(quote))
                id += read();

            return true;
        }

        bool read_public_id_literal(const char_t quote, string_t& id)
        {
            char_t c;
            id.clear();

            while (match_not(quote) && read_public_id_char(c))
                id += c;

            return true;
        }

        template <typename funcT>
        bool read_quoted_value(funcT func, string_t& value)
        {
            char_t quote;
            value.clear();
            push();

            if(!read_quote(quote))
                goto error;

            if(!(*this.*func)(quote, value))
                goto error;

            if(!match(quote, quote))
                goto error;

            drop();
            return true;

        error:
            value.clear();
            pop();
            return false;
        }

        template <typename funcT1, typename funcT2>
        bool read_name_and_quoted_value(funcT1 func1, string_t& name, funcT2 func2, string_t& value)
        {
            char_t c;
            name.clear();
            value.clear();
            push();

            if(!(*this.*func1)(name))
                goto error;

           read_spaces();

            if(!match('=', c))
                goto error;

           read_spaces();

            if(!read_quoted_value(func2, value))
                goto error;

            drop();
            return true;

        error:
            name.clear();
            value.clear();
            pop();
            return false;
        }
    };
}

#endif /* READABLE_H_INCLUDED */