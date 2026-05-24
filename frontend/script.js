const socket = new WebSocket('ws://localhost:18888/');

// Listen for the server's response
socket.onmessage = (event) => {
    const response = JSON.parse(event.data);
    if (response.status === "started") {
        console.log("Server is ready! Loading game...");
        // Now you can safely change the page or UI
        window.location.href = 'gameplay.html';
    }
};

let playerCount = 3;
const colors = ["#ff0044", "#0088ff", "#9900ff"]; // Red, Blue, Purple

function updatePlayerInputs() {
    const container = document.getElementById('player-inputs-container');
    container.innerHTML = '';
    for (let i = 0; i < playerCount; i++) {
        container.innerHTML += `
            <div class="player-input-row">
                <div class="color-box" style="background-color: ${colors[i]}"></div>
                <input type="text" id="player-name-${i}" placeholder="Player ${i+1} Name" value="Player ${i+1}">
            </div>
        `;
    }
}

function incrementPlayers() {
    if (playerCount < 3) {
        playerCount++;
        document.getElementById('player-val').textContent = playerCount;
        updatePlayerInputs();
    }
}

function decrementPlayers() {
    if (playerCount > 2) { // Minimum 2 players
        playerCount--;
        document.getElementById('player-val').textContent = playerCount;
        updatePlayerInputs();
    }
}

let timerMinutes = 10;

function incrementTimer() {
    if (timerMinutes < 60) {
        timerMinutes++;
        document.getElementById('timer-val').textContent = timerMinutes;
    }
}

function decrementTimer() {
    if (timerMinutes > 1) {
        timerMinutes--;
        document.getElementById('timer-val').textContent = timerMinutes;
    }
}

// THE FIXED START FUNCTION
function startGame() {
    // Save players to sessionStorage
    const playersData = [];
    for (let i = 0; i < playerCount; i++) {
        const name = document.getElementById(`player-name-${i}`).value || `Player ${i+1}`;
        playersData.push({ id: `p${i+1}`, name: name, color: colors[i], ap: 3 });
    }
    sessionStorage.setItem("gameTimerMinutes", timerMinutes);
    sessionStorage.setItem("gamePlayers", JSON.stringify(playersData));

    if (socket.readyState === WebSocket.OPEN) {
        const data = { 
            type: "START_GAME", 
            players: playersData,
            timer_minutes: timerMinutes,
            total_cities: 8
        };
        socket.send(JSON.stringify(data));
        console.log("Data sent to C++...");
    } else {
        console.error("Socket is not open yet! Proceeding to local play just in case.");
        window.location.href = 'gameplay.html'; // Fallback if C++ server is offline
    }
}

// Initialize on load
updatePlayerInputs();