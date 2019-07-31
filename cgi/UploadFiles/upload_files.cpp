#include "commonutil/api_logger.h"
#include "commonutil/api_mysql.h"
#include "commonutil/Properties.h"
#include "commonutil/Random.h"
#include "commonutil/StringUtil.h"
#include "commonutil/CharConvertUtil.h"
#include "commonutil/GlobalUtil.h"
#include "httputil/HttpUtil.h"
#include "jsonutil.hpp"
#include "errorutil.hpp"

#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

#include "upload_files.hpp"

struct GlobalData
{
	commonutil::PropertiesPtr ptrProperties;
	mysql::ConnectionPool* mysqlPool;
	PoolDeleter<mysql::Connection> mysqlDelter;

	std::string uploadPath;
	json outJson;
};

GlobalData g_App;

struct UploadFile
{
	std::string file_id;
	std::string file_name;
	std::string user_id;
	std::string from;
	int state;
	std::string create_time;

	EINT create(const MysqlConnScopedPtr& conn)
	{
		return OK;
#if 0
		int affected = conn->update(
				"insert into zhfq.t_upload_file_record set "
				"F_file_id='%s',F_user_id='%s',F_from='%s',"
				"F_file_path='%s',F_state=%d,F_create_time='%s'",
				file_id.c_str(), user_id.c_str(), from.c_str(),
				file_name.c_str(), 0, create_time.c_str());
		return affected == 1;
#endif
	}
};

// 上传文件输出
static
void cgi_output(json& jOut)
{
	EINT error = jOut.value("error", 0);
	if (error == OK)
	{
		jOut["errstr"] = "succ";
	}
	else
	{
		jOut["errstr"] = "fail";
	}

	std::cout << "Content-Type: application/json; charset=UTF-8\r\n\r\n";
	std::string strJson = jOut.dump();
	log_debug("json = %s", strJson.c_str());
	std::cout << strJson;
}

// 下载查看文件重定向
static
void cgi_redirect(const std::string& strPath, const std::string& strType)
{
	if (!strType.empty())
	{
		std::cout << "Content-Type: image/png\n" ;
	}

	std::cout << "Content-Disposition: inline; filename=" << file_name <<"\n";
	std::cout << "X-Accel-Redirect: /" << strPath <<"\n";
	std::cout << "\n";
}

static
EINT create_multi_dir(const std::string & path, std::string & error, mode_t mode = 0777)
{
	int i, len;

	len = path.length();
	char dir_path[len+1];
	dir_path[len] = '\0';

	strncpy(dir_path, path.c_str(), len);

	for (i=0; i<len; i++)
	{
		if (dir_path[i] == '/' && i > 0)
		{
			dir_path[i]='\0';
			if (access(dir_path, F_OK) < 0)
			{
				if (mkdir(dir_path, mode) < 0)
				{
					error = std::string("mkdir=") + dir_path + " msg=" + strerror(errno);
					return OK;
				}
			}
			dir_path[i]='/';
		}
	}

	return OK;
}

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

	int auth_exist = access(upload_path.c_str(), F_OK);
	log_trace("auth_exist: %d", auth_exist);
	if(auth_exist != 0)
	{
		std::string create_error;
		if(create_multi_dir(upload_path, create_error) < 0)
		{
			log_error(create_error.c_str());
		}
	}
	int auth_write = access(upload_path.c_str(), W_OK);
	log_trace("auth_write: %d", auth_write);
	if(auth_write < 0)
	{
		log_error("upload_path permission denied: %s", upload_path.c_str());
		return NOK;
	}

	g_App.uploadPath = upload_path;
	return OK;
}

////////////////////////////////////////////////////////////

int UploadFilesCGI::Run()
{
    EINT error = 0;
    std::string errstr;
    do {
		error = ::Init(errstr);
		if (error)
		{
			break;
		}

#if 0
        MysqlConnScopedPtr conn(g_App.mysqlPool->get(), g_App.mysqlDelter);
        if (!conn) {
            error = -1;
            break;
        }
#endif

		// 由参数 view 决定上传或下载行为
		std::string viewID = m_pReqData->GetPara("view");
		m_bUpload = viewID.empty();

        std::string strFrom = m_pReqData->GetPara("from");
        std::string strUser = m_pReqData->GetPara("user");
        std::string clientIp = m_pReqData->GetEnv("RealClientIp");
		std::string strNow = commonutil::Time::now().toDateTime();
		
        // 获取文件列表
        std::vector<cgicc::FormFile> files;
        m_pReqData->GetUploadFileList(files);
        int32_t fileCnt = files.size();
        if (!fileCnt) {
            error = NOK;
            errstr = "请选择文件上传";
            break;
        }
        
        json fileList;
        for (int32_t i = 0; i < fileCnt; i++)
		{
            std::string fileName = files[i].getFilename();
            json fileInfo;
			fileInfo["file_name"] = fileName;

            std::vector<std::string> nameData;
            commonutil::splitString(fileName, '.', nameData);
            int32_t size = nameData.size();
            if (size < 2) {
                error = NOK;
                errstr = "文件类型错误";
                break;
            }
            std::string fileExtName = nameData[size - 1];
            log_trace("name:%s, type:%s, name:%s, ext:%s", fileName.c_str(), files[i].getDataType().c_str(), files[i].getName().c_str(), fileExtName.c_str());

			std::string fileID = createFileID();
			std::string filePath = g_App.uploadPath + fileID + "." + fileExtName;
			std::string filePtr = files[i].getData();
			int fileLength = files[i].getDataLength();

			//打开fd
			std::fstream fd;
			log_trace("save file to: %s", filePath.c_str());
			fd.open(filePath.c_str(), std::fstream::out | std::fstream::trunc);  //创建文件
			if(!fd)
			{
				fileInfo["file_id"] = "0"; // 标记无效id
				log_error("save file[%s] failed.", fileName.c_str());
				continue;
			}
			fd.write(filePtr.c_str(), fileLength);
			fd.close();

			UploadFile record;
			record.file_id = fileID;
			record.file_name = fileName;
			record.user_id = strUser;
			record.from = strFrom;
			record.state = 0;
			record.create_time = strNow;

#if 0
			error = record.create(conn);
            if (error) {
				log_error("fail to crate upload_file_record");
				fileInfo["file_id"] = "0";
                continue;
            }
#endif

            fileInfo["file_id"] = fileID;
            fileList.push_back(fileInfo);
        }

		g_App.outJson["file_info"] = fileList;
    } while (0);

	g_App.outJson["error"] = error;
	g_App.outJson["errstr"] = errstr;
	cgi_output(g_App.outJson);

    return -1;
}

std::string UploadFilesCGI::createFileID()
{
    std::string id = 
        commonutil::Time::now().toYYYYMMDDhhmmss()
      + commonutil::intToString(commonutil::random(1000000));
    return id;
}

