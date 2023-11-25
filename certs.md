# How to create the certs
mkcert -install -key-file key.pem -cert-file cert.pem example.com *.example.com

Location of rootCA.pem --> /home/angelo/.local/share/mkcert/