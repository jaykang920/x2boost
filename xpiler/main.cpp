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

        std::cout << "forced=" << Xpiler::options.forced << std::endl;
        std::cout << "outDir=" << Xpiler::options.outDir << std::endl;
        std::cout << "recursive=" << Xpiler::options.recursive << std::endl;
        std::cout << "spec=" << Xpiler::options.spec << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Unhandled Exception: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}

// EOF main.cpp
