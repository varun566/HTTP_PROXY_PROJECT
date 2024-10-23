
# HTTP Proxy Project

This project is an implementation of a simple HTTP proxy that forwards client requests to a remote server and relays the server's response back to the client. The proxy handles HTTP GET requests and can be used for testing purposes.

## Features

- **HTTP GET Request Handling:** 
  The proxy handles HTTP GET requests, forwarding them to the desired server and returning the response.
  
- **Port Configuration:** 
  The proxy server starts on a user-defined port, and clients can connect via that port.

- **Proxy Testing:** 
  The project includes a `grading-test.py` script to test the proxy using various URLs, as well as instructions on how to verify correct behavior using `wget`.

## Files

- `proxy.c`: 
  Contains the main proxy implementation.
  
- `utils.c`: 
  Utility functions used by the proxy to manage client-server connections.

  
- `test.py`: 
  An additional testing script to validate the proxy server's functionality using a set of sample URLs.

## How to Compile

To compile the project, use the following command:

```bash
gcc -o proxy proxy.c utils.c
```

This will generate the executable `proxy` that can be used to run the HTTP proxy server.

## How to Run the Proxy Server

Start the proxy server by running:

```bash
./proxy
```

The proxy will start on a default or specified port. The output will show the server's IP address and the port it's listening on.

## How to Test the Proxy

### Using `wget`

You can test the proxy using `wget` by comparing the results of a download with and without the proxy:

1. Download a resource without the proxy:

```bash
export http_proxy="" && wget http://example.com
```

2. Download the same resource through the proxy:

```bash
export http_proxy="http://127.0.0.1:PORT" && wget http://example.com
```

3. Compare the two downloaded resources using `diff`:

```bash
diff file_without_proxy.html file_with_proxy.html
```

If there are no differences, your proxy is functioning correctly.



You will be prompted to enter the proxy's host and port. The script will test several predefined URLs and report whether they were successfully fetched through the proxy.

### Using `test.py`

Run the additional testing script for more sample URLs:

```bash
sudo python3 test.py
```

You will be asked to provide the proxy's host and port. The script will attempt to fetch a list of URLs and report success or failure for each one.

## Troubleshooting

### DNS Resolution Error

If you encounter the error:

```
connecttoserver: Can't get host by name.
```

This indicates that the proxy was unable to resolve the hostname. Ensure that DNS resolution is correctly handled in your `proxy.c` implementation.

### Proxy Not Handling HTTPS

Note that this proxy only supports **HTTP** connections, not **HTTPS**. Ensure that your URLs start with `http://` instead of `https://` for testing.

## Additional Notes

- **Testing in Kali Linux:** If you're testing in a Kali Linux environment and encountering issues, ensure that firewall rules or other security settings are not blocking proxy connections.
- **Wireshark Debugging:** You can use Wireshark to inspect the traffic being sent through the proxy for debugging purposes.

## License

This project is licensed under the MIT License.
# HTTP_PROXY_PROJECT
