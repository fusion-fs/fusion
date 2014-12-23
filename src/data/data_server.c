#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <syslog.h>
#include <signal.h>
#include <syslog.h>

#include <libwebsockets.h>
#include <rpc.hpp>
volatile int force_exit = 0;

static int
xio_protocol_callback(struct libwebsocket_context *context,
                      struct libwebsocket *wsi,
                      enum libwebsocket_callback_reasons reason,
                      void *user, void *in, size_t len)
{
	switch (reason) {

	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		fprintf(stderr, "LWS_CALLBACK_CLIENT_ESTABLISHED\n");
        {
            unsigned char buffer[4096];
            unsigned char *p = buffer;
            p += sprintf((char *)p,
                         "COMMAND:register\x0d\x0a"
                         "PATH:/tmp\x0d\x0a"
                         "CAPACITY:100\x0d\x0a");

            int n = libwebsocket_write(wsi, buffer, p - buffer, LWS_WRITE_BINARY);
            fprintf(stderr, "wrote %d\n", n);
        }

		break;

	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		fprintf(stderr, "LWS_CALLBACK_CLIENT_CONNECTION_ERROR\n");
		break;

	case LWS_CALLBACK_CLOSED:
		fprintf(stderr, "LWS_CALLBACK_CLOSED\n");
        //FIXME: retry if connection lost, but for now just exit
        force_exit = 1;
		break;
	case LWS_CALLBACK_CLIENT_RECEIVE:
        lwsl_notice("receive\n");
		fprintf(stderr, "%s\n", (char *)in);
		break;

    case LWS_CALLBACK_SERVER_WRITEABLE:
        lwsl_notice("writable\n");
        break;

	default:
		break;
	}

	return 0;
}

static struct libwebsocket_protocols protocols[] = {
    {
        "xio-protocol",
        xio_protocol_callback,
        sizeof(struct per_session_data_xio),
    },
    { NULL, NULL, 0} /* terminator */
};

void sighandler(int sig)
{
	force_exit = 1;
}

static struct option options[] = {
	{ "help",	no_argument,		NULL, 'h' },
	{ "port",	required_argument,	NULL, 'p' },
	{ NULL, 0, 0, 0 }
};

int usage()
{
    fprintf(stdout, "usage:\n");
    return 0;
}

int main(int argc, char **argv)
{
	int n = 0;
	int ret = 0;
	int port = 9999;
	struct libwebsocket_context *context;
	const char *address;
	struct libwebsocket *wsi_xio;
	int ietf_version = -1; /* latest */
	struct lws_context_creation_info info;
    int debug_level = 7;
    int syslog_options = LOG_PID | LOG_PERROR;
    /* we will only try to log things according to our debug_level */
    setlogmask(LOG_UPTO (LOG_DEBUG));
    openlog("lwsts", syslog_options, LOG_DAEMON);

    /* tell the library what debug level to emit and to send it to syslog */
    lws_set_log_level(debug_level, lwsl_emit_syslog);

	memset(&info, 0, sizeof info);

	if (argc < 2)
		return usage();

	while (n >= 0) {
		n = getopt_long(argc, argv, "hp:", options, NULL);
		if (n < 0)
			continue;
		switch (n) {
		case 'p':
			port = atoi(optarg);
			break;
		case 'h':
            return usage();
		}
	}

	if (optind >= argc)
        return usage();

	signal(SIGINT, sighandler);

	address = argv[optind];

	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;
    info.extensions = libwebsocket_get_internal_extensions();

	context = libwebsocket_create_context(&info);
	if (context == NULL) {
		fprintf(stderr, "Creating libwebsocket context failed\n");
		return 1;
	}

	/* create a client websocket using xio increment protocol */

	wsi_xio = libwebsocket_client_connect(context, address, port, 0,
                                          "/", argv[optind], "localhost",
                                          protocols[0].name, ietf_version);

	if (wsi_xio == NULL) {
		fprintf(stderr, "[%s] libwebsocket connect failed\n", address);
		ret = 1;
	}

	fprintf(stderr, "Waiting for connect...\n");
	n = 0;
	while (n >= 0 && !force_exit) {
        n = libwebsocket_service(context, 1);
    }

	fprintf(stderr, "Exiting\n");

	libwebsocket_context_destroy(context);

	return ret;
}
