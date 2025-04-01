const config = require('./server-config');

const connect = require('connect');
const serve   = require('serve-static');
const path    = require('path');

const dirname = path.resolve('.');

const ip = config.ip === 'localhost' ? 'localhost' : '0.0.0.0';

connect().use(serve(`${dirname}/`)).listen(config.port.web, ip, () => {
  console.log(`Listen ... (${config.port.web})`);
  console.log(`Type \`open http://${config.ip}:${config.port.web}/answer.html\``);
  console.log(`Type \`open http://${config.ip}:${config.port.web}/offer.html\``);
});
