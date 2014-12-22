#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <libwebsockets.h>
#include <set>
#ifdef __cplusplus
extern "C" {
    enum xio_states {
        XIO_INIT,
        XIO_ESTABLISHED,
        XIO_SEND_COMMAND,
        XIO_HANDLE_MESSAGE,
        XIO_PAYLOAD_SUM,
    };

    struct per_session_data_xio {
        int packets_left;
        int total_message;
        unsigned long sum;
        enum xio_states state;
    };

    static struct libwebsocket_context *context;
    using namespace std;
    typedef pair<int, libwebsocket *> socket_pair;
    static set<socket_pair> client_vec;
    static volatile int force_exit = 0;

    static int add_client(struct libwebsocket *wsi)
    {
        int fd = libwebsocket_get_socket_fd(wsi);
        char peer_name[128], ip[30];
        libwebsockets_get_peer_addresses(context, wsi,
                                         fd,
                                         peer_name, sizeof peer_name, ip, sizeof ip);
        fprintf(stderr, "add %s %s\n", peer_name, ip);

        client_vec.insert(make_pair(fd, wsi));
    }

    static int remove_client(struct libwebsocket *wsi)
    {
        int fd = libwebsocket_get_socket_fd(wsi);
        set<socket_pair>::iterator it;
        for (it = client_vec.begin(); it != client_vec.end(); it ++) {
            if (it->first == fd){
                char peer_name[128], ip[30];
                libwebsockets_get_peer_addresses(context, wsi,
                                                 fd,
                                                 peer_name, sizeof peer_name, ip, sizeof ip);
                fprintf(stderr, "remove %s %s\n", peer_name, ip);
                client_vec.erase(it);
                break;
            }
        }

    }

    static struct libwebsocket *get_client(int fd)
    {
        set<socket_pair>::iterator it;
        for (it = client_vec.begin(); it != client_vec.end(); it ++) {
            if (it->first == fd){
                char peer_name[128], ip[30];
                libwebsockets_get_peer_addresses(context, it->second,
                                                 fd,
                                                 peer_name, sizeof peer_name, ip, sizeof ip);
                fprintf(stderr, "found %s %s\n", peer_name, ip);
                return it->second;
            }
        }
        return NULL;
    }

    static int
    xio_protocol_callback(struct libwebsocket_context *context,
                          struct libwebsocket *wsi,
                          enum libwebsocket_callback_reasons reason,
                          void *user, void *in, size_t len);

    static int callback_http(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len);

    static struct libwebsocket_protocols protocols[] = {
        {
            "http-only",    /* name */
            callback_http,  /* callback */
            0               /* per_session_data_size */
        },
        {
            "xio-protocol",
            xio_protocol_callback,
            4096,
        },
        { NULL, NULL, 0} /* terminator */
    };

    static void handshake_info(struct libwebsocket *wsi)
    {
    }

    static int
    xio_protocol_callback(struct libwebsocket_context *context,
                          struct libwebsocket *wsi,
                          enum libwebsocket_callback_reasons reason,
                          void *user, void *in, size_t len)
    {
        struct per_session_data_xio *xio = (struct per_session_data_xio *)user; 

        switch (reason) {

        case LWS_CALLBACK_ESTABLISHED:
            lwsl_notice("established\n");
            xio->state = XIO_ESTABLISHED;
            add_client(wsi);
            break;


        case LWS_CALLBACK_SERVER_WRITEABLE:
            lwsl_notice("writable\n");
            break;

        case LWS_CALLBACK_CLOSED:
            fprintf(stderr, "LWS_CALLBACK_CLOSED\n");
            remove_client(wsi);
            break;

        case LWS_CALLBACK_RECEIVE:
        {
            fprintf(stderr, "receive:\n");
            fprintf(stderr, "%s\n", (char *)in);
            switch (xio->state) {
            case XIO_ESTABLISHED:
            {
                unsigned char buffer[4096];
                unsigned char *p = buffer;
                p += sprintf((char *)p,
                             "Status: OK\x0d\x0a");
                
                int n = libwebsocket_write(wsi, buffer, p - buffer, LWS_WRITE_BINARY);
            }
            break;
            }

        }
        break;
        case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION:
            handshake_info(wsi);
            lwsl_notice("filter\n");
            break;

        default:
            break;
        }

        return 0;
    }

    static int callback_http(struct libwebsocket_context *context, struct libwebsocket *wsi, enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len)
    {
        switch (reason) {
        case LWS_CALLBACK_HTTP: 
            lwsl_notice("http.\n");
            break;
 
        case LWS_CALLBACK_HTTP_FILE_COMPLETION:
            lwsl_info("LWS_CALLBACK_HTTP_FILE_COMPLETION seen\n");
            break;
        case LWS_CALLBACK_HTTP_WRITEABLE:
            lwsl_info("LWS_CALLBACK_HTTP_WRITABLE\n");
            break;
 
        default:
            break;
        }
        return 0;
    }


    static void sighandler(int sig)
    {
        force_exit = 1;
    }

    int start_rpc_server(ulong port)
    {
        int n = 0;

        int opts = LWS_SERVER_OPTION_LIBEV;
        const char *iface = NULL;
        int syslog_options = LOG_PID | LOG_PERROR;
        struct lws_context_creation_info info;

        int debug_level = 7;

        memset(&info, 0, sizeof info);
        info.port = port;


        signal(SIGINT, sighandler);

        /* we will only try to log things according to our debug_level */
        setlogmask(LOG_UPTO (LOG_DEBUG));
        openlog("lwsts", syslog_options, LOG_DAEMON);

        /* tell the library what debug level to emit and to send it to syslog */
        lws_set_log_level(debug_level, lwsl_emit_syslog);

        info.iface = iface;
        info.protocols = protocols;
        info.extensions = libwebsocket_get_internal_extensions();
        info.ssl_cert_filepath = NULL;
        info.ssl_private_key_filepath = NULL;

        info.gid = -1;
        info.uid = -1;
        info.options = opts;

        context = libwebsocket_create_context(&info);
        if (context == NULL) {
            lwsl_err("libwebsocket init failed\n");
            return -1;
        }
        libwebsocket_callback_on_writable_all_protocol(&protocols[1]);
        n = 0;
        while (n >= 0 && !force_exit) {
	
            n = libwebsocket_service(context, 50);
		
        };


        libwebsocket_context_destroy(context);

        lwsl_notice("libwebsocket server exited cleanly\n");

        closelog();

        return 0;
    }

    int write_to_client(int fd, unsigned char *data, ulong len)
    {
        struct libwebsocket *wsi = get_client(fd);
        if (!wsi || !data || !len) {
            return -1;
        }
        return libwebsocket_write(wsi, data , len, LWS_WRITE_BINARY);
    }

    int read_from_client(int fd, unsigned char *req, ulong req_len, 
                         unsigned char *resp, ulong resp_len)
    {
        struct libwebsocket *wsi = get_client(fd);
        if (!wsi || !req || !resp || !req_len || !resp_len) {
            return -1;
        }
        
        int n = libwebsocket_write(wsi, req , req_len, LWS_WRITE_BINARY);
        if (n < 0 ){
            return n;
        }
        n = libwebsocket_read(context, wsi, resp, resp_len);
        return n;
    }

}
#endif
