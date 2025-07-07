# Makefile pro kalkulačku
# Kompiler a flagy
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = kalkulacka
SOURCES = kalkulacka.c
HEADERS = seznam.h

# Hlavní pravidlo - sestaví program
all: $(TARGET)

# Sestavení spustitelného souboru
$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

# Spuštění programu
run: $(TARGET)
	./$(TARGET)

# Test s konkrétním příkladem
test: $(TARGET)
	@echo "Test 1: 56+89"
	@printf "56+89\n" | ./$(TARGET)
	@echo ""
	@echo "Test 2: 123*45"
	@printf "123*45\n" | ./$(TARGET)
	@echo ""
	@echo "Test 3: 10-5+3"
	@printf "10-5+3\n" | ./$(TARGET)

# Čištění - smaže spustitelný soubor
clean:
	rm -f $(TARGET)

# Debug verze s více informacemi
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Verze pro release (optimalizovaná)
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# Kontrola paměti pomocí valgrind (pokud je nainstalován)
memcheck: $(TARGET)
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "Spouštím kontrolu paměti..."; \
		printf "56+89\n" | valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET); \
	else \
		echo "Valgrind není nainstalován. Instalace: sudo apt-get install valgrind"; \
	fi

# Informace o projektu
info:
	@echo "=== Informace o projektu kalkulačka ==="
	@echo "Zdrojové soubory: $(SOURCES)"
	@echo "Hlavičkové soubory: $(HEADERS)"
	@echo "Cíl: $(TARGET)"
	@echo "Kompiler: $(CC)"
	@echo "Flagy: $(CFLAGS)"
	@echo ""
	@echo "Dostupné příkazy:"
	@echo "  make         - sestaví program"
	@echo "  make run     - spustí program"
	@echo "  make test    - spustí testy"
	@echo "  make clean   - smaže spustitelný soubor"
	@echo "  make debug   - sestaví debug verzi"
	@echo "  make release - sestaví optimalizovanou verzi"
	@echo "  make memcheck- kontrola paměti (vyžaduje valgrind)"
	@echo "  make help    - zobrazí tuto nápovědu"

# Alias pro info
help: info

# Phony targets (cíle, které nejsou soubory)
.PHONY: all run test clean debug release memcheck info help
