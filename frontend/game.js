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

// Initialize the 50x50 2D Data Array (The source of truth)
let mapGrid = [];

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

window.addEventListener('mouseup', function() {
    camera.isDragging = false;
});

// Bootstrap Setup Execution
initMap();
drawGame();