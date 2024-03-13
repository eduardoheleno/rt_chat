FROM node:21.6.2

WORKDIR /usr/src/app

RUN npm i -g @nestjs/cli

COPY package*.json .

RUN npm install

COPY . .

CMD ["nest", "start", "--watch"]
