# Menggunakan image Go resmi
FROM golang:1.23-alpine

# Menambahkan dependency tools
RUN apk add --no-cache git

# Set working directory
WORKDIR /app

# Copy semua file yang diperlukan ke dalam container
COPY go.mod go.sum ./

COPY . .
RUN go mod download

# Build aplikasi
RUN go build -o main ./src/main.go

# Ekspor port default
EXPOSE 8080

# Menjalankan aplikasi
CMD ["./main"]
