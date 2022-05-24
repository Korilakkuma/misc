module.exports = {
  mode: 'development',
  entry: {
    app: [
      './src/main.js'
    ]
  },
  output: {
    filename: '[name].js',
    path: `${__dirname}/public`
  },
  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /node_modules/
      }
    ]
  },
  devtool: 'source-map'
};
