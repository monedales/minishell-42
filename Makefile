# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mona <mona@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/21 00:00:00 by mona              #+#    #+#              #
#    Updated: 2026/03/28 16:10:52 by mona             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Libft
LIBFT_DIR = libraries/libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I include -I $(LIBFT_DIR)

# On macOS, Homebrew's GNU readline must be used explicitly because the
# system ships libedit instead, which lacks rl_replace_line and other symbols.
# On Linux (42 school), the default -lreadline links to full GNU readline.
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    READLINE_PREFIX := $(shell brew --prefix readline 2>/dev/null)
    INCLUDES += -I$(READLINE_PREFIX)/include
    LIBS = -L$(READLINE_PREFIX)/lib -lreadline -L$(LIBFT_DIR) -lft
else
    LIBS = -lreadline -L$(LIBFT_DIR) -lft
endif

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
			  expander_utils.c \
              parser.c \
              parser_utils.c \
              parser_free.c \
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
           redirections.c \
           redirections_utils.c

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

# Diretórios necessários
OBJ_SUBDIRS = $(OBJ_DIR)/parsing/ $(OBJ_DIR)/env $(OBJ_DIR)/execution $(OBJ_DIR)/builtins $(OBJ_DIR)/signals $(OBJ_DIR)/utils

# Garante que todos os subdiretórios existem antes de compilar qualquer objeto
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR) $(OBJ_SUBDIRS)

$(OBJ_SUBDIRS):
	@mkdir -p $@

# Main
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Parsing
$(OBJ_DIR)/parsing/%.o: $(PARSING_DIR)/%.c | $(OBJ_DIR)/parsing
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Environment
$(OBJ_DIR)/env/%.o: $(ENV_DIR)/%.c | $(OBJ_DIR)/env
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Execution
$(OBJ_DIR)/execution/%.o: $(EXEC_DIR)/%.c | $(OBJ_DIR)/execution
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Builtins
$(OBJ_DIR)/builtins/%.o: $(BUILTIN_DIR)/%.c | $(OBJ_DIR)/builtins
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Signals
$(OBJ_DIR)/signals/%.o: $(SIGNALS_DIR)/%.c | $(OBJ_DIR)/signals
	@echo "🔨 Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Utils
$(OBJ_DIR)/utils/%.o: $(UTILS_DIR)/%.c | $(OBJ_DIR)/utils
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

valgrind: $(NAME)
	@echo "🔍 Running valgrind..."
	@valgrind --suppressions=include/valgrind.sup --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

.PHONY: all clean fclean re norm test valgrind test_env
