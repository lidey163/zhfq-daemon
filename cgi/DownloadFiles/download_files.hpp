#ifndef ZHFQ_DOWNLOAD_FILE_CGI_H
#define ZHFQ_DOWNLOAD_FILE_CGI_H


#include "stdcgi/Transaction.h"
#include <string>

class DownloadFileCGI : public commoncgi::Transaction
{
public:
    DownloadFileCGI(){};
    virtual ~DownloadFileCGI(){};

protected:
    virtual int Run();

private:
};



#endif //ZHFQ_DOWNLOAD_FILE_CGI_H
