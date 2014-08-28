// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "options.hpp"
#include "xpiler.hpp"

using namespace x2boost;

int main(int argc, char* argv[])
{
    try
    {
        int index = Xpiler::options.Parse(argc, argv);

        std::cout << Xpiler::options.input << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Unhandled Exception: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}

// EOF main.cpp
