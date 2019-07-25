#include "stdcgi/CGI.h"
#include "upload_files.h"

commoncgi::Transaction* commoncgi::CGI::MakeTransObj()
{
    commoncgi::Transaction* pTransaction = new UploadFilesCGI();

    m_resData.SetOutPutType(commoncgi::OUTPUT_TYPE_XML);

    return pTransaction;
}
