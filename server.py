#!/usr/local/bin/python3
import http.server
import socketserver
import mimetypes

PORT = 8000

Handler = http.server.SimpleHTTPRequestHandler

Handler.extensions_map.update({ '.wasm': 'application/wasm' })

httpd = socketserver.TCPServer(('', PORT), Handler)

print('serving at port', PORT)
print('open http://localhost:8000/')

httpd.serve_forever()
