// game.js - Comprehensive Grid Engine for Energy Not Included

const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// Engine Configuration Variables
const GRID_SIZE = 30;
const CELL_SIZE = 60; // Increased by 1.5x (from 40 to 60)

// Dynamic Canvas Sizing
function resizeCanvas() {
    const container = document.getElementById('game-container');
    canvas.width = container.clientWidth - 40; 
    canvas.height = container.clientHeight - 40;
}
window.addEventListener('resize', resizeCanvas);
resizeCanvas(); // Initial size

// Camera Viewport State
let camera = { x: 0, y: 0, isDragging: false, startX: 0, startY: 0 };

// Load Players from sessionStorage
let players = JSON.parse(sessionStorage.getItem("gamePlayers")) || [
    { id: "p1", name: "Player 1", color: "#ff0044", ap: 3, score: 0, waste: 0, coins: 300 },
    { id: "p2", name: "Player 2", color: "#0088ff", ap: 3, score: 0, waste: 0, coins: 300 }
];

// Ensure existing sessions without score/waste/coins get them initialized
players.forEach(p => {
    if (typeof p.score === 'undefined') p.score = 0;
    if (typeof p.waste === 'undefined') p.waste = 0;
    if (typeof p.coins === 'undefined') p.coins = 300;
});

let currentPlayerIndex = 0;

// Game State Layers
let nodeLayer = [];
let buildingLayer = [];
let links = [];

let selectedNodeForLink = null; // Used when creating a link
let pendingBuilding = null; // Used when the subtype modal is open

const RESOURCE_TYPES = ["Coal", "Gas", "Biomass", "Uranium"];
const POWER_TYPES = ["Coal Plant", "Gas Plant", "Biomass Plant", "Solar Plant", "Wind Plant", "Hydro Plant", "Nuclear Plant"];

// Initialize Game Data
function initMap() {
    for (let r = 0; r < GRID_SIZE; r++) {
        let rowNodes = [];
        let rowBuildings = [];
        for (let c = 0; c < GRID_SIZE; c++) {
            rowNodes.push(null);
            rowBuildings.push(null);
        }
        nodeLayer.push(rowNodes);
        buildingLayer.push(rowBuildings);
    }

    const mapData = [
        {type: "Resource", subtype: "Coal", x: 6, y: 2},
        {type: "Resource", subtype: "Gas", x: 14, y: 2},
        {type: "Resource", subtype: "Uranium", x: 24, y: 2},
        {type: "Power", x: 3, y: 4, sunlight: 60, wind: 30, water: false},
        {type: "Power", x: 9, y: 4, sunlight: 70, wind: 40, water: false},
        {type: "Power", x: 21, y: 4, sunlight: 80, wind: 50, water: false},
        {type: "Power", x: 27, y: 4, sunlight: 60, wind: 70, water: false},
        {type: "City", subtype: "Small", x: 16, y: 5},
        {type: "Resource", subtype: "Biomass", x: 19, y: 6},
        {type: "Resource", subtype: "Coal", x: 2, y: 7},
        {type: "City", subtype: "Big", x: 8, y: 7},
        {type: "Resource", subtype: "Gas", x: 14, y: 7},
        {type: "City", subtype: "Big", x: 24, y: 7},
        {type: "Resource", subtype: "Biomass", x: 27, y: 7},
        {type: "Resource", subtype: "Uranium", x: 17, y: 10},
        {type: "Power", x: 3, y: 11, sunlight: 50, wind: 60, water: false},
        {type: "City", subtype: "Small", x: 17, y: 12},
        {type: "Resource", subtype: "Gas", x: 24, y: 12},
        {type: "Power", x: 27, y: 12, sunlight: 40, wind: 80, water: false},
        {type: "Power", x: 12, y: 13, sunlight: 70, wind: 60, water: false},
        {type: "Power", x: 20, y: 13, sunlight: 60, wind: 90, water: false},
        {type: "Resource", subtype: "Coal", x: 6, y: 15},
        {type: "City", subtype: "Small", x: 13, y: 16},
        {type: "Resource", subtype: "Biomass", x: 7, y: 18},
        {type: "Power", x: 16, y: 18, sunlight: 80, wind: 40, water: true},
        {type: "Resource", subtype: "Coal", x: 22, y: 18},
        {type: "City", subtype: "Small", x: 27, y: 18},
        {type: "Resource", subtype: "Gas", x: 4, y: 20},
        {type: "Power", x: 9, y: 20, sunlight: 50, wind: 70, water: false},
        {type: "Resource", subtype: "Biomass", x: 15, y: 20},
        {type: "Resource", subtype: "Uranium", x: 25, y: 20},
        {type: "Power", x: 23, y: 21, sunlight: 40, wind: 90, water: false},
        {type: "City", subtype: "Small", x: 18, y: 22},
        {type: "City", subtype: "Big", x: 8, y: 23},
        {type: "Power", x: 28, y: 24, sunlight: 60, wind: 50, water: true},
        {type: "Power", x: 1, y: 25, sunlight: 70, wind: 30, water: false},
        {type: "Resource", subtype: "Coal", x: 6, y: 27},
        {type: "Power", x: 14, y: 27, sunlight: 80, wind: 60, water: false},
        {type: "Resource", subtype: "Gas", x: 17, y: 27},
        {type: "Power", x: 21, y: 27, sunlight: 50, wind: 80, water: false},
        {type: "Resource", subtype: "Biomass", x: 26, y: 27}
    ];

    mapData.forEach(d => {
        if (d.type === 'City') {
            nodeLayer[d.y][d.x] = { 
                type: 'City', 
                subtype: d.subtype,
                energy: 0, 
                minEnergy: d.subtype === 'Big' ? 25 : 10,
                maxEnergy: d.subtype === 'Big' ? 40 : 20,
                isPowered: false 
            };
        } else if (d.type === 'Power') {
            nodeLayer[d.y][d.x] = {
                type: 'Power',
                sunlight: d.sunlight,
                wind: d.wind,
                water: d.water
            };
        } else if (d.type === 'Resource') {
            nodeLayer[d.y][d.x] = { type: 'Resource', subtype: d.subtype };
        }
    });
}

function updateHUD() {
    const container = document.getElementById('players-container');
    container.innerHTML = '';
    
    players.forEach((player, index) => {
        const isActive = index === currentPlayerIndex;
        container.innerHTML += `
            <div class="player-status ${isActive ? 'active' : ''}">
                <div class="player-header">
                    <div class="color-indicator" style="background-color: ${player.color}"></div>
                    <div class="player-name">${player.name}</div>
                </div>
                <div class="ap-display">
                    AP: <strong>${player.ap}</strong> | 
                    Coins: <strong style="color: #ffd700">🪙${player.coins}</strong> | 
                    Waste: <strong style="color: #ff4444">🛢️${player.waste}</strong>
                </div>
            </div>
        `;
    });
}

function showMessage(msg) {
    document.getElementById('game-message').textContent = msg;
    setTimeout(() => {
        document.getElementById('game-message').textContent = 'Welcome to Energy Not Included!';
    }, 3000);
}

// Render Engine
function drawGame() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.save();
    ctx.translate(camera.x, camera.y);

    const startCol = Math.max(0, Math.floor(-camera.x / CELL_SIZE));
    const endCol = Math.min(GRID_SIZE, Math.ceil((-camera.x + canvas.width) / CELL_SIZE));
    const startRow = Math.max(0, Math.floor(-camera.y / CELL_SIZE));
    const endRow = Math.min(GRID_SIZE, Math.ceil((-camera.y + canvas.height) / CELL_SIZE));

    // Draw Grid Background
    for (let r = startRow; r < endRow; r++) {
        for (let c = startCol; c < endCol; c++) {
            const x = c * CELL_SIZE;
            const y = r * CELL_SIZE;
            ctx.fillStyle = "#1a1a24";
            ctx.fillRect(x, y, CELL_SIZE, CELL_SIZE);
            ctx.strokeStyle = "rgba(255, 255, 255, 0.05)";
            ctx.lineWidth = 1;
            ctx.strokeRect(x, y, CELL_SIZE, CELL_SIZE);
        }
    }

    // Draw Links (Bottom Layer of interactables)
    links.forEach(link => {
        const fromX = link.from.c * CELL_SIZE + CELL_SIZE / 2;
        const fromY = link.from.r * CELL_SIZE + CELL_SIZE / 2;
        const toX = link.to.c * CELL_SIZE + CELL_SIZE / 2;
        const toY = link.to.r * CELL_SIZE + CELL_SIZE / 2;

        ctx.beginPath();
        ctx.moveTo(fromX, fromY);
        ctx.lineTo(toX, toY);
        ctx.strokeStyle = link.color;
        ctx.lineWidth = 6; // Increased link width slightly for bigger scale
        ctx.stroke();
        
        // Draw moving particle to show direction
        const time = Date.now() / 1000;
        const t = (time % 2) / 2; // 0 to 1 over 2 seconds
        const px = fromX + (toX - fromX) * t;
        const py = fromY + (toY - fromY) * t;
        ctx.beginPath();
        ctx.arc(px, py, 6, 0, Math.PI * 2);
        ctx.fillStyle = "#ffffff";
        ctx.fill();
    });

    // Draw Nodes and Buildings
    for (let r = startRow; r < endRow; r++) {
        for (let c = startCol; c < endCol; c++) {
            const node = nodeLayer[r][c];
            const building = buildingLayer[r][c];
            const x = c * CELL_SIZE;
            const y = r * CELL_SIZE;
            const centerX = x + CELL_SIZE / 2;
            const centerY = y + CELL_SIZE / 2;

            if (node) {
                // Base Node Shape (White circle)
                ctx.beginPath();
                ctx.arc(centerX, centerY, CELL_SIZE / 2.5, 0, 2 * Math.PI);
                ctx.fillStyle = "#ffffff";
                ctx.fill();

                // Outline logic
                if (node.type === 'City') {
                    // City node outline color based on powered status
                    ctx.strokeStyle = node.isPowered ? "#00ffcc" : "#ff9900"; 
                    ctx.lineWidth = 4;
                    ctx.stroke();
                    
                    // Vertical Energy Gauge on the right side
                    const gaugeWidth = 6;
                    const gaugeHeight = CELL_SIZE * 0.6;
                    const gaugeX = centerX + CELL_SIZE / 2.5 + 2; // Placed just outside the right edge of the circle
                    const gaugeY = centerY - gaugeHeight / 2;
                    
                    // Gauge Background
                    ctx.fillStyle = "#222";
                    ctx.fillRect(gaugeX, gaugeY, gaugeWidth, gaugeHeight);
                    
                    // Gauge Fill (bottom-up)
                    const fillRatio = Math.min(node.energy / node.maxEnergy, 1.0);
                    const fillHeight = gaugeHeight * fillRatio;
                    ctx.fillStyle = node.isPowered ? "#00ffcc" : "#ff9900";
                    ctx.fillRect(gaugeX, gaugeY + gaugeHeight - fillHeight, gaugeWidth, fillHeight);
                    
                    // Gauge Border
                    ctx.strokeStyle = "#111";
                    ctx.lineWidth = 1;
                    ctx.strokeRect(gaugeX, gaugeY, gaugeWidth, gaugeHeight);
                    
                } else if (building) {
                    ctx.strokeStyle = building.ownerColor;
                    ctx.lineWidth = 4;
                    ctx.stroke();
                } else {
                    ctx.strokeStyle = "#555566";
                    ctx.lineWidth = 3;
                    ctx.stroke();
                }

                // Node Icon
                ctx.fillStyle = "#000000";
                ctx.font = "24px Arial"; // Scaled font up for larger cells
                ctx.textAlign = "center";
                ctx.textBaseline = "middle";
                let icon = "";
                if (node.type === 'City') icon = "🏙️";
                else if (node.type === 'Power') icon = "⚡";
                else if (node.type === 'Resource') {
                    if (node.subtype === 'Coal') icon = "🪨";
                    else if (node.subtype === 'Gas') icon = "☁️";
                    else if (node.subtype === 'Biomass') icon = "🌿";
                    else if (node.subtype === 'Uranium') icon = "☢️";
                    else icon = "⛏️";
                }
                
                // If building exists, override icon
                if (building) {
                    if (building.type === 'PowerPlant') icon = "🏭";
                    if (building.type === 'ResourcePlant') icon = "🚜";
                }
                
                const textOffsetY = 2; // +2 helps center standard emojis vertically
                ctx.fillText(icon, centerX, centerY + textOffsetY);

                // Draw Label Below Node
                ctx.shadowColor = "rgba(0, 0, 0, 0.9)";
                ctx.shadowBlur = 5;
                ctx.fillStyle = "#ffffff";
                ctx.font = "bold 14px 'Segoe UI', Tahoma, sans-serif";
                ctx.textAlign = "center";
                
                let label = "";
                if (building) {
                    label = building.subtype || building.type;
                } else if (node.type === 'City') {
                    label = "City";
                } else if (node.type === 'Resource') {
                    label = node.subtype; // Display "Coal", "Gas", etc.
                } else if (node.type === 'Power') {
                    label = "Power Node";
                }
                
                if (label) {
                    ctx.fillText(label, centerX, centerY + 24);
                }
                
                // Draw Passive Resource Stats for empty Power Nodes
                if (node.type === 'Power' && !building) {
                    ctx.shadowColor = "rgba(0, 0, 0, 0.95)";
                    ctx.shadowBlur = 6;
                    ctx.font = "bold 16px 'Segoe UI', Tahoma, sans-serif"; // Slightly larger since it's just emojis now
                    let passives = [];
                    // Only display the icon if the index is high enough to actually build the plant (>= 50)
                    if (node.sunlight >= 50) passives.push(`☀️`);
                    if (node.wind >= 50) passives.push(`💨`);
                    if (node.water) passives.push(`💧`);
                    
                    if (passives.length > 0) {
                        ctx.fillText(passives.join(' '), centerX, centerY - 28);
                    }
                    ctx.shadowBlur = 0; // reset shadow
                }
                ctx.shadowBlur = 0; // reset shadow for outer block
            }
            
            // Highlight selected node for linking
            if (selectedNodeForLink && selectedNodeForLink.r === r && selectedNodeForLink.c === c) {
                ctx.strokeStyle = "#00ffcc";
                ctx.lineWidth = 4;
                ctx.setLineDash([6, 6]);
                ctx.strokeRect(x, y, CELL_SIZE, CELL_SIZE);
                ctx.setLineDash([]);
            }
        }
    }
    ctx.restore();
    requestAnimationFrame(drawGame); // Continuous render loop for link animations
}

// WebSocket Setup
const socket = new WebSocket('ws://localhost:18888/');
socket.onopen = () => console.log("Connected to C++ Backend");

socket.onmessage = (event) => {
    try {
        const msg = JSON.parse(event.data);
        if (msg.action === "game_state") {
            msg.players.forEach(serverPlayer => {
                let localPlayer = players.find(p => p.id === serverPlayer.id);
                if (localPlayer) {
                    localPlayer.ap = serverPlayer.ap;
                    localPlayer.coins = serverPlayer.coins;
                    localPlayer.waste = serverPlayer.waste;
                }
            });
            if (msg.nodes) {
                msg.nodes.forEach(n => {
                    let city = nodeLayer[n.r][n.c];
                    if (city && city.type === 'City') {
                        city.energy = n.energy;
                        city.isPowered = n.isPowered;
                    }
                });
            }
            updateHUD();
            drawGrid(); // Re-render grid to show updated UI state if necessary
        } else if (msg.action === "game_over") {
            triggerGameOver(msg.winner_name, msg.winner_score, msg.reason);
        }
    } catch(e) {
        console.error("Error parsing websocket message:", e);
    }
};

function sendActionToBackend(actionData) {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify(actionData));
    }
}

// --- MOUSE & CAMERA CONTROLS ---
canvas.addEventListener('contextmenu', e => e.preventDefault());

canvas.addEventListener('mousedown', function(event) {
    const rect = canvas.getBoundingClientRect();
    const screenX = event.clientX - rect.left;
    const screenY = event.clientY - rect.top;

    if (event.button === 2 || event.shiftKey) {
        camera.isDragging = true;
        camera.startX = screenX - camera.x;
        camera.startY = screenY - camera.y;
    } else if (event.button === 0) {
        const worldX = screenX - camera.x;
        const worldY = screenY - camera.y;

        const col = Math.floor(worldX / CELL_SIZE);
        const row = Math.floor(worldY / CELL_SIZE);

        if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE) {
            handleGridClick(row, col);
        }
    }
});

canvas.addEventListener('mousemove', function(event) {
    const rect = canvas.getBoundingClientRect();
    const screenX = event.clientX - rect.left;
    const screenY = event.clientY - rect.top;

    if (camera.isDragging) {
        camera.x = screenX - camera.startX;
        camera.y = screenY - camera.startY;
        
        // Boundaries
        const maxPanX = canvas.width - (GRID_SIZE * CELL_SIZE);
        const maxPanY = canvas.height - (GRID_SIZE * CELL_SIZE);
        if (maxPanX < 0) camera.x = Math.min(0, Math.max(maxPanX, camera.x));
        if (maxPanY < 0) camera.y = Math.min(0, Math.max(maxPanY, camera.y));
    }
});

window.addEventListener('mouseup', () => camera.isDragging = false);

// Game Logic
function handleGridClick(row, col) {
    const currentPlayer = players[currentPlayerIndex];
    const node = nodeLayer[row][col];
    const selectedTool = document.querySelector('input[name="tool"]:checked').value;

    if (selectedTool === 'info') {
        if (node) {
            const b = buildingLayer[row][col];
            let msg = `Node: ${node.type} | Building: ${b ? b.subtype || b.type : 'None'}`;
            if (node.type === 'Power') {
                msg += ` | Passives - ☀️${node.sunlight} 💨${node.wind} 💧${node.water ? 'Yes' : 'No'}`;
            }
            showMessage(msg);
        }
        return;
    }

    if (currentPlayer.ap <= 0) {
        showMessage("No Action Points left! End your turn.");
        return;
    }

    if (selectedTool === 'resource') {
        if (!node || node.type !== 'Resource') {
            showMessage("Resource Plants must be built on Resource Nodes!");
            return;
        }
        if (buildingLayer[row][col]) {
            showMessage("Building already exists here.");
            return;
        }
        openBuildingModal('ResourcePlant', row, col, node);
    } 
    else if (selectedTool === 'power') {
        if (!node || node.type !== 'Power') {
            showMessage("Power Plants must be built on Power Nodes!");
            return;
        }
        if (buildingLayer[row][col]) {
            showMessage("Building already exists here.");
            return;
        }
        openBuildingModal('PowerPlant', row, col, node);
    }
    else if (selectedTool === 'link') {
        if (!node) return;
        
        if (!selectedNodeForLink) {
            // First click (source)
            const building = buildingLayer[row][col];
            if (!building) {
                showMessage("Source node must have a building.");
                return;
            }
            if (building.ownerId !== currentPlayer.id) {
                showMessage("You can only link from your own buildings!");
                return;
            }
            selectedNodeForLink = { r: row, c: col, node: node };
            showMessage("Select destination node.");
        } else {
            // Second click (destination)
            if (selectedNodeForLink.r === row && selectedNodeForLink.c === col) {
                selectedNodeForLink = null; // Cancel
                showMessage("Link cancelled.");
                return;
            }
            
            // Validate Link Logic: Resource -> Power OR Power -> City
            const source = selectedNodeForLink.node;
            const dest = node;
            
            let valid = false;
            let errorMsg = "Invalid Link! Must be Resource->Power or Power->City.";
            
            if (source.type === 'Resource' && dest.type === 'Power') {
                const sourceB = buildingLayer[selectedNodeForLink.r][selectedNodeForLink.c];
                const destB = buildingLayer[row][col];
                if (!sourceB || !destB) {
                    errorMsg = "Both nodes must have buildings to link them!";
                } else if (sourceB.subtype === "Coal" && destB.subtype !== "Coal Plant") {
                    errorMsg = "Coal can only be sent to a Coal Plant!";
                } else if (sourceB.subtype === "Gas" && destB.subtype !== "Gas Plant") {
                    errorMsg = "Gas can only be sent to a Gas Plant!";
                } else if (sourceB.subtype === "Biomass" && destB.subtype !== "Biomass Plant") {
                    errorMsg = "Biomass can only be sent to a Biomass Plant!";
                } else if (sourceB.subtype === "Uranium" && destB.subtype !== "Nuclear Plant") {
                    errorMsg = "Uranium can only be sent to a Nuclear Plant!";
                } else {
                    valid = true;
                }
            } else if (source.type === 'Power' && dest.type === 'City') {
                valid = true;
            }

            if (!valid) {
                showErrorPopup(errorMsg);
                selectedNodeForLink = null;
                return;
            }

            // Reward player if connecting to city
            if (dest.type === 'City') {
                // Do not instantly reward or power the city. The backend end-of-round simulation will handle energy and payouts!
            }

            links.push({
                from: { r: selectedNodeForLink.r, c: selectedNodeForLink.c },
                to: { r: row, c: col },
                ownerId: currentPlayer.id,
                color: currentPlayer.color
            });
            
            let tType = (source.type === 'Resource') ? "Resource" : "Energy";
            
            let fromSubtype = source.subtype;
            let toSubtype = dest.subtype;
            if (buildingLayer[selectedNodeForLink.r][selectedNodeForLink.c]) {
                fromSubtype = buildingLayer[selectedNodeForLink.r][selectedNodeForLink.c].subtype || fromSubtype;
            }
            if (buildingLayer[row][col]) {
                toSubtype = buildingLayer[row][col].subtype || toSubtype;
            }

            sendActionToBackend({ 
                  action: "link", 
                  from: { r: selectedNodeForLink.r, c: selectedNodeForLink.c, type: source.type, subtype: fromSubtype }, 
                  to: { r: row, c: col, type: dest.type, subtype: toSubtype }, 
                  player: currentPlayer.id, 
                  transport_type: tType 
              });
            
            selectedNodeForLink = null;
            updateHUD();
        }
    }
}

function endTurn() {
    players[currentPlayerIndex].ap = 3; // Reset AP for next turn
    currentPlayerIndex = (currentPlayerIndex + 1) % players.length;
    selectedNodeForLink = null;
    updateHUD();
    showMessage(`${players[currentPlayerIndex].name}'s Turn`);
    sendActionToBackend({ action: "end_turn", nextPlayer: players[currentPlayerIndex].id });
}

function disposeWaste() {
    const amountStr = prompt("How much waste would you like to dispose? (Cost: 3 Coins per unit, requires 1 AP)");
    if (!amountStr) return;
    
    const amount = parseFloat(amountStr);
    if (isNaN(amount) || amount <= 0) {
        alert("Invalid amount.");
        return;
    }

    const p = players[currentPlayerIndex];
    sendActionToBackend({ action: "dispose_waste", amount: amount, player: p.id });
}

// Modal Logic
function openBuildingModal(buildingType, row, col, nodeObj = null) {
    pendingBuilding = { type: buildingType, r: row, c: col };
    
    const modal = document.getElementById('building-modal');
    const title = document.getElementById('modal-title');
    const select = document.getElementById('subtype-select');
    
    select.innerHTML = '';
    
    if (buildingType === 'ResourcePlant') {
        title.textContent = "Confirm Resource Extraction";
        // Only allow building the plant that matches the node's innate resource
        select.innerHTML += `<option value="${nodeObj.subtype}">${nodeObj.subtype} Extraction</option>`;
    } else if (buildingType === 'PowerPlant') {
        title.textContent = "Select Power Plant Type";
        
        // Base types that require input resources
        const availableOptions = ["Coal Plant", "Gas Plant", "Biomass Plant", "Nuclear Plant"];
        
        // Add passive plants conditionally based on node attributes
        if (nodeObj) {
            if (nodeObj.sunlight >= 50) availableOptions.push("Solar Plant");
            if (nodeObj.wind >= 50) availableOptions.push("Wind Plant");
            if (nodeObj.water) availableOptions.push("Hydro Plant");
        }
        
        availableOptions.forEach(opt => {
            select.innerHTML += `<option value="${opt}">${opt}</option>`;
        });
    }
    
    modal.style.display = "flex";
}

function cancelBuild() {
    document.getElementById('building-modal').style.display = "none";
    pendingBuilding = null;
    showMessage("Build cancelled.");
}

function confirmBuild() {
    if (!pendingBuilding) return;
    
    const subtype = document.getElementById('subtype-select').value;
    const currentPlayer = players[currentPlayerIndex];
    const row = pendingBuilding.r;
    const col = pendingBuilding.c;
    const node = nodeLayer[row][col];
    
    buildingLayer[row][col] = { 
        type: pendingBuilding.type, 
        subtype: subtype,
        ownerId: currentPlayer.id, 
        ownerColor: currentPlayer.color 
    };

    sendActionToBackend({ 
        action: "build", 
        type: pendingBuilding.type, 
        subtype: subtype,
        r: row, 
        c: col, 
        player: currentPlayer.id,
        sunlight: node.sunlight || 0,
        wind: node.wind || 0,
        water: node.water || false
    });
    
    updateHUD();
    document.getElementById('building-modal').style.display = "none";
    pendingBuilding = null;
}

// Bootstrap Setup Execution
initMap();
updateHUD();
drawGame();

// Error Modal Logic
function showErrorPopup(msg) {
    document.getElementById('error-message-text').textContent = msg;
    document.getElementById('error-modal').style.display = 'flex';
}

function closeErrorModal() {
    document.getElementById('error-modal').style.display = 'none';
}

// Timer Logic
let remainingSeconds = (parseInt(sessionStorage.getItem('gameTimerMinutes')) || 10) * 60;
const timerDisplay = document.getElementById('timer-display');

function updateTimerDisplay() {
    const mins = Math.floor(remainingSeconds / 60);
    const secs = remainingSeconds % 60;
    timerDisplay.textContent = `${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`;
    
    if (remainingSeconds <= 60) {
        timerDisplay.style.color = '#ff4444';
        timerDisplay.style.borderColor = '#ff4444';
        timerDisplay.style.textShadow = '0 0 10px rgba(255, 68, 68, 0.5)';
    }
}

const timerInterval = setInterval(() => {
    if (remainingSeconds > 0) {
        remainingSeconds--;
        updateTimerDisplay();
    } else {
        clearInterval(timerInterval);
        timerDisplay.textContent = "00:00";
    }
}, 1000);
updateTimerDisplay();

// Game Over Logic
function triggerGameOver(winnerName, winnerScore, reason) {
    clearInterval(timerInterval);
    document.getElementById('game-over-reason').textContent = reason;
    document.getElementById('winner-name').textContent = winnerName;
    document.getElementById('winner-score').textContent = `Score: ${winnerScore.toFixed(1)}`;
    document.getElementById('game-over-modal').style.display = 'flex';
}