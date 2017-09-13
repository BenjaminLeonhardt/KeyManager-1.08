## Key Manager

Version 1.08
Created on: 13.07.2016
Author: Benjamin Leonhardt

## !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Achtung bei diesem Projekt handelt es sich um ein Proof of Concept und sollte 
nicht für die sichere Verwahrung von Kennwörtern verwendet werden!

## !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

### Was ist KeyManager?

KeyManager ist ein Passwort Tressor. Es hilft Passwörter sicher aufzubewahren
und es können auch sichere zufall Passwörter generiert werden. 
KeyManager verschlüsselt die Datenbank mit dem AES-128-Algorithmus und
ist in der Lage AES mit verschiedenen Modis zu speichern. Bitte verwenden Sie 
für die größt mögliche Sicherheit die empfohlenen Einstellungen.

Der Quellcode ist in C++ geschrieben und kann in der aktuellen Version unter
Windows sowie Linux und unter x86/x64 sowie unter ARM kompiliert werden.

Unter Linux wird die Curses Library benötigt. Diese kann mit 
-> sudo apt-get install libncurses5-dev libncursesw5-dev
installiert werden. Zum kompilieren ist g++ ausreichend.
-> g++ KeyManager.cpp AES.cpp -lncurses -o KeyManager.out -std=c++11

Unter Windows habe ich Microsoft Visual Studio verwendet und sollte dort ohne 
Probleme kompilierbar sein.

Dieses Projekt ist wärhrend meiner Studienzeit entstanden und hat mir geholfen
Verschlüsselung zu verstehen. Der Code steht jedem zur freien Verfügung und 
kann gerne für Studienzwecke verwendet werden.

Im Quellcode ist auskommentiert eine Funktion für eine Brute Force Attacke. 
Diese Funktioniert allerdings nur für den ECB Modus und wurde aus interesse 
implementiert. Da ich die Funktion zu einem sehr frühen stadium implementiert 
habe und mich um das weiterentwickeln dieser nicht gekümmert habe, gebe ich 
auf die Funktion keine gewähr. Ich wollte allerdings ausdrücklich darauf 
hinweisen das diese existiert.

### Anleitung

Nach dem Start wird man mit einem Willkommensbildschirm begrüßt.
![Willkommensbildschirm](./Bilder/Willkommen1.PNG "Willkommensbildschirm")  

Hier besteht dir Möglichkeit eine neue Datenbank zu erstellen. 