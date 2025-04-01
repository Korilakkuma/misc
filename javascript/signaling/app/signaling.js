'use strict';

const config = require('./server-config');

const WebSocket = require('ws');
const WebSocketServer = WebSocket.Server;

const ws = new WebSocketServer({ port: config.port.signaling, host: config.ip });

console.log(`ws://${config.ip} Wait port (${config.port.signaling}) ...`);

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
