const signalingChannel = new SignalingChannel(`ws://${ip}:${port}/`);
const peerConnection = new RTCPeerConnection();

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

    await local.play()

    peerConnection
      .createOffer()
      .then((offer) => {
        peerConnection.setLocalDescription(offer);
        signalingChannel.send(offer);
      })
      .catch(console.error);

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

    peerConnection.ontrack = async (event) => {
      const remote = document.getElementById('remote');
      remote.srcObject = event.streams[0] ? event.streams[0] : null;

      await remote.play();
    };
  })
  .catch(console.error);
