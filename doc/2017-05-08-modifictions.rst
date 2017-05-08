****************
New Requirements
****************

The two bar graphs showing each player's generated energy will be repaced with a map display with 10 "cities". Each city is a NeoPixel ring with 24 LEDs on it, daisy-chained in such a way that the first 24 LEDs in strip 1 will refer to city 1, the next 24 to city 2 and so on. First first five cities will map onto LED1, the second 5 cities onto LED2.

Each city has an energy level. When one of the player's energy levels exceeds this value, the city will be illuminated with the colour of the player who first exceeded that city's colour.

At the end of the race, the winner is the player who has illuminated the most cities. If the number of cities is equal, the final accumulated energy levels for the players will be used to determine the winner.

