#!/bin/bash

if [ "$REQUEST_METHOD" == "GET" ]
then
echo "Content-type: text/html"
echo "Status: 200 OK"
echo ""
echo "<html>"
echo "<body>"
echo "<div style=\"width: 100%; font-size: 40px; font-weight: bold; text-align: center;\">"
echo "CGI Script BASH Test Page"
echo "</div>"
echo "<h2> REMOTE_ADDR $REMOTE_ADDR </h2>"
echo "<h2> REMOTE_PORT $REMOTE_PORT </h2>"
echo "<h2> REQUEST_METHOD $REQUEST_METHOD </h2>"
echo "<h2> REQUEST_URI $REQUEST_URI </h2>"
echo "<h2> SCRIPT_NAME $SCRIPT_NAME </h2>"
echo "<h2> SERVER_NAME $SERVER_NAME </h2>"
echo "<h2> SERVER_PORT $SERVER_PORT </h2>"
echo "<h2> SERVER_PROTOCOL $SERVER_PROTOCOL </h2>"
echo "<h2> PATH_INFO $PATH_INFO </h2>"
echo "<h2> PATH_TRANSLATED $PATH_TRANSLATED </h2>"
echo "<h2> QUERY_STRING $QUERY_STRING </h2>"
echo "</body>"
echo "</html>"
fi
if [ "$REQUEST_METHOD" == "POST" ]
then

fi
if [ "$REQUEST_METHOD" == "DELETE" ]; then
  if [ "$QUERY_STRING" -ne "" ]
  then
    echo "Content-type: text/html"
    echo "Status: 200 OK"
  else
    echo "Content-type: text/html"
    echo "Status: 400 OK"
    echo ""
    echo "<html>"
    echo "<body>"
    echo "<div style=\"width: 100%; font-size: 40px; font-weight: bold; text-align: center;\">"
    echo "BAD REQUEST"
    echo "</div>"
    echo "</body>"
    echo "</html>"
  fi
fi
