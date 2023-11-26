# Nginx configuration for reverse proxy

server {
    listen 443 ssl;
    server_name localhost;  # Use your local development domain or localhost

    ssl_certificate /home/angelo/projects/CloudVigilante/certs/cert.pem;  # Path to your SSL certificate
    ssl_certificate_key /home/angelo/projects/CloudVigilante/certs/key.pem;  # Path to your SSL certificate key

    location / {
        proxy_pass https://localhost:8080;  # Forward requests to your Bun server
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "upgrade";
        proxy_set_header Host $host;
    }
}
