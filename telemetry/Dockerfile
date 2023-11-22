FROM node:18.17.0

# Create app directory
WORKDIR /usr/src/app

# Install dependencies
COPY package.json ./
COPY packages/constants/package.json ./packages/constants/
COPY packages/server/package.json ./packages/server/
COPY packages/types/package.json ./packages/types/
COPY packages/ui/package.json ./packages/ui/
COPY yarn.lock ./
COPY patches ./patches
RUN yarn install

# Expose ports
EXPOSE 5173
EXPOSE 3000

# Entrypoint
COPY entry.sh /usr/src/app/entry.sh
RUN chmod +x /usr/src/app/entry.sh

ENTRYPOINT [ "/usr/src/app/entry.sh" ]
