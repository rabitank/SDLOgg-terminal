
#include "utils/fileTool.h"




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

std::string GetFilename(const std::string &in_path,char in_separator)
{
    using namespace std;
	string::size_type iPos = in_path.find_last_of(in_separator) + 1;
	const string filename = in_path.substr(iPos, in_path.length() - iPos);
    return filename;
}

void GetFileNameASuffix(const std::string &in_path, std::string *g_name, std::string *g_suffix ,char in_separator )
{
    //1.获取不带路径的文件名
    using namespace std;
	string::size_type iPos =in_path.find_last_of(in_separator) + 1;
	string filename = in_path.substr(iPos, in_path.length() - iPos);
    
    if(g_name)
    {
	    *g_name = filename.substr(0, filename.rfind("."));
    }

    if(g_suffix)
    {
	    *g_suffix = filename.substr(filename.find_last_of('.') + 1);
    }    

}

char const *OpenFileDialog(char const *in_Title, char const *in_DefaultPathAndOrFile, int in_NumOfFilterPatterns, char const *const *in_FilterPatterns, char const *in_SingleFilterDescription, int in_AllowMultipleSelects)
{
    return tinyfd_openFileDialog(in_Title,in_DefaultPathAndOrFile,in_NumOfFilterPatterns,in_FilterPatterns,in_SingleFilterDescription,in_AllowMultipleSelects);
}
