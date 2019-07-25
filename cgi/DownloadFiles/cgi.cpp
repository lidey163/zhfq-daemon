#include "stdcgi/CGI.h"
#include "DownloadAuthCGI.h"

commoncgi::Transaction* commoncgi::CGI::MakeTransObj()
{
    commoncgi::Transaction* pTransaction = new DownloadFilesCGI();

    m_resData.SetOutPutType(commoncgi::OUTPUT_TYPE_NO);

	return pTransaction;
}
