// game.js - Strategic Viewport Grid Engine

const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// Engine Configuration Variables
const GRID_SIZE = 50;
const CELL_SIZE = 30; // 40px tiles make assets crisp and visible!

// Camera Viewport State
let camera = {
    x: 0,
    y: 0,
    isDragging: false,
    startX: 0,
    startY: 0
};

<<<<<<< Updated upstream
// Initialize the 50x50 2D Data Array (The source of truth)
let mapGrid = [];

=======
// Load Players from sessionStorage
let players = JSON.parse(sessionStorage.getItem("gamePlayers")) || [
    { id: "p1", name: "Player 1", color: "#ff0044", ap: 3, score: 0, waste: 0, coins: 500 },
    { id: "p2", name: "Player 2", color: "#0088ff", ap: 3, score: 0, waste: 0, coins: 500 },
    { id: "p3", name: "Player 3", color: "#9900ff", ap: 3, score: 0, waste: 0, coins: 500 }
];

// Ensure existing sessions without score/waste/coins get them initialized
players.forEach(p => {
    if (typeof p.score === 'undefined') p.score = 0;
    if (typeof p.waste === 'undefined') p.waste = 0;
    if (typeof p.coins === 'undefined') p.coins = 500;
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
>>>>>>> Stashed changes
function initMap() {
    for (let r = 0; r < GRID_SIZE; r++) {
        mapGrid[r] = [];
        for (let c = 0; c < GRID_SIZE; c++) {
            mapGrid[r][c] = { type: 'blank' };
            
            // Procedurally scatter a few random resource nodes
            if (Math.random() < 0.04) {
                mapGrid[r][c] = { type: 'node' };
            }
        }
    }
}

// The Main Render Engine
function drawGame() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // Save current drawing state and apply camera translations
    ctx.save();
    ctx.translate(camera.x, camera.y);

    // Calculate visibility bounds so we don't waste power rendering off-screen tiles
    const startCol = Math.max(0, Math.floor(-camera.x / CELL_SIZE));
    const endCol = Math.min(GRID_SIZE, Math.ceil((-camera.x + canvas.width) / CELL_SIZE));
    const startRow = Math.max(0, Math.floor(-camera.y / CELL_SIZE));
    const endRow = Math.min(GRID_SIZE, Math.ceil((-camera.y + canvas.height) / CELL_SIZE));

    for (let r = startRow; r < endRow; r++) {
        for (let c = startCol; c < endCol; c++) {
            const cell = mapGrid[r][c];
            const x = c * CELL_SIZE;
            const y = r * CELL_SIZE;

            // Condition 1: Blank space (Green pasture)
            ctx.fillStyle = "#388e3c";
            ctx.fillRect(x, y, CELL_SIZE, CELL_SIZE);

            // Subtle grid boundaries
            ctx.strokeStyle = "rgba(0, 0, 0, 0.12)";
            ctx.lineWidth = 1;
            ctx.strokeRect(x, y, CELL_SIZE, CELL_SIZE);

            // Condition 2: Resource Nodes (Large Gold Circle)
            if (cell.type === 'node') {
                ctx.beginPath();
                ctx.arc(x + CELL_SIZE / 2, y + CELL_SIZE / 2, CELL_SIZE / 4, 0, 2 * Math.PI);
                ctx.fillStyle = "#fbc02d";
                ctx.fill();
            }

            // Condition 3: Buildings (Big, centered icons over a ring indicator)
            if (cell.type === 'building') {
                ctx.strokeStyle = "rgba(251, 192, 45, 0.8)";
                ctx.lineWidth = 2;
                ctx.beginPath();
                ctx.arc(x + CELL_SIZE / 2, y + CELL_SIZE / 2, CELL_SIZE / 3, 0, 2 * Math.PI);
                ctx.stroke();

                ctx.fillStyle = "#ffffff";
                ctx.font = "24px Arial"; 
                ctx.textAlign = "center";
                ctx.textBaseline = "middle";

                let icon = "🏭";
                ctx.fillText(icon, x + CELL_SIZE / 2, y + CELL_SIZE / 2);
            }
        }
    }

    // Restore context view position
    ctx.restore();
}

// --- MOUSE & CAMERA CONTROLS ---

// Prevent right-click context menus from popping up over our viewport canvas
canvas.addEventListener('contextmenu', e => e.preventDefault());

canvas.addEventListener('mousedown', function(event) {
    const rect = canvas.getBoundingClientRect();
    const screenX = event.clientX - rect.left;
    const screenY = event.clientY - rect.top;

    // Right-Click (or left click while holding Shift Key) initiates map panning drag
    if (event.button === 2 || event.shiftKey) {
        camera.isDragging = true;
        camera.startX = screenX - camera.x;
        camera.startY = screenY - camera.y;
    } else if (event.button === 0) {
        // Left-Click: Calculate target coordinate including camera position offset
        const worldX = screenX - camera.x;
        const worldY = screenY - camera.y;

        const col = Math.floor(worldX / CELL_SIZE);
        const row = Math.floor(worldY / CELL_SIZE);

        if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE) {
            const selectedTool = document.querySelector('input[name="tool"]:checked').value;

            if (selectedTool === 'blank') mapGrid[row][col] = { type: 'blank' };
            else if (selectedTool === 'node') mapGrid[row][col] = { type: 'node' };
            else if (selectedTool === 'factory') mapGrid[row][col] = { type: 'building', buildingType: 'factory' };

            drawGame();
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

        // Bound camera constraints so you can't pan infinitely off the edge of the map
        const maxPanX = canvas.width - (GRID_SIZE * CELL_SIZE);
        const maxPanY = canvas.height - (GRID_SIZE * CELL_SIZE);
        camera.x = Math.min(0, Math.max(maxPanX, camera.x));
        camera.y = Math.min(0, Math.max(maxPanY, camera.y));

        drawGame();
    }
});

<<<<<<< Updated upstream
window.addEventListener('mouseup', function() {
    camera.isDragging = false;
});
=======
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
            
            // Validate AP
            if (currentPlayer.ap <= 0) {
                showErrorPopup("Not enough Action Points (AP)!");
                selectedNodeForLink = null;
                return;
            }

            // Calculate Link Cost
            let dx = dest.c - selectedNodeForLink.c;
            let dy = dest.r - selectedNodeForLink.r;
            let dist = Math.sqrt(dx*dx + dy*dy);
            let tType = (source.type === 'Resource') ? "Resource" : "Energy";
            let rate = (tType === 'Resource') ? 2.0 : 1.0;
            let cost = dist * rate;
            
            if (currentPlayer.coins < cost) {
                showErrorPopup(`Not enough coins! Link costs 🪙${cost.toFixed(1)}.`);
                selectedNodeForLink = null;
                return;
            }

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
    const p = players[currentPlayerIndex];
    if (p.ap <= 0) {
        showErrorPopup("Not enough Action Points (AP)!");
        return;
    }

    const amountStr = prompt("How much waste would you like to dispose? (Cost: 3 Coins per unit, requires 1 AP)");
    if (!amountStr) return;
    
    const amount = parseFloat(amountStr);
    if (isNaN(amount) || amount <= 0) {
        alert("Invalid amount.");
        return;
    }

    if (p.coins < amount * 3.0) {
        showErrorPopup(`Not enough coins! Disposing ${amount} waste costs 🪙${amount * 3.0}.`);
        return;
    }

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
    
    // Validate AP
    if (currentPlayer.ap <= 0) {
        showErrorPopup("Not enough Action Points (AP)!");
        cancelBuild();
        return;
    }
    
    // Validate Coins
    const costs = {
        "Coal Plant": 80, "Gas Plant": 140, "Biomass Plant": 120, "Solar Plant": 160,
        "Wind Plant": 180, "Hydro Plant": 250, "Nuclear Plant": 1200,
        "Coal": 100, "Gas": 160, "Biomass": 180, "Uranium": 500
    };
    const cost = costs[subtype] || 0;
    if (currentPlayer.coins < cost) {
        showErrorPopup(`Not enough coins! ${subtype} costs 🪙${cost}.`);
        cancelBuild();
        return;
    }

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
>>>>>>> Stashed changes

// Bootstrap Setup Execution
initMap();
drawGame();