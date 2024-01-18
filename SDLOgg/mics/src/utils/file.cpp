#include "utils/file.h"

bool IsFileExist(const std::string &in_path)
{
    FILE *  fh  =  fopen( in_path.c_str() , " r " );
    fclose(fh);
    if (fh  ==  NULL)
    {
        return false;
    }
    return true;
}