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
	const string pwKorrekt = "Passwort korrekt. Datei wird geöffnet.";
	const string pwKurz = "Passwort zu kurz!";
	const string pwLang = "Passwort zu lang!";

	const string fehlerBeimSchreiben = "*****Fehler beim Schreiben der Datei!*****";
	const string fehlerBeimOeffnen = "*****Fehler beim Öffnen der Datei!*****";
	const string falscheEingabe = "*****Falsche Eingabe!*****";
	const string falschesFormat = "*****Falsches Format!*****";

	const string speichern = "Speichern? [j/n]";
	const string dateiGeoeffnet = "Datei geöffnet! :)";
	const string dateiBereitsVorhanden = "Datei bereits vorhanden!";
	const string dateiUeberschreiben = "Datei überschreiben? [j/n]";


	const string eintragAendernS = "Welchen Eintrag möchten Sie ändern?";
	const string eintragHinzufuegenS = "Möchten Sie weitere Einträge hinzufügen? [j/n]";
	const string eintragLoeschenS = "Welchen Eintrag möchten Sie löschen?";
	const string weitereLoeschen = "Weitere Einträge löschen? [j/n]";
	const string aenderungenVerworfen = "Änderungen werden verworfen.";
	const string eintragKopieren = "Welchen Eintrag möchten Sie kopieren?";

	const string abbrechenNull = "0.) Zurück";
	const string zurueckZumHauptmenue = "0.) Zurück zum Hauptmenü";
	const string pwInZwisch = "2.) Passwort in Zwischenablage kopieren";
	const string nameInZwisch = "1.) Benutzername in Zwischenablage kopieren";
	const string zurueck = "3.) Zurück";
	const string menueWasTun = "Was möchten Sie tun?";
	const string menue1 = "1.) Einträge ändern";
	const string menue2 = "2.) Einträge hinzufügen";
	const string menue3 = "3.) Einträge löschen";
	const string menue4 = "4.) Einträge anzeigen";
	const string menue5 = "5.) Einträge in Zwischenablage kopieren";
	const string menue6 = "6.) Datenbank schliessen";

	const string kopierName = "Klicken Sie in das Feld, in dem Sie den Benutzernamen haben möchten, und drücken Sie zum Einfügen Strg + v auf der Tastatur.";
	const string kopierPW = "Klicken Sie in das Feld, in dem Sie das Passwort haben möchten, und drücken Sie zum Einfügen Strg + v auf der Tastatur.";

	const string neueDB = "Neue Datenbank wird erstellt.";
	const string trenner = "******************************************************************";
	const string aesAlgo = "KeyManager verschlüsselt die Datenbank mit dem AES Algorithmus.";
	const string welcheModus = "Welchen Verschlüsselungsmodus möchten Sie verwenden?";
	const string vernuefpen = "AES verschlüsselt in 256-Bit Blöcken. Für eine höhere Sicherheit, können die einzelnen Blöcke miteinander \nverknüpft werden.";
	const string ebcs = "1) ECB";
	const string cbcs = "2) CBC (empfohlen)";
	const string abbrechenDrei = "3) Abbrechen";
	const string hilfeF = "?) Für Hilfe";

	const string modeHilfe1 ="AES verschlüsselt in 256-Bit-Blöcken. Für eine höhere Sicherheit können die einzelnen Blöcke miteinander verknüpft werden.";
	const string modeHilfe2 = "ECB - Electronic Codebook - Hier werden je 256 Bit als ein Block genommen und dieser verschlüsselt. Der Nachteil dieses Modus liegt darin dass ein bestimmter Text immer den gleichen verschlüsselten Text ergibt, wenn er mit demselben Schlüssel verschlüsselt wird.";
	const string modeHilfe3 = "CBC - Cypher Block Chaining - Hier werden die vorangegangenen Blöcke mit dem verschlüsselten Nachfolgerblock mit einer XOR-Verknüpfung verbunden. Somit ist der Block durch seine Vorgänger verändert und mit einem zufälligen Initialisierungsvektor immer anders, auch wenn derselbe Klartext mit demselben Schlüssel verschlüsselt wird.";
	const string taste = "Drücken Sie eine beliebige Taste . . .";

	const string IV1 = "CBC kann mit einem Initialisierungsvektor sicherer gemacht werden.";
	const string IV2 = "Möchten Sie einen Initialisierungsvektor erstellen?";
	const string IV2Menue1 = "1) Nein - IV wird mit NULL initialisiert";
	const string IV2Menue2 = "2) Ja (empfohlen) - mit Zufallszahl initialisieren";
	const string IV2Menue3 = "3) Ja - mit selbst eingegebenen Werten füllen";
	const string IV2Menue4 = "4) Zurück";
	const string IV2Menue5 = "5) Abbrechen";
	const string IVHilfe1 = "Der Initialisierungsvektor initialisiert die Verschlüsselungskette des CBC Modus.";
	const string IVHilfe2 = "Bei einem NULL-Initialisierungsvektor bleibt der erste Verschlüsselungsblock immer gleich. Erst darauffolgende Blöcke werden mit ihren Vorgängern verknüpft. Nachteil: Der erste Block des Chiffrats ist bei gleichem Text und Schlüssel immer gleich.";
	const string IVHilfe3 = "Bei einem zufallsgenerierten Initialisierungsvektor wird ein Block zufälliger Daten vorrangestellt. Somit ist der erste Block, der mit Daten gefüllt wird, mit einem Vektor zufälliger Daten verändert. Das Chiffrat ist hier jedes Mal ein anderes, auch wenn sich Text und Schlüssel nicht ändern.";
	const string IVHilfe4 = "Für den Initialisierungsvektor mit selbst eingegebenen Werten, gilt das Gleiche, wie bei den Zufallswerten. Diese sollten möglichst zufällig und jedes Mal anders sein. Diese müssen Sie sich nicht merken, um die Datenbank zu entschlüsseln.";

	const string padding1 = "Welche Paddingmethode möchten Sie verwenden?";
	const string padding2 = "1) Zero-Padding";
	const string padding3 = "2) ANSI X.923";
	const string padding4 = "3) PKCS7 (empfohlen)";
	const string padding5 = "4) Zurück";
	const string padding6 = "5) Abbrechen";

	const string paddingHilfe1 = "AES ist eine sogennante Blockchiffre, d. h. zum Verschlüsseln werden immer 256 Bit genommen und verschlüsselt.";
	const string paddingHilfe2 = "Wenn am Ende der Datei nicht mehr genügend Daten vorhanden sind, um einen Block zu füllen, müssen die restlichen Bits aufgefüllt werden.";
	const string paddingHilfe3 = "Hierfür gibt es verschiedene Standards:";
	const string paddingHilfe4 = "Zero-Padding - Blöcke werden mit Nullwerten gefüllt. Bei 4 nicht gefüllten Bytes werden alle mit Nullen gefüllt \n-> 00 00 00 00.";
	const string paddingHilfe5 = "ANSI X.923 - Blöcke werden mit Nullwerten gefüllt. Das letzte Byte zeigt wie viele Bytes mit Padding gefüllt wurden \n-> 00 00 00 04.";
	const string paddingHilfe6 = "PKCS7 - Alle Bytes zeigen wie viele Bytes mit Padding gefüllt wurden \n-> 04 04 04 04.";

	const string ersterEintrag1 = "Nun kommt der erste Eintrag  Ihrer Passwort-Datenbank.";
	const string ersterEintrag2 = "Dieser setzt sich aus dem Titel, dem Benutzername und dem Passwort zusammen.";

	const string IVEing1 = "Geben Sie bitte 16 beliebige Zeichen für den IV ein [min. 3 - max. 16 ASCII Zeichen]:";
	const string IVEing2 = "Diese müssen Sie sich nicht merken. Sie dienen nur als Startwert für die Verschlüsselung.";
	const string IVEing3 = "Eingabe zu kurz!";
	const string IVEing4 = "Eingabe zu lang!";

	const string DBAusg1 = "Inhalt der aktuellen Datenbank:";
	const string DBAusg2 = "------------------------------------------------------------------";
	const string DBAusg3 = "Index\tName\t\t\tBenutzername\t\tPasswort";
	const string DBAusg4 = "Es befindet sich aktuell ";
	const string DBAusg5 = " Eintrag in der Datenbank.";
	const string DBAusg6 = "Es befinden sich aktuell ";
	const string DBAusg7 = " Einträge in der Datenbank.";

	const string neuerEintrag1 = "Bitte geben Sie im Titel an, wofür das zu speichernde Passwort verwendet wird \n(wie z.B. telnet 192.168.0.5):";
	const string neuerEintrag2 = "Bitte geben Sie den Benutzernamen ein:";
	const string neuerEintrag3 = "Bitte geben Sie das Passwort oder die Zahl 1 für ein automatisch generiertes Passwort ein: [1]";
	const string neuerEintrag4 = "Wie viele Zeichen soll das Passwort haben?";

	const string welcome1 = "******************************";
	const string welcome2 = "*                            *";
	const string welcome3 = "* Willkommen bei KeyManager! *";

	const string hauptMenu1 = "Was möchten Sie tun?";
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
