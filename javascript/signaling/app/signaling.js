'use strict';

const WebSocket = require('ws');
const WebSocketServer = WebSocket.Server;
const port = (process.argv[2] > 0) && (process.argv[2] <= 65535) ? process.argv[2] : 3000;

const ws = new WebSocketServer({ port });

console.log(`Wait port (${port}) ...`);

ws.on('connection', (socket) => {
  socket.on('message', (message) => {
    const stringMessage = new Buffer(message).toString('utf8');

    ws.clients.forEach((client) => {
      if ((socket !== client) && (socket.readyState === WebSocket.OPEN)) {
        client.send(stringMessage);
        console.log('--- Sent ---');
      } else {
        console.log('--- Skip Sender ---');
      }
    });
  });
});
