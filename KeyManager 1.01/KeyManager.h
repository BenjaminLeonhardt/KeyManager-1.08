/*
* KeyManager.h
*
*  Created on: 13.07.2016
*      Author: Benjamin Leonhardt
*/

#ifndef SRC_KEYMANAGER_H_
#define SRC_KEYMANAGER_H_

using namespace std;

class KeyManager {
public:
	KeyManager();
	KeyManager(int, int);
	virtual ~KeyManager();
	void openDB(string, string);
	void createDB();
	void bfDB();
	bool dateiEinlesen(string&);
	string passwortTest(string&,string&);
	string eintragAendern(string*, string, string);
	string eintragHinzufuegen(string*, string, string);
	void inZwischenablage(string*);
	string eintragLoeschen(string*, string, string);
	void eintragAnzeigen( string*);
	const unsigned char AE = static_cast<unsigned char>(142);
	const unsigned char ae = static_cast<unsigned char>(132);
	const unsigned char OE = static_cast<unsigned char>(153);
	const unsigned char oe = static_cast<unsigned char>(148);
	const unsigned char ss = static_cast<unsigned char>(225);
	const unsigned char UE = static_cast<unsigned char>(154);
	const unsigned char ue = static_cast<unsigned char>(129);

	int algo = 0;
	int Cmode = 0;		//0=ECB 1=CBC 2=CFB 3=OFB
	int Pmode = 0;		//0=Zero-Patting 1=ANSI X.923 2=pkcs7 3=ISO 10126 4=ISO/IEC 7816-4  
	int initVector = 0;
	const string version = "KeyManager 1.0";
	const string kms = "KeyManager";
	const string pwsign = "Passwort korrekt";

	const string dateieingabe = "Bitte geben Sie den Pfad und Dateiname der Datenbank-Datei ein:\n(z.B.: c:\\ordner\\dateiname)\n0)Abbrechen";
	const string masterPasswort = "Bitte geben Sie das Master-Passwort ein: [min 3 - max 16 ASCII Zeichen]";
	const string masterPasswortOeffnen = "Bitte geben Sie das Master-Passwort ein:";
	const string pwfalsch = "Das Passwort ist falsch ... Sie haben noch ";
	const string versuch = " Versuch.";
	const string versuche = " Versuche.";
	const string pwZuOft = "Passwort zu oft falsch eingegeben. Abbruch!";
	const string pwKorrekt = "Passwort korrekt. Datei wird ge�ffnet.";
	const string pwKurz = "Passwort zu kurz!";
	const string pwLang = "Passwort zu lang!";

	const string fehlerBeimSchreiben = "*****Fehler beim Schreiben der Datei!*****";
	const string fehlerBeimOeffnen = "*****Fehler beim �ffnen der Datei!*****";
	const string falscheEingabe = "*****Falsche Eingabe!*****";
	const string falschesFormat = "*****Falsches Format!*****";

	const string speichern = "Speichern? [j/n]";
	const string dateiGeoeffnet = "Datei ge�ffnet! :)";
	const string dateiBereitsVorhanden = "Datei bereits vorhanden!";
	const string dateiUeberschreiben = "Datei �berschreiben? [j/n]";


	const string eintragAendernS = "Welchen Eintrag m�chten Sie �ndern?";
	const string eintragHinzufuegenS = "M�chten Sie weitere Eintr�ge hinzuf�gen? [j/n]";
	const string eintragLoeschenS = "Welchen Eintrag m�chten Sie l�schen?";
	const string weitereLoeschen = "Weitere Eintr�ge l�schen? [j/n]";
	const string aenderungenVerworfen = "�nderungen werden verworfen.";
	const string eintragKopieren = "Welchen Eintrag m�chten Sie kopieren?";

	const string abbrechenNull = "0.) Zur�ck";
	const string zurueckZumHauptmenue = "0.) Zur�ck zum Hauptmen�";
	const string pwInZwisch = "2.) Passwort in Zwischenablage kopieren";
	const string nameInZwisch = "1.) Benutzername in Zwischenablage kopieren";
	const string zurueck = "3.) Zur�ck";
	const string menueWasTun = "Was m�chten Sie tun?";
	const string menue1 = "1.) Eintr�ge �ndern";
	const string menue2 = "2.) Eintr�ge hinzuf�gen";
	const string menue3 = "3.) Eintr�ge l�schen";
	const string menue4 = "4.) Eintr�ge anzeigen";
	const string menue5 = "5.) Eintr�ge in Zwischenablage kopieren";
	const string menue6 = "6.) Datenbank schliessen";

	const string kopierName = "Klicken Sie in das Feld, in dem Sie den Benutzernamen haben m�chten, und dr�cken Sie zum Einf�gen Strg + v auf der Tastatur.";
	const string kopierPW = "Klicken Sie in das Feld, in dem Sie das Passwort haben m�chten, und dr�cken Sie zum Einf�gen Strg + v auf der Tastatur.";

	const string neueDB = "Neue Datenbank wird erstellt.";
	const string trenner = "******************************************************************";
	const string aesAlgo = "KeyManager verschl�sselt die Datenbank mit dem AES Algorithmus.";
	const string welcheModus = "Welchen Verschl�sselungsmodus m�chten Sie verwenden?";
	const string vernuefpen = "AES verschl�sselt in 128-Bit Bl�cken. F�r eine h�here Sicherheit, k�nnen die einzelnen Bl�cke miteinander \nverkn�pft werden.";
	const string ebcs = "1) ECB";
	const string cbcs = "2) CBC (empfohlen)";
	const string abbrechenDrei = "3) Abbrechen";
	const string hilfeF = "?) F�r Hilfe";

	const string modeHilfe1 ="AES verschl�sselt in 128-Bit-Bl�cken. F�r eine h�here Sicherheit k�nnen die einzelnen Bl�cke miteinander verkn�pft \nwerden.";
	const string modeHilfe2 = "ECB - Electronic Codebook - Hier werden je 128 Bit als ein Block genommen und dieser verschl�sselt. Mehrere Bl�cke \nwerden hintereinander geh�ngt. Der Nachteil dieses Modus liegt darin dass ein bestimmter Text immer den gleichen \nverschl�sselten Text ergibt, wenn er mit demselben Schl�ssel verschl�sselt wird.";
	const string modeHilfe3 = "CBC - Cypher Block Chaining - Hier werden die vorangegangenen Bl�cke mit dem verschl�sselten Nachfolgerblock mit einer \nXOR-Verkn�pfung verbunden. Somit ist der Block durch seine Vorg�nger ver�ndert und mit einem zuf�lligen \nInitialisierungsvektor immer anders, auch wenn derselbe Klartext mit demselben Schl�ssel verschl�sselt wird.";
	const string taste = "Dr�cken Sie eine beliebige Taste . . .";

	const string IV1 = "CBC kann mit einem Initialisierungsvektor sicherer gemacht werden.";
	const string IV2 = "M�chten Sie einen Initialisierungsvektor erstellen?";
	const string IV2Menue1 = "1) Nein - IV wird mit NULL initialisiert";
	const string IV2Menue2 = "2) Ja (empfohlen) - mit Zufallszahl initialisieren";
	const string IV2Menue3 = "3) Ja - mit selbst eingegebenen Werten f�llen";
	const string IV2Menue4 = "4) Zur�ck";
	const string IV2Menue5 = "5) Abbrechen";
	const string IVHilfe1 = "Der Initialisierungsvektor initialisiert die Verschl�sselungskette des CBC Modus.";
	const string IVHilfe2 = "Bei einem NULL-Initialisierungsvektor bleibt der erste Verschl�sselungsblock immer gleich. Erst darauffolgende Bl�cke \nwerden mit ihren Vorg�ngern verkn�pft. Nachteil: Der erste Block des Chiffrats ist bei gleichem Text und Schl�ssel \nimmer gleich.";
	const string IVHilfe3 = "Bei einem zufallsgenerierten Initialisierungsvektor wird ein Block zuf�lliger Daten vorrangestellt. Somit ist der erste Block, der mit Daten gef�llt wird, mit einem Vektor zuf�lliger Daten ver�ndert. Das Chiffrat ist hier jedes Mal ein \nanderes, auch wenn sich Text und Schl�ssel nicht �ndern.";
	const string IVHilfe4 = "F�r den Initialisierungsvektor mit selbst eingegebenen Werten, gilt das Gleiche, wie bei den Zufallswerten. Diese \nsollten m�glichst zuf�llig und jedes Mal anders sein. Diese m�ssen Sie sich nicht merken, um die Datenbank zu \nentschl�sseln.";

	const string padding1 = "Welche Paddingmethode m�chten Sie verwenden?";
	const string padding2 = "1) Zero-Padding";
	const string padding3 = "2) ANSI X.923";
	const string padding4 = "3) PKCS7 (empfohlen)";
	const string padding5 = "4) Zur�ck";
	const string padding6 = "5) Abbrechen";

	const string paddingHilfe1 = "AES ist eine sogennante Blockchiffre, d.h. zum Verschl�sseln werden immer 128 Bit genommen und verschl�sselt.";
	const string paddingHilfe2 = "Wenn am Ende der Datei nicht mehr gen�gend Daten vorhanden sind, um einen Block zu f�llen, m�ssen die restlichen Bits \naufgef�llt werden.";
	const string paddingHilfe3 = "Hierf�r gibt es verschiedene Standards:";
	const string paddingHilfe4 = "Zero-Padding - Bl�cke werden mit Nullwerten gef�llt. Bei 4 nicht gef�llten Bytes werden alle mit Nullen gef�llt \n-> 00 00 00 00.";
	const string paddingHilfe5 = "ANSI X.923 - Bl�cke werden mit Nullwerten gef�llt. Das letzte Byte zeigt wie viele Bytes mit Padding gef�llt wurden \n-> 00 00 00 04.";
	const string paddingHilfe6 = "PKCS7 - Alle Bytes zeigen wie viele Bytes mit Padding gef�llt wurden \n-> 04 04 04 04.";

	const string ersterEintrag1 = "Nun kommt der erste Eintrag Ihrer Passwort-Datenbank.";
	const string ersterEintrag2 = "Dieser setzt sich aus dem Titel, dem Benutzername und dem Passwort zusammen.";

	const string IVEing1 = "Geben Sie bitte 16 beliebige Zeichen f�r den IV ein [min. 3 - max. 16 ASCII Zeichen]:";
	const string IVEing2 = "Diese m�ssen Sie sich nicht merken. Sie dienen nur als Startwert f�r die Verschl�sselung.";
	const string IVEing3 = "Eingabe zu kurz!";
	const string IVEing4 = "Eingabe zu lang!";

	const string DBAusg1 = "Inhalt der aktuellen Datenbank:";
	const string DBAusg2 = "------------------------------------------------------------------";
	const string DBAusg3 = "Index\tName\t\t\tBenutzername\t\tPasswort";
	const string DBAusg4 = "Es befindet sich aktuell ";
	const string DBAusg5 = " Eintrag in der Datenbank.";
	const string DBAusg6 = "Es befinden sich aktuell ";
	const string DBAusg7 = " Eintr�ge in der Datenbank.";

	const string neuerEintrag1 = "Bitte geben Sie im Titel an, wof�r das zu speichernde Passwort verwendet wird \n(wie z.B. ssh 192.168.0.5):";
	const string neuerEintrag2 = "Bitte geben Sie den Benutzernamen ein:";
	const string neuerEintrag3 = "Bitte geben Sie das Passwort oder die Zahl 1 f�r ein automatisch generiertes Passwort ein: [1]";
	const string neuerEintrag4 = "Wie viele Zeichen soll das Passwort haben?";

	const string welcome1 = "******************************";
	const string welcome2 = "*                            *";
	const string welcome3 = "* Willkommen bei KeyManager! *";

	const string hauptMenu1 = "Was m�chten Sie tun?";
	const string hauptMenu2 = "1.) Neue KeyManager-Datenbank anlegen";
	const string hauptMenu3 = "2.) Bestehende KeyManager-Datenbank laden";
	const string hauptMenu4 = "3.) Beenden";
	const string hauptMenu5 = "4.) Bruteforce Datenbank";

	const char dbTrenner = (char)3;
	bool klartext=false;

private:
	void DBbearbeiten(string dateiname);
	void DBAufHDDSchreiben(string txt, string dateiname, string pw, bool ueberschreiben);
	unsigned int DBAusgabe(string txt);
	string NeuerEintrag();
	void insClipboard(string s);
	void padding(string&, wort*,int);
	string pwEingabe(void);


};

#endif /* SRC_KEYMANAGER_H_ */
