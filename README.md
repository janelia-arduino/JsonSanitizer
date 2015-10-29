#JsonSanitizer

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>
    Serge Zaitsev <zaitsev.serge@gmail.com>
    Benoît Blanchon <http://blog.benoitblanchon.fr>

License:

    BSD

```json
json_valid:
{"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
sanitized:
{"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
```

```json
json_partial_valid:
{"sensor":gps,test:null,truth:true,method:?,"time":1351824120,"data":[48.756080,2.302038]}
sanitized:
{"sensor":"gps","test":null,"truth":true,"method":"?","time":1351824120,"data":[48.756080,2.302038]}
```

```json
json_unsanitized_array:
   ? test, 1  ??   null true [1,2 3] "nice string" weird_\"string
sanitized:
["?","test",1,"??",null,true,[1,2,3],"nice string","weird_\"string"]
```

```json
json_unsanitized_object:
  server : example.com, post: 80, message: "hello world"
sanitized:
{"server":"example.com","post":80,"message":"hello world"}
```

```json
json_valid_request:
["setSerialNumber","serial_number","?"]
sanitized:
["setSerialNumber","serial_number","?"]
```

[Usage Examples](./examples)

