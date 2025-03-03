const connect = require('connect');
const serve   = require('serve-static');
const path    = require('path');

const dirname = path.resolve('.');

const port = 8080;

connect().use(serve(`${dirname}/`)).listen(port, () => {
  console.log(`Listen ... (${port})`);
  console.log(`Type \`open http://localhost:${port}/answer.html\``);
  console.log(`Type \`open http://localhost:${port}/offer.html\``);
});
