# JsonSanitizer

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>
    Serge Zaitsev <zaitsev.serge@gmail.com>
    Benoît Blanchon <http://blog.benoitblanchon.fr>

License:

    BSD

This library uses Serge Zaitsev's
[jsmn code](https://github.com/zserge/jsmn) and some code from Benoît
Blanchon's [ArduinoJson](https://github.com/bblanchon/ArduinoJson) to
sanitize erroneous, but interpretable JSON strings.

Valid compact JSON is unchanged.

```json
json_valid:
{"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
sanitized:
{"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
```

Unquoted strings get quoted and spaces between array elements get converted to commas.

```json
json_partial_valid:
{"sensor":gps,test:null,truth:true,method:?,"time":1351824120,"data":[48.756080 2.302038]}
sanitized:
{"sensor":"gps","test":null,"truth":true,"method":"?","time":1351824120,"data":[48.756080,2.302038]}
```

The root of every JSON string should either be an object or array. The
outermost brackets will be added to a JSON array missing them.

```json
json_unsanitized_array:
   ? test, 1  ??   null true [1,2 3] "nice string" weird_\"string
sanitized:
["?","test",1,"??",null,true,[1,2,3],"nice string","weird_\"string"]
```

The outermost braces will be added to a JSON object missing them, if
colons separate keys from values and commas separate key/value pairs.

```json
json_unsanitized_object:
  server : example.com, post: 80, message: "hello world"
sanitized:
{"server":"example.com","post":80,"message":"hello world"}
```

Some JSON strings are too ambiguous to be sanitized correctly. Without
commas separating the key/value pairs, the parser gets confused and
does not sanitize properly.

```json
json_unsanitizable_object:
  server : example.com post: 80 message: "hello world"
unsanitized:
{"server":"example.com"}{"server":"example.com"}{"server":}
```

C++ style comments are removed when preceding JSON wrapped in brackets
or braces.

```json
json_partial_valid_with_cpp_comments:
// this is a c++ style comment {"sensor":gps,test:null,truth:true,method:?,"time":1351824120,"data":[48.756080 2.302038]}
sanitized:
{"sensor":"gps","test":null,"truth":true,"method":"?","time":1351824120,"data":[48.756080,2.302038]}
```

C style comments are removed even when preceding unsanitized JSON.

```json
json_unsanitized_with_c_comments:
/* this is a c style comment */ ? test, 1  ??   null true [1,2 3] "nice string" weird_\"string
sanitized:
["?","test",1,"??",null,true,[1,2,3],"nice string","weird_\"string"]
```

[Usage Examples](./examples)

## Install Library Dependencies

[arduino-libraries](https://github.com/janelia-arduino/arduino-libraries)
