var net = require('net');

try {
  let activeSockets = [];
  let graphicAppSendResult = false;
  let connectionCounter = 0;
  var server = net.createServer(function(socket) {
    socket.pipe(socket);

    activeSockets.push(socket);

    connectionCounter++;
    socket.on('data', function(data) {
      const dataString = (data.toString() || '');
      const parsed = dataString.match(/{(.*?)}/gm);

      if ( parsed ) {
        parsed.forEach(item => {
          try {
            const result = JSON.parse(item);

            if ( result.log ) {
              console.log(`Log: ${result.log}`);
            } else if (result.result) {
              console.log(`Result: ${result.result}`);
            } else {
              console.log(`Other unknown: ${result}`);
            }
          } catch (e) {
            console.error('Problem with parsing', item);
          }
        });

        activeSockets.forEach(i => i.write('graphic-app-finished-calc'));
        graphicAppSendResult = true;
      } else {
        console.log('Not known input data', dataString)
      }
    });

    socket.on('close', function () {
      console.log('Closing socket');
      const toRemove = activeSockets.find(i => i === socket);

      activeSockets = activeSockets.filter(i => i !== toRemove);
    });

    socket.on('error', function (e) {
      console.log('Socket error', e);
    });

    if ( connectionCounter >= 2 ) {
      activeSockets.forEach(i => i.write('broadcaster-connected'));
    }

    if (graphicAppSendResult) {
      socket.write('graphic-app-finished-calc');
    }
  });

  server.listen(8080, () => {
    console.log('Web listener waiting on 8080');
  });

} catch (e) {
  console.log('Web listener error', e);
  throw e;
}
