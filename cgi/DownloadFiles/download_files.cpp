#include <commonutil/api_logger.h>
#include <commonutil/Properties.h>
#include <commonutil/service.h>
#include <httputil/HttpUtil.h>
#include <commoninclude/mposerror.h>
#include "DownloadAuthCGI.h"
#include "protocpp/service.base.pb.h"
#include <commonutil/GlobalUtil.h>
#include <commonutil/CharConvertUtil.h>

#include <errno.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

commonutil::PropertiesPtr g_ptrProperties;

int DownloadFileCGI::Run()
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

    HttpUtil::InitHttpEnvironment();


    int error = 0;
    std::string errstr;
    Json::Value val;
    do {
        std::string strFrom = m_pReqData->GetPara("from");
        std::string mch_id = m_pReqData->GetPara("mch_id");
        std::string acq_id = m_pReqData->GetPara("acq_id");
        int pic_type = atoi(m_pReqData->GetPara("pic_type").c_str());

        std::string user_id;
        std::string clientIp = m_pReqData->GetEnv("RealClientIp");

        MysqlConnScopedPtr conn(mysqlPool->get(), conn_delter);
        if (!conn) {
            error = -1;
            break;
        }

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

        std::string file_ext;
        std::string file_name;
        std::string pic_path;

        if(not mch_id.empty()){
            ScopedPtr<mysql::ResultSet> rs;
            rs.reset(conn->query(
                "select F_pic_name from ssmpos.t_cert_pic where F_mch_id='%s' and F_pic_type=%d",
                mch_id.c_str(), pic_type));
            if (!rs || !rs->next()) {
                log_trace("not pic found for mch_id:%s pic_type:%d", mch_id.c_str(), pic_type);
                error = -1;
                break;
            }
            pic_path = rs->getString(0);
        }else if(not acq_id.empty()){
            ScopedPtr<mysql::ResultSet> rs;
            rs.reset(conn->query(
                "select F_pic_name from ssmpos.t_acquirer_cert_pic where F_acq_id='%s' and F_pic_type=%d",
                acq_id.c_str(), pic_type));
            if (!rs || !rs->next()) {
                log_trace("not pic found for t_acquirer_cert_pic:%s pic_type:%d", acq_id.c_str(), pic_type);
                error = -1;
                break;
            }
            pic_path = rs->getString(0);
        }

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
