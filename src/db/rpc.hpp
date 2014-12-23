enum xio_states {
    XIO_INIT,
    XIO_ESTABLISHED,
    XIO_SEND_COMMAND,
    XIO_HANDLE_MESSAGE,
    XIO_COMMAND_DONE,
};

struct per_session_data_xio {
    int packets_left;
    int total_message;
    unsigned long sum;
    enum xio_states state;
};
extern volatile int force_exit;
#ifdef __cplusplus
extern "C" {
    extern int find_client(const char *path);
    extern int write_to_client(int fd, unsigned char *data, ulong len);
    extern int read_from_client(int fd, unsigned char *req, ulong req_len, 
                                unsigned char *resp, ulong resp_len);
    extern int start_rpc_server(ulong);
}
#else
extern int find_client(const char *path);
extern int write_to_client(int fd, unsigned char *data, ulong len);
extern int read_from_client(int fd, unsigned char *req, ulong req_len, 
                            unsigned char *resp, ulong resp_len);
extern int start_rpc_server(ulong);
#endif
