#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED

#include <cstdarg>
#include <exception>

namespace xml {
    class exception : public std::exception
    {
    public:
        exception() throw() {}
        virtual ~exception() throw() {}

        virtual const char* what() const throw() = 0;
    };

    class parsing_exception : public exception
    {
    public:
        virtual ~parsing_exception() throw()
        {
            if (nullptr != mMessage)
                delete[] mMessage;
        }

        virtual const char* what() const throw()
        {
            return mMessage;
        }

        static parsing_exception* create_parsing_exception(const char* fmt, ...)
        {
            int size = 0;
            char *p = NULL;
            va_list ap;

            /* Determine required size */

            va_start(ap, fmt);
            size = vsnprintf(p, size, fmt, ap);
            va_end(ap);

            if (size < 0)
                return NULL;

            size++;             /* For '\0' */
            p = new char[size];
            if (p == NULL)
               return NULL;

            va_start(ap, fmt);
            size = vsnprintf(p, size, fmt, ap);
            if (size < 0) {
                free(p);
                return NULL;
            }
            va_end(ap);

            return new parsing_exception(p);
        }

    private:
        parsing_exception(const char* message) throw()
        :
            mMessage(message)
        {}

        const char* mMessage;
    };
}

#endif /* EXCEPTION_H_INCLUDED */