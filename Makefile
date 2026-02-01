# Priority Social Media - Glassmorphic Edition
# High-Performance C Backend with WebView Frontend

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LIBS = -ljson-c -lwebkit2gtk-4.0 -lgtk-3.0 $(shell pkg-config --cflags --libs webkit2gtk-4.0 gtk+-3.0)
TARGET = priority_social_media
SOURCES = main.c
ASSETS = working_social_media.html style.css

# Default target
all: $(TARGET)

# Compile the main executable
$(TARGET): $(SOURCES)
	@echo "🔨 Building Priority Social Media..."
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIBS)
	@echo "✅ Build complete!"

# Install dependencies (Ubuntu/Debian)
install-deps:
	@echo "📦 Installing dependencies..."
	sudo apt-get update
	sudo apt-get install -y build-essential libwebkit2gtk-4.0-dev libgtk-3-dev libjson-c-dev pkg-config
	@echo "✅ Dependencies installed!"

# Install dependencies (Fedora/RHEL)
install-deps-fedora:
	@echo "📦 Installing dependencies (Fedora)..."
	sudo dnf install -y gcc webkit2gtk3-devel gtk3-devel json-c-devel pkgconfig
	@echo "✅ Dependencies installed!"

# Install dependencies (macOS)
install-deps-macos:
	@echo "📦 Installing dependencies (macOS)..."
	brew install webkit2gtk gtk+3 json-c pkg-config
	@echo "✅ Dependencies installed!"

# Run the application
run: $(TARGET)
	@echo "🚀 Starting Priority Social Media..."
	./$(TARGET)

# Clean build artifacts
clean:
	@echo "🧹 Cleaning build artifacts..."
	rm -f $(TARGET) *.o app_state.dat
	@echo "✅ Clean complete!"

# Package for distribution
package: $(TARGET)
	@echo "📦 Creating distribution package..."
	mkdir -p dist/priority_social_media
	cp $(TARGET) dist/priority_social_media/
	cp $(ASSETS) dist/priority_social_media/
	cp README.md dist/priority_social_media/ 2>/dev/null || echo "README.md not found, skipping..."
	cd dist && tar -czf priority_social_media.tar.gz priority_social_media/
	@echo "✅ Package created: dist/priority_social_media.tar.gz"

# Development mode with debug symbols
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)
	@echo "🐛 Debug build complete!"

# Performance optimized build
release: CFLAGS += -O3 -DNDEBUG -march=native
release: $(TARGET)
	@echo "⚡ Release build complete!"

# Memory check with Valgrind
memcheck: debug
	@echo "🔍 Running memory check..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Code formatting
format:
	@echo "🎨 Formatting code..."
	clang-format -i $(SOURCES)
	@echo "✅ Code formatted!"

# Static analysis
analyze:
	@echo "🔍 Running static analysis..."
	cppcheck --enable=all $(SOURCES)
	@echo "✅ Analysis complete!"

# Help target
help:
	@echo "🚀 Priority Social Media - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all              - Build the application (default)"
	@echo "  install-deps     - Install dependencies (Ubuntu/Debian)"
	@echo "  install-deps-fedora - Install dependencies (Fedora/RHEL)"
	@echo "  install-deps-macos  - Install dependencies (macOS)"
	@echo "  run              - Build and run the application"
	@echo "  clean            - Remove build artifacts"
	@echo "  package          - Create distribution package"
	@echo "  debug            - Build with debug symbols"
	@echo "  release          - Build optimized release version"
	@echo "  memcheck         - Run memory leak detection"
	@echo "  format           - Format source code"
	@echo "  analyze          - Run static code analysis"
	@echo "  help             - Show this help message"

# Phony targets
.PHONY: all install-deps install-deps-fedora install-deps-macos run clean package debug release memcheck format analyze help