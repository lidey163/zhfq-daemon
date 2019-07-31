#ifndef ZHFQ_UPLOAD_FILE_CGI_H
#define ZHFQ_UPLOAD_FILE_CGI_H

#include "stdcgi/Transaction.h"
#include "commonnet/BasicStreamF.h"


#include <string>
#include <map>

class UploadFilesCGI : public stdcgi::Transaction
{
public:
    UploadFilesCGI() { };
    virtual ~UploadFilesCGI() { };

private:
    virtual int Run();
    std::string createFileID();

	bool m_bUpload; // 上传或下载查看
};


#endif
