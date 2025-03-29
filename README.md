
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
<< intercepted strftime(): original result: ' Fri', modified result: 'ABCD' >>
        ^0      example.strftime.so     strftime        +0x14e  @0x7f3c6830ff37
        ^1      date    ?       +0x52c4 @0x5622d62f92c4
        ^2      date    ?       +0x69fe @0x5622d62fa9fe
        ^3      date    ?       +0x4333 @0x5622d62f8333
        ^4      date    ?       +0x3dbc @0x5622d62f7dbc
        ^5      libc.so.6       __libc_start_main       +0xf3   @0x7f3c68110083
        ^6      date    ?       +0x414e @0x5622d62f814e
        ^=      strftime < ? < ? < ? < ? < __libc_start_main < ?
<< intercepted strftime(): format: ' %b'>>
<< intercepted strftime(): original result: ' Feb', modified result: 'FGHI' >>
BCD, 13 GHI 2009 23:31:30 +0000
```

This demonstrates that the `date` command really requests a space character
at the start of the string part and then discards that first character.



#### `./autoldpre.sh --resolve-strip-suffix <invoked_as> <suffix> [--] <command> [<args>]`

This variant is meant to simplify making wrapper shell scripts.

Example: [`example.strftime.sh`](example.strftime.sh)

```text
$ ./example.strftime.sh -uRd @1234567890
# [… same output as above, except for memory offsets maybe …]
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
