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
	@echo "Test 1: 56+89=145"
	@printf "56+89\n" | ./$(TARGET)
	@echo ""
	@echo "Test 2: 5*(5+5)=50"
	@printf "5*(5+5)\n" | ./$(TARGET)
	@echo ""
	@echo "Test 3: 5.5*(0.2*3)/7+5-9*(5+(5+(5+(8*(7/3)+5.5))))=-347.028571429"
	@printf "5.5*(0.2*3)/7+5-9*(5+(5+(5+(8*(7/3)+5.5))))\n" | ./$(TARGET)

# Rozšířené testy s velmi složitými příklady
complextest: $(TARGET)
	@echo "=== KOMPLEXNÍ TESTY KALKULAČKY ==="
	@echo ""
	@echo "Test 1: Extrémně vnořené závorky"
	@echo "Příklad: ((((5+3)*2)/4)+((7-2)*3))*(((9+1)/2)-1)"
	@echo "Očekávaný výsledek: 76"
	@printf "((((5+3)*2)/4)+((7-2)*3))*(((9+1)/2)-1)\n" | ./$(TARGET)
	@echo ""
	@echo "Test 2: Mnoho úrovní s desetinnými čísly"
	@echo "Příklad: (((12.5*2.4)/(3.2+1.8))+((8.7-3.2)*1.5))/(((15.6/2.6)+2.4)*0.8)"
	@echo "Očekávaný výsledek: ≈ 2.12053571429"
	@printf "(((12.5*2.4)/(3.2+1.8))+((8.7-3.2)*1.5))/(((15.6/2.6)+2.4)*0.8)\n" | ./$(TARGET)
	@echo ""
	@echo "Test 3: Velmi dlouhý výraz bez závorek"
	@echo "Příklad: 123.45+67.89*2.1-45.67/3.4+89.12*1.23-56.78+34.56/2.1*3.4-12.34"
	@echo "Očekávaný výsledek: ≈ 349.038532773"
	@printf "123.45+67.89*2.1-45.67/3.4+89.12*1.23-56.78+34.56/2.1*3.4-12.34\n" | ./$(TARGET)
	@echo ""
	@echo "Test 4: Kombinace všech operací s hlubokými závorkami"
	@echo "Příklad: (((((100-50)*2)+25)/5)*((6+4)/(2*1)))+((((20*3)/4)+5)*((8-3)*(2+1)))"
	@echo "Očekávaný výsledek: 425"
	@printf "(((((100-50)*2)+25)/5)*((6+4)/(2*1)))+((((20*3)/4)+5)*((8-3)*(2+1)))\n" | ./$(TARGET)
	@echo ""
	@echo "Test 5: Test s malými desetinnými čísly"
	@echo "Příklad: ((0.1+0.2)*0.3)+(((0.4-0.1)*0.5)/((0.6+0.1)*0.2))"
	@echo "Očekávaný výsledek: 1.16142857143"
	@printf "((0.1+0.2)*0.3)+(((0.4-0.1)*0.5)/((0.6+0.1)*0.2))\n" | ./$(TARGET)
	@echo ""
	@echo "=== KONEC KOMPLEXNÍCH TESTŮ ==="

# Stress test - extrémně složité příklady
stresstest: $(TARGET)
	@echo "=== STRESS TEST ==="
	@echo "Extrémně dlouhý a složitý výraz:"
	@echo "Očekávaný výsledek: ≈ 4.21948559979"
	@printf "(((((12.34*5.67)/(8.9+1.23))*((4.56-2.34)*3.21))+((7.89/2.1)*((5.43+3.21)*0.87)))*((((9.87-4.32)*2.1)/(3.45+1.23))+((6.78*1.23)/(4.56-2.34))))/(((((15.6+8.7)*0.34)/(2.1*3.4))+((12.5-7.8)*1.67))*((((9.8+2.3)*0.45)/(1.2+3.4))+((8.7-3.2)*2.1)))\n" | ./$(TARGET)
	@echo ""
	@echo "=== KONEC STRESS TESTU ==="

# Čištění - smaže spustitelný soubor
clean:
	rm -f $(TARGET)

# Debug verze s více informacemi
debug: CFLAGS += -DDEBUG -O0
debug: clean $(TARGET)

# Verze pro release (optimalizovaná)
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# Kontrola paměti pomocí valgrind (pokud je nainstalován)
memcheck: $(TARGET)
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "Spouštím kontrolu paměti..."; \
		printf "(((((12.34*5.67)/(8.9+1.23))*((4.56-2.34)*3.21))+((7.89/2.1)*((5.43+3.21)*0.87)))*((((9.87-4.32)*2.1)/(3.45+1.23))+((6.78*1.23)/(4.56-2.34))))/(((((15.6+8.7)*0.34)/(2.1*3.4))+((12.5-7.8)*1.67))*((((9.8+2.3)*0.45)/(1.2+3.4))+((8.7-3.2)*2.1)))\n" | valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET); \
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
