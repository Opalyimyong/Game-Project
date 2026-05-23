const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

let state = JSON.parse(sessionStorage.getItem('energy_game_state') || 'null');
let socket = null;
let selectedNodeId = null;
let linkMode = false;
let linkFirstNodeId = null;

const camera = { x: 20, y: 20, zoom: 1, dragging: false, startX: 0, startY: 0 };
const CELL_SIZE = 30;

function wsUrl() { return `${location.protocol === 'https:' ? 'wss' : 'ws'}://${location.host}/ws`; }
function nodeById(id) { return state?.nodes?.find(n => n.id === id); }
function currentPlayer() { return state?.players?.find(p => p.id === state.currentPlayer); }

function connect() {
  socket = new WebSocket(wsUrl());
  socket.onopen = () => {
    document.getElementById('connection').textContent = 'Connected';
    send({ type: 'GET_STATE' });
  };
  socket.onclose = () => document.getElementById('connection').textContent = 'Disconnected';
  socket.onerror = () => document.getElementById('connection').textContent = 'Connection error';
  socket.onmessage = (event) => {
    const message = JSON.parse(event.data);
    if (message.state) {
      state = message.state;
      sessionStorage.setItem('energy_game_state', JSON.stringify(state));
      updateHud();
      drawGame();
    }
  };
}

function send(data) {
  if (!socket || socket.readyState !== WebSocket.OPEN) return;
  socket.send(JSON.stringify(data));
}

function updateHud() {
  if (!state) return;
  const turnPanel = document.getElementById('turn-panel');
  turnPanel.innerHTML = `<div>Turn: <b>${state.turn}</b></div><div>Current: <b>Player ${state.currentPlayer}</b></div>` +
    state.players.map(p => `<div class="player-row ${p.id === state.currentPlayer ? 'current' : ''}"><span>P${p.id}</span><span>${p.coins.toFixed(0)}⚡¢ | AP ${p.ap} | Waste ${p.waste.toFixed(1)}</span></div>`).join('') +
    `<p class="${state.winner ? 'ok' : ''}">${state.message || ''}</p>`;

  const selected = nodeById(selectedNodeId);
  const selectedPanel = document.getElementById('selected-panel');
  if (!selected) {
    selectedPanel.textContent = 'Click a node on the map.';
    return;
  }
  selectedPanel.innerHTML = `
    <div><b>#${selected.id}</b> ${selected.type} ${selected.subtype}</div>
    <div>Position: (${selected.x}, ${selected.y})</div>
    <div>Building: ${selected.hasBuilding ? `${selected.buildingType} (${selected.plantType}) by P${selected.owner}` : 'None'}</div>
    ${selected.type === 'City' ? `<div>Energy: ${selected.energy.toFixed(1)} / min ${selected.demandMin}</div>` : ''}
    ${selected.type === 'Power' ? `<div>Energy Output: ${selected.energyOutput.toFixed(1)}</div>` : ''}
    ${selected.type === 'Resource' ? `<div>Resource Output: ${selected.resourceOutput.toFixed(1)}</div>` : ''}
    ${linkMode ? '<p class="ok">Link mode is active.</p>' : ''}`;
}

function drawTile(x, y, fill) {
  ctx.fillStyle = fill;
  ctx.fillRect(x, y, CELL_SIZE, CELL_SIZE);
  ctx.strokeStyle = 'rgba(0,0,0,.16)';
  ctx.strokeRect(x, y, CELL_SIZE, CELL_SIZE);
}

function drawGame() {
  if (!state) return;
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.save();
  ctx.translate(camera.x, camera.y);
  ctx.scale(camera.zoom, camera.zoom);

  const width = state.width * CELL_SIZE;
  const height = state.height * CELL_SIZE;
  ctx.fillStyle = '#356f42';
  ctx.fillRect(0, 0, width, height);

  const startCol = Math.max(0, Math.floor((-camera.x / camera.zoom) / CELL_SIZE));
  const endCol = Math.min(state.width, Math.ceil(((-camera.x + canvas.width) / camera.zoom) / CELL_SIZE));
  const startRow = Math.max(0, Math.floor((-camera.y / camera.zoom) / CELL_SIZE));
  const endRow = Math.min(state.height, Math.ceil(((-camera.y + canvas.height) / camera.zoom) / CELL_SIZE));

  for (let r = startRow; r < endRow; r++) {
    for (let c = startCol; c < endCol; c++) drawTile(c * CELL_SIZE, r * CELL_SIZE, '#356f42');
  }

  ctx.lineWidth = 4;
  state.links.forEach(link => {
    const a = nodeById(link.a); const b = nodeById(link.b);
    if (!a || !b) return;
    ctx.strokeStyle = link.owner === state.currentPlayer ? '#00ffcc' : '#f8e16c';
    ctx.beginPath();
    ctx.moveTo(a.x * CELL_SIZE + CELL_SIZE / 2, a.y * CELL_SIZE + CELL_SIZE / 2);
    ctx.lineTo(b.x * CELL_SIZE + CELL_SIZE / 2, b.y * CELL_SIZE + CELL_SIZE / 2);
    ctx.stroke();
  });

  state.nodes.forEach(node => {
    const x = node.x * CELL_SIZE;
    const y = node.y * CELL_SIZE;
    let color = '#888'; let icon = '?';
    if (node.type === 'Resource') { color = '#fbc02d'; icon = '◆'; }
    if (node.type === 'Power') { color = '#ff7043'; icon = '⚡'; }
    if (node.type === 'City') { color = node.subtype === 'Big' ? '#1565c0' : '#64b5f6'; icon = node.subtype === 'Big' ? '🏙' : '🏠'; }

    ctx.fillStyle = color;
    ctx.beginPath();
    ctx.arc(x + CELL_SIZE / 2, y + CELL_SIZE / 2, CELL_SIZE * 0.42, 0, Math.PI * 2);
    ctx.fill();

    if (node.hasBuilding) {
      ctx.strokeStyle = node.owner === state.currentPlayer ? '#00ffcc' : '#fff';
      ctx.lineWidth = 3;
      ctx.stroke();
      icon = node.buildingType === 'PowerPlant' ? '🏭' : '⛏';
    }
    if (node.id === selectedNodeId || node.id === linkFirstNodeId) {
      ctx.strokeStyle = '#fff'; ctx.lineWidth = 4; ctx.stroke();
    }
    ctx.fillStyle = '#111';
    ctx.font = '18px Segoe UI Emoji, Arial';
    ctx.textAlign = 'center';
    ctx.textBaseline = 'middle';
    ctx.fillText(icon, x + CELL_SIZE / 2, y + CELL_SIZE / 2 + 1);
  });

  ctx.restore();
}

function screenToWorld(event) {
  const rect = canvas.getBoundingClientRect();
  return {
    x: (event.clientX - rect.left - camera.x) / camera.zoom,
    y: (event.clientY - rect.top - camera.y) / camera.zoom
  };
}

function pickNode(event) {
  const pos = screenToWorld(event);
  const col = Math.floor(pos.x / CELL_SIZE);
  const row = Math.floor(pos.y / CELL_SIZE);
  return state.nodes.find(n => n.x === col && n.y === row) || null;
}

canvas.addEventListener('contextmenu', e => e.preventDefault());
canvas.addEventListener('mousedown', event => {
  if (event.button === 2 || event.shiftKey) {
    camera.dragging = true;
    camera.startX = event.clientX - camera.x;
    camera.startY = event.clientY - camera.y;
    return;
  }
  const node = pickNode(event);
  if (!node) return;

  selectedNodeId = node.id;
  if (linkMode) {
    if (!linkFirstNodeId) {
      linkFirstNodeId = node.id;
    } else if (linkFirstNodeId !== node.id) {
      send({ type: 'ADD_LINK', a: linkFirstNodeId, b: node.id });
      linkMode = false;
      linkFirstNodeId = null;
    }
  }
  updateHud();
  drawGame();
});

canvas.addEventListener('mousemove', event => {
  if (!camera.dragging) return;
  camera.x = event.clientX - camera.startX;
  camera.y = event.clientY - camera.startY;
  drawGame();
});
window.addEventListener('mouseup', () => camera.dragging = false);
canvas.addEventListener('wheel', event => {
  event.preventDefault();
  const oldZoom = camera.zoom;
  camera.zoom = Math.min(1.7, Math.max(0.55, camera.zoom + (event.deltaY < 0 ? 0.1 : -0.1)));
  const rect = canvas.getBoundingClientRect();
  const mx = event.clientX - rect.left;
  const my = event.clientY - rect.top;
  camera.x = mx - ((mx - camera.x) / oldZoom) * camera.zoom;
  camera.y = my - ((my - camera.y) / oldZoom) * camera.zoom;
  drawGame();
});

function buildResourcePlant() {
  if (!selectedNodeId) return;
  send({ type: 'BUILD_RESOURCE', nodeId: selectedNodeId, resourceType: document.getElementById('resource-type').value });
}
function buildPowerPlant() {
  if (!selectedNodeId) return;
  send({ type: 'BUILD_POWER', nodeId: selectedNodeId, plantType: document.getElementById('plant-type').value });
}
function setLinkMode() {
  linkMode = true;
  linkFirstNodeId = null;
  updateHud();
}
function endTurn() { send({ type: 'END_TURN' }); }
function newGame() { window.location.href = 'index.html'; }

if (!state) send({ type: 'GET_STATE' });
connect();
updateHud();
drawGame();
