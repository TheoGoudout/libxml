#include <iostream>

#include <document.h>

void usage (const char* exec_path) {
    std::cout << "Usage :" << std::endl;
    std::cout << "\t" << exec_path << " <xml_filename>" << std::endl;
}

int main (int argc, char** argv) {
    
    xml::element elem("root", nullptr);
    xml::document doc(std::move(elem));

    std::cout << doc << std::endl;

    return 0;
}
