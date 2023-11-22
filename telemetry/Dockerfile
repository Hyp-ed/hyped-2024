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

# Copy all source files
COPY . .

# Replace server/ui .env with .env.docker
RUN cp packages/server/.env.docker packages/server/.env
RUN cp packages/ui/.env.docker packages/ui/.env

# Build app
RUN yarn build

# Expose ports
EXPOSE 5173
EXPOSE 3000

# Run the yarn script specified in the YARN_SCRIPT environment variable
CMD ["sh", "-c", "yarn \"$YARN_SCRIPT\""]
