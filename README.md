# MoveControlHost
This is a host app for Ableton Move to act as a fake host in "Control Live" mode. It allows you to use the Ableton Move as a mappable MIDI control surface for any DAW/DJ software/VJ software/etcetera.

The actual functionality of this app is very simple: it just establishes a connection with the Move and writes an image to the LCD screen, to keep it from going to "sleep". Without this app, the LEDs on the Move go dark 1 second after you stop sending it MIDI messages. With this app running, the LEDs stay lit.

Here's a video explanation:  
<a href=https://youtu.be/3MFJ3M2qzuE>
<img width="800" height="448" alt="image" src="https://github.com/user-attachments/assets/357c461e-23c8-4e53-bd5b-fa137f2fd534" /></a>
(https://youtu.be/3MFJ3M2qzuE)  

This project could also serve as a minimal example for learning how to write to the Ableton Move LCD screen.
