const net = require('net');

try {
  const client = new net.Socket();
  client.connect(8080, 'web-listener', function() {
    console.log('Connected with web listener socket');
  });

  client.on('data', function(data) {
    console.log('Wait for graphic app received data: ' + data);
    if (data.toString().includes('graphic-app-finished-calc')) {
      console.log('Closing process');
      client.destroy();
      setTimeout(() => {
        process.exit(0);
      }, 500);
    }
  });

  client.on('close', function() {
    console.log('Connection closed');
  });

} catch (e) {
  console.log('Wait for graphic app error', e);
  throw e;
}
