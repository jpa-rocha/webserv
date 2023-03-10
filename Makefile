################################################################################
################################ COMPILER & FLAGS ##############################
################################################################################

CC =		g++

CFLAGS	=	-Wall -Wextra -Werror -Wshadow -std=c++98

DEBUG =		-Wall -Wextra -Werror -g -std=c++98 

VAL =		valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes

################################################################################
################################### LIBRARIES ##################################
################################################################################

FT_INC	= 		-I ./include/
				
INCLUDES = 		$(FT_INC)

SRCDIR =		./src/

################################################################################
################################## SRCS & SRCS #################################
################################################################################

SRCS	=	$(SRCDIR)Config.cpp			\
			$(SRCDIR)ConfigParser.cpp	\
			$(SRCDIR)Location.cpp		\
			$(SRCDIR)configCGI.cpp		\
			$(SRCDIR)httpHeader.cpp		\
			$(SRCDIR)Server.cpp			\
			$(SRCDIR)ServerManager.cpp	\
			$(SRCDIR)Response.cpp		\
			$(SRCDIR)MIME.cpp			\
			$(SRCDIR)CGI.cpp			\
			$(SRCDIR)minilib.cpp		\
			$(SRCDIR)main.cpp			\

################################################################################
#################################### PROGRAM ###################################
################################################################################

EXEC =		webserv

RUN =		./webserv

################################################################################
#################################### RULES #####################################
################################################################################

all:		$(EXEC)

$(EXEC):	$(SRCS)
		$(CC) $(SRCS) $(INCLUDES) -o $(EXEC) $(CFLAGS)

debug:	$(SRCS)
		$(CC) $(SRCS) $(INCLUDES) -o $(EXEC) $(DEBUG)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -g $(SRCS)

clean:
		rm -f $(EXEC)

fclean:		clean
		rm -f $(EXEC)

re:			fclean all


launch: re debug
	$(RUN)

test: debug
	$(VAL) $(RUN)

.PHONY: all debug clean fclean re test