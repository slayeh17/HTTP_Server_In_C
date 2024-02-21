FROM gcc:latest
COPY . /usr/src/chttpserver
WORKDIR /usr/src/chttpserver
EXPOSE 6969
RUN gcc -o server server.c
CMD [ "./server" ]