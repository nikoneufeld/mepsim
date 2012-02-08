//
//  main.cpp
//  mepsim
//
//  Created by Niko Neufeld on 2/8/12.
//  Copyright (c) 2012 CERN. All rights reserved.
//

#include <iostream>

int read_options(const char *filename) {
    return 0;
}

int main (int argc, const char *argv[])
{
    read_options(argv[1]);
    create_instances();
    run();
    return 0;
}

