// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "options.hpp"
#include "xpiler.hpp"

using namespace x2boost;

int main(int argc, char* argv[])
{
	int index = Xpiler::options.Parse(argc, argv);

    return 0;
}

// EOF main.cpp
