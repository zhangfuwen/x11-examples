#include <memory>
#include <stdint.h>
#include "libinetsocket.h"
#include "inetserverstream.hpp"
#include "inetclientstream.hpp"
#include "dgramoverstream.hpp"

#include "polym/Msg.hpp"
#include "polym/Queue.hpp"
#include "Log.h"
#include "socket.hpp"

class Server {
public:
    Server()
    {
        auto server = libsocket::inet_stream_server("0.0.0.0", "5000", LIBSOCKET_IPv4, 0);
        auto stream = server.accept2(1);
        dgramStream = std::make_unique<libsocket::dgram_over_stream>(std::move(stream));
    }
    void operator()()
    {
        const int bufSize = 100 * 1024;
        uint8_t buf[bufSize];
        while(true) {
            int recvdSize = dgramStream->rcvmsg(buf, bufSize);
            if (recvdSize <= 0) {
                LOGE("error: invalid message %d", recvdSize);
                break;
            }
            PolyM::DataMsg<std::string> msg(0, (char*)buf, recvdSize);
            queue.put(std::move(msg));
            LOGD("got message %d: %s", recvdSize, std::string((char*)buf, recvdSize).c_str());
        }
    }
    auto getMsg(int millis = 0)
    {
        return queue.get(millis);
    }
    ssize_t sendMsg(std::string s)
    {
        return dgramStream->sndmsg(s);

    }
private:
    PolyM::Queue queue;

    std::unique_ptr<libsocket::dgram_over_stream> dgramStream;
};

class Client {
public:
    Client()
    {
        auto client = libsocket::inet_stream("127.0.0.1", "5000", LIBSOCKET_IPv4, 0);
        dgramStream = std::make_unique<libsocket::dgram_over_stream>(std::move(client));
        dgramStream->enable_nagle(true);
    }

    ssize_t send(std::string&& msg)
    {
        return dgramStream->sndmsg(msg);
    }

    std::string recvMsg()
    {
        std::string msg;
        msg.resize(100*1024);
        dgramStream->rcvmsg(&msg);
        return msg;
    }
    
private:
    std::unique_ptr<libsocket::dgram_over_stream> dgramStream;

};
