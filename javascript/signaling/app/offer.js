const signalingChannel = new SignalingChannel('ws://localhost:3000/');
const peerConnection = new RTCPeerConnection();

navigator.mediaDevices.getUserMedia({ audio: true, video: true })
  .then((stream) => {
    cancelEcho(stream);

    for (const track of stream.getTracks()) {
      peerConnection.addTrack(track, stream);
    }

    const local = document.getElementById('local');
    local.srcObject = stream;

    local.play()
      .then(() => {})
      .catch(() => {});

    peerConnection.createOffer()
      .then((offer) => {
        peerConnection.setLocalDescription(offer);
        signalingChannel.send(offer);
      })
      .catch((error) => {
        console.error(error);
      });

    peerConnection.onicecandidate = (event) => {
      if (event.candidate) {
        signalingChannel.send(event.candidate);
      }
    };

    const answer = (message) => {
      peerConnection.setRemoteDescription(message);
    };

    const candidate = (message) => {
      if (message.ice) {
        peerConnection.addIceCandidate(message.ice);
      }
    }

    signalingChannel.message(() => {}, answer, candidate);

    peerConnection.ontrack = (event) => {
      const remote = document.getElementById('remote');
      remote.srcObject = event.streams[0] ? event.streams[0] : null;

      remote.play()
        .then(() => {})
        .catch(() => {});
    };
  }).catch((error) => {
    console.error(error);
  });
