#include <iostream>

#include <document.h>

void usage (const char* exec_path) {
    std::cout << "Usage :" << std::endl;
    std::cout << "\t" << exec_path << " <xml_filename>" << std::endl;
}

int main (int argc, char** argv) {
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    try {
        xml::wdocument doc(argv[1]);
    } catch (xml::wexception & e) {
        std::wcerr << e << std::endl;
        return e.errCode();
    }

    std::wcout << doc << std::endl;

    return 0;
}
