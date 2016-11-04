/*	
*	Key Manager 
*	Version 1.05
*	Created on: 13.07.2016
*	Author: Benjamin Leonhardt
*	
*	KeyManager ist ein Passwort Tressor. Es hilft Passwörter sicher aufzubewahren
*	und es können auch sichere zufall Passwörter generiert werden. 
*	KeyManager verschlüsselt die Datenbank mit dem AES-256-Algorithmus und
*	ist in der Lage AES mit verschiedenen Modis zu speichern. Bitte verwenden Sie 
*	für die größt mögliche Sicherheit die empfohlenen Einstellungen.
*
*	Der Quellcode ist in C++ geschrieben und kann in der aktuellen Version unter
*	Windows sowie Linux und unter x86/x64 sowie unter ARM kompiliert werden.
*
*	Unter Linux wird die Curses Library benötigt. Diese kann mit 
*	-> sudo apt-get install libncurses5-dev libncursesw5-dev
*	installiert werden. Zum kompilieren ist g++ ausreichend.
*	-> g++ KeyManager.cpp AES.cpp -lncurses -o KeyManager.out -std=c++11
*
*	Unter Windows habe ich Microsoft Visual Studio verwendet und sollte dort ohne 
*	Probleme funktionieren.
*/
