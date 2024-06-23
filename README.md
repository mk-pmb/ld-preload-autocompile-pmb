
<!--#echo json="package.json" key="name" underline="=" -->
ld-preload-autocompile-pmb
==========================
<!--/#echo -->

<!--#echo json="package.json" key="description" -->
Automatically compile shared objects for LD_PRELOAD injection into another
command.
<!--/#echo -->



Installation
------------

`npm install github:mk-pmb/ld-preload-autocompile-pmb#master`

On Ubuntu, you may need to also install the apt package `build-essentials`.

Other OSs are not currently supported.



Usage
-----

#### `./autoldpre.sh hook1.c [hook2.c [… hookN.c]] [--] <command> [<args>]`

Example: Compile [`example.strftime.c`](example.strftime.c)
and inject it into the `date` command.

```text
$ ./autoldpre.sh example.strftime.c date -uRd @1234567890
<< intercepted strftime(): format: ' %a'>>
<< intercepted strftime(): result: ' Fri'>>
Fri, 13 << intercepted strftime(): format: ' %b'>>
<< intercepted strftime(): result: ' Feb'>>
Feb 2009 23:31:30 +0000
```



#### `./autoldpre.sh --resolve-strip-suffix <invoked_as> <suffix> [--] <command> [<args>]`

This variant is meant to simplify making wrapper shell scripts.

Example: [`example.strftime.sh`](example.strftime.sh)

```text
$ ./example.strftime.sh -uRd @1234567890
<< intercepted strftime(): format: ' %a'>>
<< intercepted strftime(): result: ' Fri'>>
Fri, 13 << intercepted strftime(): format: ' %b'>>
<< intercepted strftime(): result: ' Feb'>>
Feb 2009 23:31:30 +0000
```


<!--#toc stop="scan" -->



Known issues
------------

* Needs more/better tests and docs.




&nbsp;


License
-------
<!--#echo json="package.json" key=".license" -->
ISC
<!--/#echo -->
