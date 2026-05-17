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

function incrementPlayers() {
    if (playerCount < 3) {
        playerCount++;
        document.getElementById('player-val').textContent = playerCount;
    }
}

function decrementPlayers() {
    if (playerCount > 1) {
        playerCount--;
        document.getElementById('player-val').textContent = playerCount;
    }
}

// THE FIXED START FUNCTION
function startGame() {
    if (socket.readyState === WebSocket.OPEN) {
        const data = { 
            type: "START_GAME", 
            players: playerCount // Use the actual variable from your UI
        };
        socket.send(JSON.stringify(data));
        console.log("Data sent to C++...");
    } else {
        console.error("Socket is not open yet!");
    }
}