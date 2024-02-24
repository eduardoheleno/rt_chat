FROM rust:latest

WORKDIR /usr/src/app

RUN cargo install cargo-watch

COPY . .

CMD ["cargo", "watch", "-c", "-w", "src", "-x", "run"]
