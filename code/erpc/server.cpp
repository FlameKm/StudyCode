#include "c_hello_server.h"
#include "erpc_server_setup.h"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

bool nestingDetection;

using namespace std;
/** 返回当前时间字符串 */
static std::string now_str()
{
    time_t t =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%F %T");
    return ss.str();
}

void hello_world() { std::cout << "server hello world" << std::endl; }

/** servicer端实现接口方法 */
void say_message(const char *msg)
{
    std::cout << "server say_message:" << msg << std::endl;
}

int32_t erpc_add(int32_t data1, int32_t data2) { return data1 + data2; }

int main(int argc, char *argv[])
{
    // 创建client端传输层对象(TCP),127.0.0.1:5407
    auto transport = erpc_transport_tcp_init("127.0.0.1", 5407, true);

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory = erpc_mbf_dynamic_init();

    /* eRPC 服务端初始化 */
    erpc_server_t server = erpc_server_init(transport, message_buffer_factory);

    /** 将生成的接口服务DEMO添加到server, 参见生成的源文件 erpcdemo_server.h */
    erpc_add_service_to_server(server, create_DEMO_service());
    cout << "start erpcdemo server" << endl;

    /* 启动服务器 */
    erpc_server_run(server); /* or erpc_server_poll(); */

    /* 关闭socket */
    erpc_transport_tcp_deinit(transport);
    return 0;
}