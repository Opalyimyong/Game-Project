let socket = null;
let playerCount = 3;

function wsUrl() {
  return `${location.protocol === 'https:' ? 'wss' : 'ws'}://${location.host}/ws`;
}

function setStatus(message) {
  const el = document.getElementById('status');
  if (el) el.textContent = message;
}

function connect() {
  socket = new WebSocket(wsUrl());
  socket.onopen = () => setStatus('Connected to C++ server.');
  socket.onclose = () => setStatus('Server disconnected. Run ./energy_game_server and refresh.');
  socket.onerror = () => setStatus('Cannot connect to server.');
  socket.onmessage = (event) => {
    const response = JSON.parse(event.data);
    if (response.type === 'GAME_STARTED') {
      sessionStorage.setItem('energy_game_state', JSON.stringify(response.state));
      window.location.href = 'gameplay.html';
    } else if (response.type === 'ERROR') {
      setStatus(response.message || 'Error');
    }
  };
}

function incrementPlayers() {
  if (playerCount < 3) playerCount++;
  document.getElementById('player-val').textContent = playerCount;
}

function decrementPlayers() {
  if (playerCount > 1) playerCount--;
  document.getElementById('player-val').textContent = playerCount;
}

function startGame() {
  if (!socket || socket.readyState !== WebSocket.OPEN) {
    setStatus('Connecting... try again in a moment.');
    connect();
    return;
  }
  socket.send(JSON.stringify({ type: 'START_GAME', players: playerCount }));
  setStatus('Creating world...');
}

connect();
