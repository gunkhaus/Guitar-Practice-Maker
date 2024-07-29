# Guitar-Practice-Maker
An application that generates guitar OR bass tablature based on a user input or instrument and key. 

# Deeper Description
This project was a final for a college course. We were provided with a lot of freedom on what we could make, as long as it was ambitious enough, communicated to a server, and featured multithreading. Given my love for guitar and a website idea I had *concerning* guitar tablature, this seemed like a fun project to create. And it was!- Except for the multithreading part which i will talk about to a deeper extent down below. The code will generate 12 frets ( 1 octave ) worth of tablature, broken into 3 parts, in any key you want, in the major mode.

# The BIG problem with this code
If you know guitar, you know there's 12 frets in an octave. I split these frets into 3 sperate, "blocks," and used multithreading to compute and display these 3 blocks. I figured it would be a smart way to generate the tablature. It actually turned out to be a massive headache as I managed to create a plethora of race conditions. All the algorithmic work I did is most likely undone by the, "safety rail," code I had to set up in order to keep the final product displayed from being a gigantic MESS.

TL,DR: I used a sleep(1) statement to solve all my race condition related problems.

# What I Learned
Reinforced Skills in server client communication and multihreading.

Multithreading can be a detriment if not employed wisely.

Variables can get really expansive depending on how you need to use them.

Case Specific and/or Safety code can absolutely derail optimization.

Given the chance to do this again, I would probably skip multithreading this application as a whole- due to the minimal gain it would have in this circumstance. Even given the ,"perfect," situation where I didn't have to undo my codes optimization to solve an issue, it just isn't worth it in my opinion.
