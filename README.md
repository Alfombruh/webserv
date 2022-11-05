# WEBSERV
## To-Do List

- look up what .ipp and .tpp file extensions are
- what is an HTTP server
- READ "the RFC" DO SOME TEST WITH "telnet" AND "NGINX" before starting to work in this 

---
## File hierarchy
```
.
├─ includes
├─ srcs
│  └─ main.cpp
├─ config
├─ Makefile
└─ README.md
```
---
## Requirements
```
• Your program has to take a configuration file as argument, or use a default path.
• You can’t execve another web server.
• Your server must never block and the client can be bounced properly if necessary.
• It must be non-blocking and use only 1 poll() (or equivalent) for all the I/O operations 
  between the client and the server (listen included).
• poll() (or equivalent) must check read and write at the same time.
• You must never do a read or a write operation without going through poll() (or equivalent).
• Checking the value of errno is strictly forbidden after a read or a write operation.
• You don’t need to use poll() (or equivalent) before reading your configuration file.
• You can use every macro and define like FD_SET, FD_CLR, FD_ISSET, FD_ZERO (understanding 
  what and how they do it is very useful).
• A request to your server should never hang forever.
• Your server must be compatible with the web browser of your choice.
• We will consider that NGINX is HTTP 1.1 compliant and may be used to compare
  headers and answer behaviors.
• Your HTTP response status codes must be accurate.
• You server must have default error pages if none are provided.
• You can’t use fork for something else than CGI (like PHP, or Python, and so forth).
• You must be able to serve a fully static website.
• Clients must be able to upload files.
• You need at least GET, POST, and DELETE methods.
• Stress tests your server. It must stay available at all cost.
• Your server must be able to listen to multiple ports (see Configuration file).
```
---
## Configuration Files
```
In the configuration file, you should be able to:
• Choose the port and host of each ’server’.
• Setup the server_names or not.
• The first server for a host:port will be the default for this host:port (that means
it will answer to all the requests that don’t belong to an other server).
• Setup default error pages.
• Limit client body size.
• Setup routes with one or multiple of the following rules/configuration (routes wont
be using regexp):
  ◦ Define a list of accepted HTTP methods for the route.
  ◦ Define a HTTP redirection.
  ◦ Define a directory or a file from where the file should be searched (for example,
    if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is
    /tmp/www/pouic/toto/pouet).
  ◦ Turn on or off directory listing.
  ◦ Set a default file to answer if the request is a directory.
  ◦ Execute CGI based on certain file extension (for example .php).
  ◦ Make the route able to accept uploaded files and configure where they should
    be saved.
    ∗ Do you wonder what a [CGI][CGI] is?
    ∗ Because you won’t call the CGI directly, use the full path as PATH_INFO.
    ∗ Just remember that, for chunked request, your server needs to unchunked
      it and the CGI will expect EOF as end of the body.
    ∗ Same things for the output of the CGI. If no content_length is returned
      from the CGI, EOF will mark the end of the returned data.
    ∗ Your program should call the CGI with the file requested as first argument.
    ∗ The CGI should be run in the correct directory for relative path file access.
  ∗ Your server should work with one CGI (php-CGI, Python, and so forth).
```
---
## Allowed functions an usage
<table>
  <tr>
    <th>Function</th>
    <th>Prototype</th>
    <th>Description</th>
  </tr>
  <tr>
    <td><a href="https://linux.die.net/man/3/htons">htons</a></td>
    <td><code>uint16_t htons(uint16_t hostshort)</code></td>
    <td>Host To Number Short (#include &ltarpa/inet.h&gt)</td>
    </tr>
    <tr>
    <td><a href="https://linux.die.net/man/3/htonl">htonl</a></td>
    <td><code>uint32_t htons(uint32_t hostlong)</code></td>
    <td>Host To Number Long(#include &ltarpa/inet.h&gt)</td>
    </tr>
  <tr>
    <td><a href="https://linux.die.net/man/3/ntohs">ntohs</a></td>
    <td><code>uint16_t ntohs(uint16_t hostshort)</code></td>
    <td>Number to Hostshort (#include &ltarpa/inet.h&gt)</td>
  </tr>
   <tr>
    <td><a href="https://linux.die.net/man/3/ntohl">ntohl</a></td>
    <td><code>uint32_t ntohl(uint32_t hostlong)</code></td>
    <td>Number to Hostlong (#include &ltarpa/inet.h&gt)</td>
  </tr>
   <tr>
    <td><a href="https://linux.die.net/man/1/select">select</a></td>
    <td><code> int
     select(int nfds, 
     fd_set *restrict readfds, 
     fd_set *restrict writefds, 
     fd_set *restrict errorfds, s
     truct timeval *restrict timeout);</code></td>
    <td>To be Found (#include &ltsys/select.h&gt)</td>
  </tr>
  <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/poll.2.html">poll</a></td>
    <td><code>int poll(struct pollfd fds[], nfds_t nfds, int timeout);</code></td>
    <td>To be Found (#include &ltpoll.h&gt)</td>
  </tr>
  <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/epoll_create.2.html">epoll_create</a></td>
    <td><code>int epoll_create(int flag/size);</code></td>
    <td>To be Found (#include &ltsys/epoll.h&gt)</td>
  </tr>
    <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/epoll_ctl.2.html">epoll_ctl</a></td>
    <td><code>int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);</code></td>
    <td>Add modify or remove epoll things (#include &ltsys/epoll.h&gt)</td>
  </tr>
   </tr>
    <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/epoll_wait.2.html">epoll_wait</a></td>
    <td><code>int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);</code></td>
    <td>Waits for an epoll event (#include &ltsys/epoll.h&gt)</td>
  </tr>
  <tr>
    <td><a href="https://www.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2">kqueue</a></td>
    <td><code>int kqueue(void);</code></td>
    <td>To be Found (#include &ltsys/event.h&gt/&ltsys/time.h&gt)</td>
  </tr>
  <tr>
    <td><a href="https://www.daemon-systems.org/man/kevent.2.htmll">kevent</a></td>
    <td><code>int
     kevent(int kq, const struct kevent *changelist, size_t nchanges,
     struct kevent *eventlist, size_t nevents,
     const struct timespec *timeout);</code></td>
    <td>Long to write rn (#include &ltsys/event.h&gt/&ltsys/time.h&gt)</td>
  </tr>
 <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/socket.2.html">socket</a></td>
    <td><code>int socket(int domain, int type, int protocol);</code></td>
    <td>Creates an endpoint for comunication and return an fd for it (#include &ltsys/socket.h&gt)</td>
  </tr>
   <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/accept.2.html">accept</a></td>
    <td><code>int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);</code></td>
    <td>Extracts the first connection request on the queue from a socket (#include &ltsys/socket.h&gt)</td>
  </tr>
 <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/accept.2.html">accept</a></td>
    <td><code>int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);</code></td>
    <td>Extracts the first connection request on the queue from a socket (#include &ltsys/socket.h&gt)</td>
  </tr>
  <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/listen.2.html">listen</a></td>
    <td><code>int listen(int sockfd, int backlog);</code></td>
    <td>Makes the socket passive, which makes the socket accept connections(#include &ltsys/socket.h&gt)</td>
  </tr>
  <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/send.2.html">send</a></td>
    <td><code>ssize_t send(int sockfd, const void *buf, size_t len, int flags);</code></td>
    <td>Transmits a message to a socket(#include &ltsys/socket.h&gt)</td>
  </tr>
    <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/recv.2.html">recv</a></td>
    <td><code>ssize_t recv(int sockfd, void *buf, size_t len, int flags);</code></td>
    <td>Recieves a message from a socket(#include &ltsys/socket.h&gt)</td>
  </tr>
    <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/bind.2.html">bind</a></td>
    <td><code> int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);</code></td>
    <td>Binds a socket name to a address(#include &ltsys/socket.h&gt)</td>
  </tr>
   <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/connect.2.html">connect</a></td>
    <td><code>int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);</code></td>
    <td>Connects the socked reffered by sockedfd to the address(#include &ltsys/socket.h&gt)</td>
  </tr>
    <tr>
    <td><a href="https://linux.die.net/man/3/inet_addr">inet_addr</a></td>
    <td><code>in_addr_t inet_addr(const char *cp);</code></td>
    <td>Converts the ipv4 direction to into binary into binary data(#include &ltsys/socket.h&gt)</td>
  </tr>
    <tr>
    <td><a href="https://linux.die.net/man/2/setsockopt">setsockopt</a></td>
    <td><code>int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);</code></td>
    <td>Manipulate options of the socket(#include &ltsys/socket.h&gt)</td>
  </tr>
    <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/getsockname.2.html">getsocketname</a></td>
    <td><code>int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);</code></td>
    <td>Returns the socked address(#include &ltsys/socket.h&gt)</td>
  </tr>
  <tr>
    <td><a href="https://man7.org/linux/man-pages/man2/fcntl.2.html">fcntl</a></td>
    <td><code>int fcntl(int fd, int cmd, ... /* arg */ );</code></td>
    <td>Performs an operation on the fd based on cmd(#include &ltfcntl.h&gt)</td>
  </tr>
</table>

```Libraries that we are using:```
- [<arpa/inet.h>][arpa/inet.h]
- [<sys/select.h][sys/select.h]
- [<poll.h>][poll.h]
- [<sys/epoll.h>][sys/epoll.h]
- [<sys/event.h>][sys/event.h]
- [<sys/time.h>][sys/time.h]
- [<sys/socket.h>][sys/socket.h]

---
## Ok, but what is HTTP?

__Hypertext Transfer Protocol__

HTTP is a protocol, a set of rules for data transmission between computers, for fetching resources.
So it is the set of rules servers and clients use to transfers documents on the web back and forth.

HTTP is a "Stateless" procotol, which means that it does not have memory of previus requests. That make users unable to walk through requests because they are not conected.
Even though it is statless it is not Sessionless, which means that the server can store data of you previus request and store them as "cookies". So when you request access back to 
a website you visited it has info about you.
Cookies are saved as HTTP Headers, which can store all kind of data request

So the last basic priciple about it is that it works based on request/response pairs. So every action starts with a request using an HTTP method, and ends with a response of a HTTP status code, along with what happened to the request, data ...

## What's an HTTP server?


---
## BIBLIOGRAPHY

- [An overview of HTTP][AOFHTTP]
- 

## LICENSE
I Do not belive in those things


[//]: #
  [arpa/inet.h]: <https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html>
  [sys/select.h]: <https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/select.h.html>
  [poll.h]: <https://pubs.opengroup.org/onlinepubs/7908799/xsh/poll.h.html>
  [sys/epoll.h]: <https://man7.org/linux/man-pages/man7/epoll.7.html>
  [sys/event.h]: <https://opensource.apple.com/source/xnu/xnu-792/bsd/sys/event.h.auto.html>
  [sys/time.h]: <https://pubs.opengroup.org/onlinepubs/7908799/xsh/systime.h.html>
  [sys/socket.h]: <https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html>
  [CGI]: <https://en.wikipedia.org/wiki/Common_Gateway_Interface>
  [AOFHTTP]: <https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview>