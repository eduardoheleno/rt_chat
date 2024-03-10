FROM node:20.11.1-alpine3.19

WORKDIR /usr/src/app

RUN npm i -g @nestjs/cli

COPY package*.json .

RUN npm install

COPY . .

CMD ["nest", "start", "--watch"]
