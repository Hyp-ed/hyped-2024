const io = require('socket.io-client');

// URL of the Socket.io server you want to connect to
const serverURL = 'http://localhost:3000'; // Change this to your server's URL

// Connect to the Socket.io server
const socket = io(serverURL, {
  path: '/live-logs',
  transports: ['websocket'],
  debug: true,
});

// Event handler for successful connection
socket.on('connect', () => {
  console.log('Connected to the Socket.io server');

  // subscribe
  socket.emit('subscribe', 'subscribe-to-logs');
});

// Event handler for receiving messages from the server
socket.on('log', (message) => {
  console.log(`Received message: ${message}`);
});

// Event handler for connection errors
socket.on('connect_error', (error) => {
  console.error(`Socket.io connection error: ${error}`);
});

// Event handler for disconnection
socket.on('disconnect', (reason) => {
  console.log(`Disconnected from the Socket.io server. Reason: ${reason}`);
});
