#include "commonutil/api_logger.h"
#include "commonutil/api_mysql.h"
#include "commonutil/Properties.h"
#include "commonutil/service.h"
#include "commonutil/GlobalUtil.h"
// #include <commonutil/CharConvertUtil.h>

#include "download_files.hpp"
#include "jsonutil.hpp"
#include "errorutil.hpp"

#include <errno.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>

struct GlobalData
{
	commonutil::PropertiesPtr ptrProperties;
	mysql::ConnectionPool* mysqlPool;
	PoolDeleter<mysql::Connection> mysqlDelter;

	std::string uploadPath;
	json outJson;
};

GlobalData g_App;

static
EINT Init(std::string& errstr)
{
	// HttpUtil::InitHttpEnvironment();

	commonutil::PropertiesPtr ptrProperties = new commonutil::Properties();
	ptrProperties->load(commonutil::Properties::getPropertyPath());
	g_App.ptrProperties = ptrProperties;

	init_log(ptrProperties->getProperty("LogService.IP"), ptrProperties->getPropertyAsInt("LogService.Port"));

	mysql::conn_info_t info;
	info.host = ptrProperties->getProperty("MysqlServer.IP");
	info.port = ptrProperties->getPropertyAsInt("MysqlServer.Port");
	info.user = ptrProperties->getProperty("MysqlServer.User");
	info.pwd = ptrProperties->getProperty("MysqlServer.Password").c_str();
	info.db = ptrProperties->getProperty("MysqlServer.Database");
	info.charset = ptrProperties->getProperty("MysqlServer.CharsetName");
	info.client_ip = ptrProperties->getProperty("MysqlServer.ClientIP");

	g_App.mysqlPool = new mysql::ConnectionPool(info, -1);
	g_App.mysqlPool->init();
	g_App.mysqlDelter.setPool(g_App.mysqlPool);

	//获取上传目录
	std::string upload_path = ptrProperties->getProperty("UploadFiles.FilePath");
	if(upload_path.empty())
	{
		upload_path = "./";
	}
	else if(upload_path[upload_path.length()-1] != '/')
	{
		upload_path += "/";
	}

	int auth_read = access(upload_path.c_str(), R_OK);
	log_trace("auth_read: %d", auth_read);
	if(auth_read < 0)
	{
		log_error("upload_path permission denied: %s", upload_path.c_str());
		return NOK;
	}

	g_App.uploadPath = upload_path;
	return OK;
}

int DownloadFileCGI::Run()
{
	EINT error = 0;
	std::string errstr;
    do {
		error = ::Init(errstr);
		if (error)
		{
			break;
		}

        std::string fileID = m_pReqData->GetPara("file_id");
        std::string clientIp = m_pReqData->GetEnv("RealClientIp");
		std::string filePath = g_App.uploadPath + fileID;

        log_trace("pic_path:%s", pic_path.c_str());
        std::vector<std::string> vec_file_name;
        commonutil::splitString(pic_path, '.', vec_file_name);
        int32_t size = vec_file_name.size();
        if (size < 2) {
            error = -1;
            errstr = "文件类型错误";
            break;
        }
        file_ext = vec_file_name[size - 1];
        file_name = mch_id+"_"+commonutil::intToString(pic_type)+"."+file_ext;
        cout << "Content-Type: image/png\n" ;
        cout << "Content-Disposition: inline; filename=" << file_name <<"\n";
        cout << "X-Accel-Redirect: /" << pic_path <<"\n";
        cout << "\n";
        return -1;

    } while (0);

    if (errstr.empty()) {
        errstr = "系统繁忙";
    }
    val["error"] = I2S(error);
    val["errstr"] = errstr;

    std::cout << "Content-Type: application/json; charset=UTF-8\r\n\r\n";
    std::string jsonStr = val.toString();
    log_debug("json:%s", jsonStr.c_str());
    std::cout << jsonStr;
        
    
return -1 ;
}
