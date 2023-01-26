miscellaneous programs
======

### Setup Web Server

If use WebAssembly, the following web server sends `application/wasm` as a correct response.

```bash
$ python3 server.py
```

### ALSA

Install ALSA libraries on Ubuntu (or others Linux distributions).

```bash
$ sudo apt -y install libasound2-dev
$ sudo apt -y install libflac-dev
$ sudo apt -y install libfltk1.3-dev
$ sudo apt -y install libsndfile1-dev
```

### CASL Ⅱ Online Simulator

https://www.officedaytime.com/dcaslj/

### Create certificates for TLS

```bash
$ cp /etc/ssl/openssl.cnf ./  # if macOS
$ vim openssl.cnf


# append the below:

[ CA ]
basicConstraints=critical,CA:TRUE,pathlen:0
keyUsage=digitalSignature,keyCertSign,cRLSign

[ Server ]
basicConstraints=CA:FALSE
keyUsage=digitalSignature,dataEncipherment
extendedKeyUsage=serverAuth

[ Client ]
basicConstraints=CA:FALSE
keyUsage=digitalSignature,dataEncipherment
extendedKeyUsage=clientAuth


# Create root certificate
$ openssl genrsa -out ca.key 2048
$ openssl req -new -sha256 -key ca.key -out ca.csr -config openssl.cnf
$ openssl x509 -in ca.csr -days 365 -req -signkey ca.key -sha256 -out ca.crt -extfile ./openssl.cnf -extensions CA

# Confirm
$ openssl rsa -in ca.key -text
$ openssl req -in ca.csr -text
$ openssl x509 -in ca.crt -text

# Create server certificate
$ openssl genrsa -out server.key 2048
$ openssl req -new -nodes -sha256 -key server.key -out server.csr -config openssl.cnf
$ openssl x509 -req -days 365 -in server.csr -sha256 -out server.crt -CA ca.crt -CAkey ca.key -CAcreateserial -extfile ./openssl.cnf -extensions Server
```
