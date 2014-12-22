extern int start_rpc_server(ulong port);
extern int write_to_client(int fd, unsigned char *data, ulong len);
extern int read_from_client(int fd, unsigned char *req, ulong req_len, 
                         unsigned char *resp, ulong resp_len);
