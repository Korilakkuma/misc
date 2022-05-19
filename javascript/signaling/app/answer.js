const signalingChannel = new SignalingChannel('ws://localhost:3000/');

let peerConnection = null;

const offer = (message) => {
  peerConnection = new RTCPeerConnection();
  peerConnection.setRemoteDescription(message);

  peerConnection.onicecandidate = (event) => {
    if (event.candidate) {
      signalingChannel.send(event.candidate);
    }
  };

  peerConnection.ontrack = (event) => {
    const remote = document.getElementById('remote');
    remote.srcObject = event.streams[0] ? event.streams[0] : null;

    remote.play()
      .then(() => {})
      .catch(() => {});
  };

  navigator.mediaDevices.getUserMedia({ audio: true, video: true })
    .then((stream) => {
      for (const track of stream.getTracks()) {
        peerConnection.addTrack(track, stream);
      }

      const local = document.getElementById('local');
      local.srcObject = stream;

      local.play()
        .then(() => {})
        .catch(() => {});

      peerConnection.createAnswer()
        .then((answer) => {
          peerConnection.setLocalDescription(answer);
          signalingChannel.send(answer);
        })
        .catch((error) => {
          console.error(error);
        });
    })
    .catch((error) => {
      console.error(error);
    });
};

const answer = (message) => {
  peerConnection.setRemoteDescription(message);
};

const candidate = (message) => {
  peerConnection.addIceCandidate(message);
};

signalingChannel.message(offer, answer, candidate);
