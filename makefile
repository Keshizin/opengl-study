# Caminho dos diretórios
#INC_DIR=include
OBJ_DIR=obj
SRC_DIR=src
BIN_DIR=bin
TMP_DIR=tmp
LIB_DIR=lib
INC_DIR=include

# Flags que devem ser passados para o compilador
OUTPUT_NAME=myopengl.exe
CPPSOURCES=$(wildcard $(SRC_DIR)/*.cpp)
OBJFILES=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPPSOURCES))
DFILES=$(patsubst $(SRC_DIR)/%.cpp,$(TMP_DIR)/%.d,$(CPPSOURCES))

INC_FLAGS=-I$(INC_DIR)
LIB_FLAGS=-lgdi32 -lopengl32 -lglu32 -ljpeg
DIRLIB_FLAG=-L$(LIB_DIR)/jpeg

all: $(BIN_DIR)/$(OUTPUT_NAME)

# Linkedição (.exe)
$(BIN_DIR)/$(OUTPUT_NAME): $(OBJFILES)
	@echo . Gerando executavel final: $@
	@g++ $^ -o $@ $(DIRLIB_FLAG) $(LIB_FLAGS) -Wall

# Compilação (.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo . Compilando $<
	@g++ -c $< $(INC_FLAGS) -o $@

clean-exe:
	@echo . Deletando o executavel
# Para o Microsoft Windows!
	del $(BIN_DIR)\$(OUTPUT_NAME)
# Para o UNIX!
#	rm bin/teste.exe

clean-obj:
	@echo . Deletando codigo-objetos
# Para o Microsoft Windows!
	del $(OBJ_DIR)\*.o
# Para o UNIX!
#	rm obj/*.o

clean-d:
	@echo . Deletando makefiles temporarios
# Para o Microsoft Windows!
	del $(TMP_DIR)\*.d
# Para o UNIX!
#	rm $(TMP_DIR)/*

clean-all:
	@echo . Limpando tudo
	$(MAKE) clean-exe
	$(MAKE) clean-obj
	$(MAKE) clean-d

# remade:
# 	@echo REMADE
# 	$(MAKE) clean-all
# 	$(MAKE) all

-include $(DFILES)

$(TMP_DIR)/%.d: $(SRC_DIR)/%.cpp
	@echo . Gerando arquivos .d (dependencias - GCC) $<
	@g++ -c $< -MM -MT 'obj/$*.o temp/$*.d ' -MD $(INC_FLAGS) -o $@

var-teste:
	@echo $(OBJ_DIR)
	@echo $(SRC_DIR)
	@echo $(BIN_DIR)
	@echo $(TMP_DIR)
	@echo $(OUTPUT_NAME)
	@echo $(CPPSOURCES)
	@echo $(OBJFILES)
	@echo $(DFILES)