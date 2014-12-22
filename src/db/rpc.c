#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <syslog.h>

#include <signal.h>

#include <libwebsockets.h>

int force_exit = 0;

static int
xio_protocol_callback(struct libwebsocket_context *context,
                      struct libwebsocket *wsi,
                      enum libwebsocket_callback_reasons reason,
                      void *user, void *in, size_t len)
{
    switch (reason) {

    case LWS_CALLBACK_ESTABLISHED:
        lwsl_notice("established\n");
        break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
        lwsl_notice("writable\n");
        break;

    case LWS_CALLBACK_RECEIVE:
        lwsl_notice("receive\n");
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

static struct libwebsocket_protocols protocols[] = {
    {
        "http-only",    /* name */
        callback_http,  /* callback */
        0               /* per_session_data_size */
    },
    {
        "xio-protocol",
        xio_protocol_callback,
        128,
    },
    { NULL, NULL, 0} /* terminator */
};

static void sighandler(int sig)
{
    force_exit = 1;
}

int start_rpc_server(ulong port)
{
    int n = 0;
    struct libwebsocket_context *context;
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

    n = 0;
    while (n >= 0 && !force_exit) {
	
        n = libwebsocket_service(context, 50);
		
    };


    libwebsocket_context_destroy(context);

    lwsl_notice("libwebsocket server exited cleanly\n");

    closelog();

    return 0;
}
