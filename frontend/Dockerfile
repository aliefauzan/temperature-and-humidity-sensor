# Gunakan image Nginx sebagai base image
FROM nginx:alpine

# Copy file HTML, CSS, dan JS ke dalam direktori Nginx
COPY . /usr/share/nginx/html

# Expose port 8080 untuk Cloud Run
EXPOSE 8080

# Ganti konfigurasi default Nginx agar berjalan di port 8080
RUN sed -i 's/listen       80;/listen 8080;/g' /etc/nginx/conf.d/default.conf

# Jalankan Nginx
CMD ["nginx", "-g", "daemon off;"]
