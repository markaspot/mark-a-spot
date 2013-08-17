Simple-PHP-Proxy
=============

First written by Alexxz  
Get it at <https://github.com/rafsoaken/Simple-php-proxy-script>

This is a simple php proxy script. It can be used where it is difficult to use other proxies like 
nginx or lighttpd. You can currently specify one single domain that this script proxies to.
You can also specify a number of headers that are proxied.

### Installation & Usage

Download and put the files into an accessible path on your website. Then copy src/example.simple-php-proxy_config.php 
to src/simple-php-proxy_config.php or to one or two levels above. Finally change the config values accordingly.

A request looks like this:

    http://www.alice.com/folder/simple-php-proxy/src/index.php/my_other_uri/that/resides/on/bob
    or without the index.php
    http://www.alice.com/folder/simple-php-proxy/src/my_other_uri/that/resides/on/bob

and the part "/my_other_uri/that/resides/on/bob" gets proxied to your specified domain. The script then outputs what it receives from:

    http://bob.com/my_other_uri/that/resides/on/bob

### License

The MIT License applies to this software

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
    

