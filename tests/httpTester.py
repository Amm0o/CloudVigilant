from http.server import BaseHTTPRequestHandler, HTTPServer
import json

class HTTPRequestHandler(BaseHTTPRequestHandler):

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        # Get data from client
        post_data = self.rfile.read(content_length)

        # Decode and print the data
        print("Received POST request with the following data:")
        print(post_data.decode('utf-8'))

        # Pase JSON and and print data
        try:
            json_data = json.loads(post_data)
            print("JSON data:", json_data)
        except:
            print("Invalid JSON")

        # Send response status code
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"POST request received\n")


# Server settings
server_address = ('', 8000) # Run on localhost:8000

httpd = HTTPServer(server_address, HTTPRequestHandler)

print(f"Starting server at http://localhost:{server_address[1]}")
httpd.serve_forever()