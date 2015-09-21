NAME		= mod1
NAME_DEBUG	= mod1_debug

SRC		=	main.cpp

HEAD_DIR	= include
SRC_DIR		= src
DEBUG_DIR	= debug
STATIC_DIR	= static
DEP_DIR		= dep


INCLUDES	=
LIBS		= -framework OpenGL

OBJ				= $(patsubst %.cpp,$(STATIC_DIR)/%.o,$(SRC))
OBJ_DEBUG		= $(patsubst %.cpp,$(DEBUG_DIR)/%.o,$(SRC))
DEPS			= $(patsubst %.cpp,$(DEP_DIR)/%.d,$(SRC))

OPTI		= -O3
CXXFLAGS	= -std=c++11 -Wall #-Wextra -Wno-unused-result
DEPENDS 	= -MT $@ -MD -MP -MF $(subst .o,.d,$@)


$(shell mkdir -p $(STATIC_DIR) $(DEBUG_DIR) $(DEP_DIR))


.PHONY: clean fclean re debug

all: $(NAME)
	@echo "Compilation terminee. (realease)"

$(NAME): $(OBJ)
	$(CXX) $(OPTI) $(CXXFLAGS) -I $(HEAD_DIR) $(INCLUDES) -o $@ $(OBJ) $(LIBS)

debug: $(OBJ_DEBUG)
	$(CXX) $(OPTI) $(CXXFLAGS) -I $(HEAD_DIR) $(INCLUDES) -o $(NAME_DEBUG) $(OBJ_DEBUG) $(LIBS) -g

-include $(OBJ:.o=.d)

$(STATIC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(OPTI) $(CXXFLAGS) $(DEPENDS) -I $(HEAD_DIR) $(INCLUDES) -o $@ -c $<

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(OPTI) $(CXXFLAGS) $(DEPENDS) -I $(HEAD_DIR) $(INCLUDES) -o $@ -c $< -g

clean:
	rm -f $(OBJ) $(OBJ_DEBUG) $(DEPS)

fclean: clean
	rm -f $(NAME) $(NAME_DEBUG)

re: fclean
	make
