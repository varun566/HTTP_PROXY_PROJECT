import urllib.request


proxy_host = input("Enter the proxy host (default: localhost): ") or 'localhost'
proxy_port = input("Enter the proxy port: ")

proxies = {
    "http": f"http://{proxy_host}:{proxy_port}",
    "https": f"http://{proxy_host}:{proxy_port}"  # Forwarding HTTPS through HTTP proxy
}

# List of URLs to be tested
urls = [
    "https://engineering.case.edu/eecs",
    "http://engineering.case.edu/eecs/about",
    "http://engineering.case.edu/eecs/node/190",
    "http://engineering.case.edu/",
    "http://engineering.case.edu/eecs/node/306",
    "http://engineering.case.edu/eecs/research",
    "http://engineering.case.edu/eecs/sites/engineering.case.edu.eecs/files/images/header-research.jpg",
    "http://httpbin.org/get",
    "http://neverssl.com",
    "https://www.wikipedia.org",
]

# Function to test if each URL can be accessed
def test_urls():
    for url in urls:
        try:
            # Make the HTTP request through the proxy
            response = urllib.request.urlopen(url, timeout=10)
            # Check if the response is valid (HTTP status code 200 OK)
            if response.getcode() == 200:
                print(f"Web Page {url}: succeed")
            else:
                print(f"Web Page {url}: fail with status code {response.getcode()}")
        except Exception as e:
            print(f"Web Page {url}: fail with error {str(e)}")

# Run the tests
if __name__ == "__main__":
    test_urls()
