# PokemonArena
A text-based, turn-based Pokemon battle game written in C++. A list of Pokemon along with their respective attributes and attacks are read out of a csv file to be chosen by the player. 4 Pokemon are chosen to comprise the player's team and the rest make up the arena. The player must battle through all the Pokemon in the arena to win the game.

Each Pokemon has the following attributes:
- health points
- energy
- type
- type weakness
- type resistance
- a number of attacks

Pokemon take increased damage from Pokemon of types they are weak against and reduced damage from Pokemon of types they resist. Energy is consumed when using attacks, and health points are consumed when being attacked. A Pokemon is considered defeated when its health points are depleted.

Attacks have the following attributes:
- cost
- damage
- special effect

An attack's cost is how much energy a Pokemon loses when using that attack, and damage is how many health points the enemy Pokemon will lose. 10 energy is refunded to each Pokemon after every turn. Some special effects directly benefit the attacking Pokemon while others directly hinder the receiving Pokemon.
