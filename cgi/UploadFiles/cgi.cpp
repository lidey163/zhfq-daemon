#include "stdcgi/CGI.h"
#include "upload_files.hpp"

stdcgi::Transaction* stdcgi::CGI::MakeTransObj()
{
    stdcgi::Transaction* pTransaction = new UploadFilesCGI();

    m_resData.SetOutPutType(stdcgi::OUTPUT_TYPE_XML);

    return pTransaction;
}
