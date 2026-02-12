# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mona <mona@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/21 00:00:00 by mona              #+#    #+#              #
#    Updated: 2026/02/12 17:45:51 by mona             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Libft
LIBFT_DIR = libraries/libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I include -I $(LIBFT_DIR)
LIBS = -lreadline -L$(LIBFT_DIR) -lft

# ============================================================================ #
#                                DIRECTORIES                                   #
# ============================================================================ #

SRC_DIR = src
OBJ_DIR = obj

PARSING_DIR = $(SRC_DIR)/parsing
EXEC_DIR = $(SRC_DIR)/execution
BUILTIN_DIR = $(SRC_DIR)/builtins
ENV_DIR = $(SRC_DIR)/env
UTILS_DIR = $(SRC_DIR)/utils
SIGNALS_DIR = $(SRC_DIR)/signals

# ============================================================================ #
#                              SOURCE FILES                                    #
# ============================================================================ #

# Main
MAIN_SRC = main.c

# Parsing (Pessoa A)
PARSING_SRC = lexer.c \
              expander.c \
              parser.c \
              quotes.c \
			  tokens.c

# Environment (Pessoa A)
ENV_SRC = env_init.c \
          env_get.c \
          env_set.c \
          env_unset.c \
          env_utils.c

# Execution (Pessoa B)
EXEC_SRC = executor.c \
           executor_simple.c \
           executor_pipeline.c \
           path_finder.c \
           redirections.c

# Builtins (Pessoa B)
BUILTIN_SRC = builtin_checker.c \
              builtin_echo.c \
              builtin_cd.c \
              builtin_pwd.c \
              builtin_export.c \
              builtin_unset.c \
              builtin_env.c \
              builtin_exit.c

# Signals (Pessoa B)
SIGNALS_SRC = signals.c

# Utils (Ambos)
UTILS_SRC = error_utils.c \
            free_utils.c

# ============================================================================ #
#                              OBJECT FILES                                    #
# ============================================================================ #

MAIN_OBJ = $(addprefix $(OBJ_DIR)/, $(MAIN_SRC:.c=.o))
PARSING_OBJ = $(addprefix $(OBJ_DIR)/parsing/, $(PARSING_SRC:.c=.o))
ENV_OBJ = $(addprefix $(OBJ_DIR)/env/, $(ENV_SRC:.c=.o))
EXEC_OBJ = $(addprefix $(OBJ_DIR)/execution/, $(EXEC_SRC:.c=.o))
BUILTIN_OBJ = $(addprefix $(OBJ_DIR)/builtins/, $(BUILTIN_SRC:.c=.o))
SIGNALS_OBJ = $(addprefix $(OBJ_DIR)/signals/, $(SIGNALS_SRC:.c=.o))
UTILS_OBJ = $(addprefix $(OBJ_DIR)/utils/, $(UTILS_SRC:.c=.o))

ALL_OBJ = $(MAIN_OBJ) $(PARSING_OBJ) $(ENV_OBJ) $(EXEC_OBJ) \
          $(BUILTIN_OBJ) $(SIGNALS_OBJ) $(UTILS_OBJ)

# Minimal objects for testing env and lexer only
TEST_SRC = test_main.c
TEST_OBJ = $(addprefix $(OBJ_DIR)/, $(TEST_SRC:.c=.o))
TEST_ENV_OBJ = $(TEST_OBJ) $(ENV_OBJ) $(addprefix $(OBJ_DIR)/parsing/, lexer.o tokens.o)

# ============================================================================ #
#                                  RULES                                       #
# ============================================================================ #

all: $(NAME)

$(LIBFT):
	@echo "📚 Compiling libft..."
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(ALL_OBJ)
	@echo "🔗 Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(ALL_OBJ) $(LIBS) -o $(NAME)
	@echo "✅ $(NAME) created successfully!"

# Main
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Parsing
$(OBJ_DIR)/parsing/%.o: $(PARSING_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/parsing
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Environment
$(OBJ_DIR)/env/%.o: $(ENV_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/env
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Execution
$(OBJ_DIR)/execution/%.o: $(EXEC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/execution
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Builtins
$(OBJ_DIR)/builtins/%.o: $(BUILTIN_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/builtins
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Signals
$(OBJ_DIR)/signals/%.o: $(SIGNALS_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/signals
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Utils
$(OBJ_DIR)/utils/%.o: $(UTILS_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/utils
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "🧹 Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@echo "✅ Clean complete!"

fclean: clean
	@echo "🧹 Removing $(NAME)..."
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "✅ Full clean complete!"

re: fclean all

# ============================================================================ #
#                                 UTILITIES                                    #
# ============================================================================ #

norm:
	@echo "📏 Running norminette..."
	@norminette include/ src/

test: $(NAME)
	@echo "🧪 Running basic tests..."
	@./$(NAME)

valgrind: $(NAME)
	@echo "🔍 Running valgrind..."
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

# Target para testar apenas env (para desenvolvimento incremental)
test_env: $(LIBFT) $(TEST_ENV_OBJ)
	@echo "🔗 Linking test_env..."
	@$(CC) $(CFLAGS) $(TEST_ENV_OBJ) $(LIBS) -o test_env
	@echo "✅ test_env created! Run with: ./test_env"

.PHONY: all clean fclean re norm test valgrind test_env
