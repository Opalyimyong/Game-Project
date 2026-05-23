# Energy Not Included - Complete Playable Build

This build keeps the original idea: a C++ Mongoose backend, a menu page, a 50x50 canvas frontend, players, nodes, buildings, links, turn/AP/coin/waste state, and Resource → Power → City energy flow.

## Run

```bash
make
./energy_game_server
```

Open:

```text
http://localhost:18888
```

## Gameplay Loop

1. Start a 1-3 player game.
2. Click a Resource Node and build a Resource Plant.
3. Click a Power Node and build a Power Plant.
4. Use Link mode to connect Resource ↔ Power and Power ↔ City.
5. End Turn to process energy, city revenue, waste, and next player.
6. The game ends when every city reaches minimum energy; highest asset score wins.

## Controls

- Left-click: select node.
- Right-click or Shift-drag: pan map.
- Mouse wheel: zoom.
