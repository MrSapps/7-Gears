#pragma once

#include "kernel/filesystem.hpp"

class Kernel
{
public:
    Kernel();
private:

    FileSystem mFileSystem;
};
