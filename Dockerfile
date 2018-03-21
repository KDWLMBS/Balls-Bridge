FROM alpine
COPY . /app
RUN cd /app \
 && apk add --no-cache gcc g++ musl-dev cmake make \
 && cmake . \
 && make bridge

ENTRYPOINT /app/bridge