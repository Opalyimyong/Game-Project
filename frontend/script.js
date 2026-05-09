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

function startGame() {
    window.location.href = 'gameplay.html';
}
