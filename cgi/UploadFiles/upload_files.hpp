#ifndef UPLOAD_FILE_CGI_H
#define UPLOAD_FILE_CGI_H

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
};


#endif
