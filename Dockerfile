FROM ubuntu:20.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source files
COPY . .

# Compile the web server
RUN gcc -o web_server web_server.c

# Expose port (Render will set PORT environment variable)
EXPOSE $PORT

# Run the web server
CMD ["./web_server"]