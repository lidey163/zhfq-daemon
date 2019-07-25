#ifndef MPOS_DOWNLOAD_FILE_CGI_H
#define MPOS_DOWNLOAD_FILE_CGI_H


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



#endif //MPOS_DOWNLOAD_FILE_CGI_H
