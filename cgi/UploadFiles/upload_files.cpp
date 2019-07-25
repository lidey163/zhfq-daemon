#include <commonutil/api_logger.h>
#include <commonutil/HttpUtil.h>
#include <commonutil/Properties.h>
#include <commonutil/Random.h>
#include <commonutil/MapToString.h>
#include <commonutil/StringUtil.h>
#include <commoninclude/mposerror.h>
#include <commonutil/CharConvertUtil.h>
#include <commonutil/GlobalUtil.h>
#include <httputil/HttpUtil.h>
#include <json/json.h>
#include "upload_files.h"


commonutil::PropertiesPtr g_ptrProperties;

int UploadFilesCGI::Run()
{
    
    commonutil::PropertiesPtr ptrProperties = new commonutil::Properties();
    ptrProperties->load(commonutil::Properties::getPropertyPath());
    g_ptrProperties = ptrProperties;

    init_log(ptrProperties->getProperty("SSMPosLogService.IP"), ptrProperties->getPropertyAsInt("SSMPosLogService.Port"));
 
    mysql::conn_info_t info;
    info.host = ptrProperties->getProperty("SSMPosMysqlServer.IP");
    info.port = ptrProperties->getPropertyAsInt("SSMPosMysqlServer.Port");
    info.user = ptrProperties->getProperty("SSMPosMysqlServer.User");
    info.pwd = ptrProperties->getProperty("SSMPosMysqlServer.Password").c_str();
    info.db = ptrProperties->getProperty("SSMPosMysqlServer.Database");
    info.charset = ptrProperties->getProperty("SSMPosMysqlServer.CharsetName");
    info.client_ip = ptrProperties->getProperty("SSMPosMysqlServer.ClientIP");

    mysql::ConnectionPool* mysqlPool;
    PoolDeleter<mysql::Connection> conn_delter;
    mysqlPool = new mysql::ConnectionPool(info, -1);
    mysqlPool->init();
    conn_delter.setPool(mysqlPool);

    std::string sessionCheckUrl = ptrProperties->getProperty("OmsSessionCheckUrl");
    std::string configFilePath = ptrProperties->getProperty("fileUpload.FastDfsConfigFilePath");

    HttpUtil::InitHttpEnvironment();


    int error = 0;
    std::string errstr;
    Json::Value val;
    do {
        MysqlConnScopedPtr conn(mysqlPool->get(), conn_delter);
        if (!conn) {
            error = -1;
            break;
        }

        std::string strFrom = m_pReqData->GetPara("from");
        std::string clientIp = m_pReqData->GetEnv("RealClientIp");
        std::string user_id;
		
		#ifndef DEBUG_GLOBAL
        if (strFrom == "app") {
        }
		else if (strFrom == "web") {
        }
		else {
            log_trace("unknown source:%s", strFrom.c_str());
            error = -1;
            break;
        }
		#endif
		
        // 获取文件列表
        std::vector<cgicc::FormFile> files;
        m_pReqData->GetUploadFileList(files);
        int32_t fileCnt = files.size();
        if (!fileCnt) {
            error = MPOSError::kArgumentError;
            errstr = "请选择文件上传";
            break;
        }
        
        // fastFDS 文件操作初始化
        FdsClient fdsClient;
        error = fdsClient.Init(configFilePath);
        if (error) {
            error = MPOSError::kSystemError;
            errstr = "系统繁忙";
            break;
        }

        Json::Value fileList;
        for (int32_t i = 0; i < fileCnt; i++) {
            std::string fileName = files[i].getFilename();
            Json::Value fileInfo;
            std::vector<std::string> nameData;
            commonutil::splitString(fileName, '.', nameData);
            int32_t size = nameData.size();
            if (size < 2) {
                error = MPOSError::kArgumentError;
                errstr = "文件类型错误";
                break;
            }
            std::string fileExtName = nameData[size - 1];
            log_trace("name:%s, type:%s, name:%s, ext:%s", fileName.c_str(), files[i].getDataType().c_str(), files[i].getName().c_str(), fileExtName.c_str());

            // 上传文件
            std::string fdsFileID;
            if (fdsClient.UploadFileByBuff(files[i].getData().c_str(), files[i].getDataLength(), fileExtName.c_str(), fdsFileID)) {
                error = -1;
                errstr = "文件上传失败";
                break;
            }
            log_trace("fdsFileID:%s", fdsFileID.c_str());

            std::string fileID = createFileID();

            int affected = conn->update(
                "insert into ssmpos.t_upload_file_record set "
                "F_file_id='%s',F_user_id='%s',F_from='%s',"
                "F_file_path='%s',F_state=%d,F_create_time='%s'",
                fileID.c_str(), user_id.c_str(), strFrom.c_str(),
                fdsFileID.c_str(), 0, commonutil::Time::now().toDateTime().c_str());
            if (affected != 1) {
                error = -1;
                break;
            }

            fileInfo["file_name"] = fileName;
            fileInfo["file_id"] = fileID;
            fileList.append(fileInfo);
        }
        fdsClient.Destory();
        val["file_info"] = fileList;
    } while (0);

    if (errstr.empty() && error != 0) {
        errstr = "系统繁忙";
    }
    val["error"] = I2S(error);
    val["errstr"] = errstr;

    std::cout << "Content-Type: application/json; charset=UTF-8\r\n\r\n";
    std::string jsonStr = val.toString();
    log_debug("json:%s", jsonStr.c_str());
    std::cout << jsonStr;

    return -1;
}

std::string UploadFilesCGI::createFileID()
{
    std::string id = 
        commonutil::Time::now().toYYYYMMDDhhmmss()
      + commonutil::intToString(commonutil::random(1000000));
    return id;
}

