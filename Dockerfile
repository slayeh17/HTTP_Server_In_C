FROM gcc:latest
COPY . /usr/src/debakerserver
WORKDIR /usr/src/debakerserver
RUN gcc -o debakerserver server.c
EXPOSE 6969
CMD ["./debakerserver"]
