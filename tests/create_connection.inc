/*

  Copyright (c) 2016 Martin Sustrik

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include <assert.h>
#include <stdio.h>
#include <libdill.h>

#include "../dillsocks.h"

coroutine int client(chan ch) {
    ipaddr addr;
    int rc = ipremote(&addr, "127.0.0.1", 5555, 0, -1);
    assert(rc == 0);
    int cs = tcpconnect(&addr, -1);
    assert(cs);
    rc = chsend(ch, &cs, sizeof(cs), -1);
    assert(rc == 0);
    return 0;
}

void create_tcp_connection(int s[2]) {
    ipaddr addr;
    int rc = iplocal(&addr, NULL, 5555, 0);
    assert(rc == 0);
    int lst = tcplisten(&addr, 10);
    assert(lst);
    chan ch = channel(sizeof(int), 0);
    assert(ch);
    int chndl = go(client(ch));
    assert(chndl >= 0);
    s[0] = tcpaccept(lst, -1);
    assert(s[0]);
    rc = chrecv(ch, &s[1], sizeof(int), -1);
    assert(rc == 0);
    rc = hwait(chndl, NULL, -1);
    assert(rc == 0);
    chclose(ch);
    hclose(lst);
}
