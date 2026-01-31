FROM ubuntu:20.04

# Install build dependencies
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    libgtk-3-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source files
COPY . .

# Compile the application
RUN gcc -o social_media_app social_media_complete.c `pkg-config --cflags --libs gtk+-3.0` || \
    gcc -o social_media_app priority_social_media.c || \
    gcc -o social_media_app frontend.c

# Expose port (adjust as needed)
EXPOSE 8080

# Run the application
CMD ["./social_media_app"]