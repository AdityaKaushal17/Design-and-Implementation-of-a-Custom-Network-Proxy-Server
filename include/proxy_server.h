#include "proxy_server.h"

int main() {
    ProxyServer server("127.0.0.1", 8888);
    server.start();
    return 0;
}
