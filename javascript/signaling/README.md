# Signaling

Signaling Server and Signaling Channel for WebRTC

## Setup

```bash
$ npm install
$ npm run dev
```

then, launch `answer.html` and `offer.html` in separate windows.

```bash
$ open http://localhost:8080/answer.html
$ open http://localhost:8080/offer.html
```

## Use Remote Signaling Server

Replace `localhost` to `18.183.102.255` in `ip` field `app/server-config.js`

```js
module.exports = {
  ip  : '18.183.102.255',
  port: {
    signaling: 3000,
    web      : 8080
  }
};
```
