'use strict';

class SignalingChannel {
  constructor(url, onopen, onerror) {
    this.websocket = new WebSocket(url);

    this.websocket.onopen  = Object.prototype.toString.call(onopen) === '[object Function]' ? onopen  : () => {};
    this.websocket.onerror = Object.prototype.toString.call(onerror) === '[object Function]' ? onerror : () => {};
  }

  send(sessionDescription) {
    if (this.websocket.readyState === WebSocket.OPEN) {
      this.websocket.send(JSON.stringify(sessionDescription));
    }
  }

  message(offer, answer, candidate) {
    this.websocket.onmessage = (event) => {
      const message = JSON.parse(event.data);

      switch (message.type) {
        case 'offer':
          if (Object.prototype.toString.call(offer) === '[object Function]') {
            offer(message);
          }

          break;
        case 'answer':
          if (Object.prototype.toString.call(answer) === '[object Function]') {
            answer(message);
          }

          break;
        default:
          if (message.candidate && (Object.prototype.toString.call(candidate) === '[object Function]')) {
            candidate(message);
          }

          break;
      }
    };
  }
}
