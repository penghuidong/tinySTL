#ifndef CONFIG_H
#define CONFIG_H

#define USE_DDEBUG
#define USE_EXCEPTION
#define HTTPSERVER


#ifdef HTTPSERVER
#define HTTPSERVER_PORT 8088
#define HTTPSERVER_BACKLOG 128
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"
#define ISspace(x) isspace((int)(x))
#endif


#endif // CONFIG_H
