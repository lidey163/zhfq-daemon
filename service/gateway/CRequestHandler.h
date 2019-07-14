#ifndef GATEWAY_REQUESTHANDLER_H
#define GATEWAY_REQUESTHANDLER_H

#include "fastcgi/fastcgi_server.hpp"
#include "fastcgi/fastcgi_reply.hpp"

class CRequestHandler : public t_fastcgi_server::t_handler
{
public:
    void HandleRequest(const commonnet::TransceiverPtr&, const t_fastcgi_request_ref &req);

private:
    int HandleFastcgi(const t_fastcgi_request_ref &req, const t_fastcgi_reply_ref &reply);

    std::string GetRealClientIp(const t_fastcgi_request_ref &req);
};

#endif

