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

  peerConnection.ontrack = async (event) => {
    const remote = document.getElementById('remote');
    remote.srcObject = event.streams[0] ? event.streams[0] : null;

    await remote.play();
  };

  navigator
    .mediaDevices
    .getUserMedia({ audio: true, video: true })
    .then(async (stream) => {
      cancelEchoUseStepOnly(stream);

      for (const track of stream.getTracks()) {
        peerConnection.addTrack(track, stream);
      }

      const local = document.getElementById('local');
      local.srcObject = stream;

      await local.play();

      peerConnection
        .createAnswer()
        .then((answer) => {
          peerConnection.setLocalDescription(answer);
          signalingChannel.send(answer);
        })
        .catch(console.error);
    })
    .catch(console.error);
};

const answer = (message) => {
  peerConnection.setRemoteDescription(message);
};

const candidate = (message) => {
  peerConnection.addIceCandidate(message);
};

signalingChannel.message(offer, answer, candidate);
