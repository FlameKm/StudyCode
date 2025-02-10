#include "c_hello_client.h"
#include "erpc_client_setup.h"
#include "erpc_port.h"
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    /* 创建client端传输层对象(TCP),127.0.0.1:5407 */
    erpc_transport_t transport = erpc_transport_tcp_init("127.0.0.1", 5407, false);
    erpc_mbf_t message_buffer_factory = erpc_mbf_dynamic_init();
    /* 初始化客户端 */
    erpc_client_t client = erpc_client_init(transport, message_buffer_factory);
    /* RPC 调用 */
    initDEMO_client(client);

    const char *msg = "hello erpc";
    cout << "client hello_world" << endl;
    hello_world();
    cout << "client say_message: " << msg << endl;
    say_message(msg);
    cout << "erpc_add(1, 2) = ";
    int res = erpc_add(1, 2);
    cout << res << endl;
    /* 关闭socket */
    erpc_transport_tcp_deinit(transport);
}