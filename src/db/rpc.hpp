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

#ifdef __cplusplus
extern "C" {
extern int write_to_client(int fd, unsigned char *data, ulong len);
extern int read_from_client(int fd, unsigned char *req, ulong req_len, 
                         unsigned char *resp, ulong resp_len);
}
#endif
