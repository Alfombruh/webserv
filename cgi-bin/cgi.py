#!/usr/bin/python3
import os

print("Content-Type: text/html")
print("Status: 200 OK")
print("\r")
print("<!DOCTYPE html>")
print("<html>\n<body>")
print("<div style=\"width: 100%; font-size: 40px; font-weight: bold; text-align: center;\">")
print("CGI PYTHON Script Test Page")
print("</div>")
for a in os.environ:
    print('<h2> ', a, os.getenv(a), "</h2>");
print("</body>\n</html>")