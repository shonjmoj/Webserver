CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98 -g -fsanitize=address

NAME = webServer

SRC = main.cpp utils.cpp configfile/configFile.cpp configfile/HttpStatusPars.cpp configfile/location.cpp configfile/serverData.cpp \
		HttpRequest/Content.cpp HttpRequest/Delete.cpp HttpRequest/html.cpp HttpRequest/HttpRequest.cpp HttpRequest/HttpRequestParse.cpp \
		HttpRequest/HttpResponse.cpp HttpRequest/MatchingLocation.cpp HttpRequest/Methods.cpp \
		server/HttpServer.cpp server/Server.cpp


HDR = utils.hpp configfile/configFile.hpp configfile/HttpStatusPars.hpp configfile/location.hpp configfile/serverData.hpp \
			HttpRequest/HttpRequest.hpp server/HttpServer.hpp server/Server.hpp

OBJ = $(SRC:.cpp=.o)


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o:%.cpp $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY : re fclean clean all
