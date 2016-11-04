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

#include "stdafx.h"
#include "KeyManager.h"
#ifdef _WIN32
#include <conio.h>
#elif __linux
#include <curses.h>
#endif // 


using namespace std;

//Standard Konstruktor
KeyManager::KeyManager() {
}
//Konstruktor
KeyManager::KeyManager(int _Cmode, int _Pmode) :Cmode(_Cmode), Pmode(_Pmode), initVector(0), algo(1) {
}
//Destruktor
KeyManager::~KeyManager() {
}

/*	Diese Funktion ist zum Einlesen der Datei.
*	Sie liest die Datei und Prüft an Hand des Datei-Headers ob es auch eine KeyManager-Datenbank-Datei ist. 
*/
bool KeyManager::dateiEinlesen(string& pfad) {
	bool DateiEinlesen = true;
	ifstream datei;

	while (DateiEinlesen) {
		cout << dateieingabe << endl;
		cin >> pfad;
		if (pfad[0] == '0') {
			return true;
		}
		else if (pfad[0] == '1') {
			pfad = "c:/pa/test";
			cout << "Die Datei " << pfad << " wird eingelesen!" << endl;
			datei.open(pfad.c_str(), ios::in | ios::ate | ios::binary);
			if (datei.is_open()) {
				bool formatKorrekt = true;
				char check[16];
				for (unsigned int i = 0; i < 16; i++) {
					check[i] = 0;
				}
				string kmsign = kms;
				datei.seekg(0, ios::beg);
				datei.read(check, 16);
				for (unsigned int i = 0; i < kmsign.length(); i++) {
					if (check[i] != kmsign[i]) {
						formatKorrekt = false;
						i = kmsign.length();
					}
				}
				algo = (check[14] >> 6);
				Cmode = 0x07 & (check[14] >> 3);
				Pmode = 0x03 & (check[14]);
				initVector = check[15];
				if (formatKorrekt) {
					cout << dateiGeoeffnet << endl;
					DateiEinlesen = false;
				}
				else {
					cout << falschesFormat << endl;
				}
			}
			else {
				cout << fehlerBeimOeffnen << endl;
			}
		}
		else {
			cout << "Die Datei " << pfad << " wird eingelesen!" << endl;
			datei.open(pfad.c_str(), ios::in | ios::ate | ios::binary);
			if (!datei.is_open()) {
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				cout << fehlerBeimOeffnen << endl;
				return true;
			}
			datei.seekg(0, ios::beg);
			if (datei.is_open()) {
				bool formatKorrekt = true;
				char check[16];
				string kmsign = kms;
				datei.read(check, 16);
				for (unsigned int i = 0; i < kmsign.length(); i++) {
					if (check[i] != kmsign[i]) {
						formatKorrekt = false;
						i = kmsign.length();
					}
				}
				algo = (check[14] >> 6);
				Cmode = 0x07 & (check[14] >> 3);
				Pmode = 0x03 & (check[14]);
				initVector = check[15];
				if (formatKorrekt) {
					cout << dateiGeoeffnet << endl;
					DateiEinlesen = false;
				}
				else {
					cout << falschesFormat << endl;
					datei.close();
				}
			}
			else {
				cout << fehlerBeimOeffnen << endl;
				datei.close();
			}
		}
	}
	return false;
}

/*	Diese Funktion prüft ob das eingegebene Passwort korrekt ist.
*/
string KeyManager::passwortTest(string& pfad,string& pwo) {
	ifstream datei;
	datei.open(pfad.c_str(), ios::binary);
	string mpw;
	cout << masterPasswortOeffnen << endl;
	if (pwo.length() == 0) {
		mpw = pwEingabe();
	}
	else {
		mpw = pwo;
	}
	cout << mpw << endl;
	AES aes(mpw);
	wort w[4];
	wort out[4];
	bool pwCheck = true;
	int count = 1;
	char buf[16];
	char check[16];
	while (pwCheck&&count < 4) {
		if (Cmode == 0) {
			datei.seekg(16, ios::beg);
			datei.read(buf, 16);

			w[0].char0 = buf[0];	w[0].char1 = buf[1];	w[0].char2 = buf[2];	w[0].char3 = buf[3];
			w[1].char0 = buf[4];	w[1].char1 = buf[5];	w[1].char2 = buf[6];	w[1].char3 = buf[7];
			w[2].char0 = buf[8];	w[2].char1 = buf[9];	w[2].char2 = buf[10];	w[2].char3 = buf[11];
			w[3].char0 = buf[12];	w[3].char1 = buf[13];	w[3].char2 = buf[14];	w[3].char3 = buf[15];

			aes.setState(w[0].dasWort, w[1].dasWort, w[2].dasWort, w[3].dasWort);
			aes.entschluesseln();


			if (!datei.eof()) {
				out[0].dasWort = aes.getS(0);	check[0] = out[0].char0;	check[1] = out[0].char1;	check[2] = out[0].char2;	check[3] = out[0].char3;
				out[1].dasWort = aes.getS(1);	check[4] = out[1].char0;	check[5] = out[1].char1;	check[6] = out[1].char2;	check[7] = out[1].char3;
				out[2].dasWort = aes.getS(2);	check[8] = out[2].char0;	check[9] = out[2].char1;	check[10] = out[2].char2;	check[11] = out[2].char3;
				out[3].dasWort = aes.getS(3);	check[12] = out[3].char0;	check[13] = out[3].char1;	check[14] = out[3].char2;	check[15] = out[3].char3;
			}
		}
		else if (Cmode == 1) {
			//IV == 0
			if (initVector == 0) {
				datei.seekg(16, ios::beg);
				datei.read(buf, 16);

				w[0].char0 = buf[0];	w[0].char1 = buf[1];	w[0].char2 = buf[2];	w[0].char3 = buf[3];
				w[1].char0 = buf[4];	w[1].char1 = buf[5];	w[1].char2 = buf[6];	w[1].char3 = buf[7];
				w[2].char0 = buf[8];	w[2].char1 = buf[9];	w[2].char2 = buf[10];	w[2].char3 = buf[11];
				w[3].char0 = buf[12];	w[3].char1 = buf[13];	w[3].char2 = buf[14];	w[3].char3 = buf[15];

				aes.setState(w[0].dasWort, w[1].dasWort, w[2].dasWort, w[3].dasWort);
				aes.entschluesseln();


				if (!datei.eof()) {
					out[0].dasWort = aes.getS(0);	check[0] = out[0].char0;	check[1] = out[0].char1;	check[2] = out[0].char2;	check[3] = out[0].char3;
					out[1].dasWort = aes.getS(1);	check[4] = out[1].char0;	check[5] = out[1].char1;	check[6] = out[1].char2;	check[7] = out[1].char3;
					out[2].dasWort = aes.getS(2);	check[8] = out[2].char0;	check[9] = out[2].char1;	check[10] = out[2].char2;	check[11] = out[2].char3;
					out[3].dasWort = aes.getS(3);	check[12] = out[3].char0;	check[13] = out[3].char1;	check[14] = out[3].char2;	check[15] = out[3].char3;
				}
			}
			else if (initVector == 1 || initVector == 2) {
				datei.seekg(16, ios::beg);
				datei.read(buf, 16);
				wort initBlock[4];
				initBlock[0].char0 = buf[0];	initBlock[0].char1 = buf[1];	initBlock[0].char2 = buf[2];	initBlock[0].char3 = buf[3];
				initBlock[1].char0 = buf[4];	initBlock[1].char1 = buf[5];	initBlock[1].char2 = buf[6];	initBlock[1].char3 = buf[7];
				initBlock[2].char0 = buf[8];	initBlock[2].char1 = buf[9];	initBlock[2].char2 = buf[10];	initBlock[2].char3 = buf[11];
				initBlock[3].char0 = buf[12];	initBlock[3].char1 = buf[13];	initBlock[3].char2 = buf[14];	initBlock[3].char3 = buf[15];

				datei.read(buf, 16);

				w[0].char0 = buf[0];	w[0].char1 = buf[1];	w[0].char2 = buf[2];	w[0].char3 = buf[3];
				w[1].char0 = buf[4];	w[1].char1 = buf[5];	w[1].char2 = buf[6];	w[1].char3 = buf[7];
				w[2].char0 = buf[8];	w[2].char1 = buf[9];	w[2].char2 = buf[10];	w[2].char3 = buf[11];
				w[3].char0 = buf[12];	w[3].char1 = buf[13];	w[3].char2 = buf[14];	w[3].char3 = buf[15];

				aes.setState(w[0].dasWort, w[1].dasWort, w[2].dasWort, w[3].dasWort);
				aes.entschluesseln();


				if (!datei.eof()) {
					out[0].dasWort = aes.getS(0) ^ initBlock[0].dasWort;	check[0] = out[0].char0;	check[1] = out[0].char1;	check[2] = out[0].char2;	check[3] = out[0].char3;
					out[1].dasWort = aes.getS(1) ^ initBlock[1].dasWort;	check[4] = out[1].char0;	check[5] = out[1].char1;	check[6] = out[1].char2;	check[7] = out[1].char3;
					out[2].dasWort = aes.getS(2) ^ initBlock[2].dasWort;	check[8] = out[2].char0;	check[9] = out[2].char1;	check[10] = out[2].char2;	check[11] = out[2].char3;
					out[3].dasWort = aes.getS(3) ^ initBlock[3].dasWort;	check[12] = out[3].char0;	check[13] = out[3].char1;	check[14] = out[3].char2;	check[15] = out[3].char3;
				}
			}
		}

		string kmsign = pwsign;
		bool passwortKorrekt = true;

		for (unsigned int i = 0; i < kmsign.length(); i++) {
			if (check[i] != kmsign[i]) {
				passwortKorrekt = false;
				i = kmsign.length();
			}
		}
		if (!passwortKorrekt) {
			if (count < 2) {
				cout << pwfalsch << 3 - count << versuche << endl;
			}
			else {
				cout << pwfalsch << 3 - count << versuch << endl;
			}
			cout << masterPasswort << endl;
			cin >> mpw;
			aes.setKey(mpw);
		}
		else {
			cout << pwKorrekt << endl;
			pwCheck = false;
		}
		count++;
		if (count == 3) {
			cout << pwZuOft << endl;
			return "";
		}
	}
	datei.close();
	return mpw;
}

void KeyManager::eintragAnzeigen(string* txt) {
	bool eintrAnz = true;
	
	vector<string> txtLines2;
	int iAlt = 0;
	int index = 0;
	for (unsigned int i = 0; i < txt->length(); i++) {
		if (txt[0][i] == '\n') {
			string s = txt->substr(iAlt, i);
			s.resize((i - iAlt));
			txtLines2.push_back(s);
			iAlt = i + 1;
			index++;
		}
	}
	while (eintrAnz) {
		int anzEintr = DBAusgabe(*txt);
		cout << endl << "Welchen Eintrag möchten Sie anzeigen lassen?" << " [1-" << anzEintr << "]" << endl;
		cout << "0.) Zurück" << endl;
		string eing;
		string textNeu;
		cin >> eing;
		int eingi = atoi(eing.c_str());
#ifdef _WIN32
		system("cls");
#elif __linux
		system("clear");
#endif
		if (eing.length() == 1 && (eingi<10 && eingi>0)) {

			if (eingi - 1 < txtLines2.size()) {
				textNeu = txtLines2[eingi - 1];
				string titel;
				string name;
				string pw;
				int i1 = 0;
				int i2 = 0;
				int i3 = 0;
				for (int i = 0; i < textNeu.length(); i++) {
					if (textNeu[i] == 3) {
						i1 = i++;
						i = textNeu.length();
					}
				}
				for (int i = i1 + 1; i < textNeu.length(); i++) {
					if (textNeu[i] == 3) {
						i2 = i++;
						i = textNeu.length();
					}
				}
				titel = textNeu.substr(i1 + 1, (i2 - i1) - 1);
				for (int i = i2 + 1; i < textNeu.length(); i++) {
					if (textNeu[i] == 3) {
						i3 = i++;
						i = textNeu.length();
					}
				}
				name = textNeu.substr(i2 + 1, (i3 - i2) - 1);
				pw = textNeu.substr(i3 + 1);
				cout << endl << endl << DBAusg2 << endl;
				cout << DBAusg3 << endl;
				cout << DBAusg2 << endl;
				string t1 = titel.length() > 5 ? "\t" : "\t\t";
				string t2 = name.length() > 5 ? "\t" : "\t\t";
				cout << endl << eingi << "\t" << titel << t1 << name << t2 << pw << endl;
				cout << DBAusg2 << endl << endl << endl;
				bool zur = true;
				while (zur) {
					cout << "0.) Zurück" << endl;
					string zurEing;
					cin >> zurEing;
					if (zurEing[0] == '0' &&zurEing.length() == 1) {
#ifdef _WIN32
						system("cls");
#elif __linux
						system("clear");
#endif
						zur = false;
					}
					else {
						cout << falscheEingabe << endl;
					}
				}
			}
			else {
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				cout << falscheEingabe << endl;
			}
		}
		else if(eing.length() == 2 && (eingi<100 && eingi>=10)){
			if (eingi - 1 < txtLines2.size()) {
				textNeu = txtLines2[eingi - 1];
				string titel;
				string name;
				string pw;
				int i1 = 0;
				int i2 = 0;
				int i3 = 0;
				for (int i = 0; i < textNeu.length(); i++) {
					if (textNeu[i] == 3) {
						i1 = i++;
						i = textNeu.length();
					}
				}
				for (int i = i1 + 1; i < textNeu.length(); i++) {
					if (textNeu[i] == 3) {
						i2 = i++;
						i = textNeu.length();
					}
				}
				titel = textNeu.substr(i1 + 1, (i2 - i1) - 1);
				for (int i = i2 + 1; i < textNeu.length(); i++) {
					if (textNeu[i] == 3) {
						i3 = i++;
						i = textNeu.length();
					}
				}
				name = textNeu.substr(i2 + 1, (i3 - i2) - 1);
				pw = textNeu.substr(i3 + 1);
				cout << endl << endl << DBAusg2 << endl;
				cout << DBAusg3 << endl;
				cout << DBAusg2 << endl;
				string t1 = titel.length() > 5 ? "\t" : "\t\t";
				string t2 = name.length() > 5 ? "\t" : "\t\t";
				cout << endl << eingi << "\t" << titel << t1 << name << t2 << pw << endl;
				cout << DBAusg2 << endl << endl << endl;
				bool zur = true;
				while (zur) {
					cout << "0.) Zurück" << endl;
					string zurEing;
					cin >> zurEing;
					if (zurEing[0] == '0' &&zurEing.length() == 1) {
#ifdef _WIN32
						system("cls");
#elif __linux
						system("clear");
#endif
						zur = false;
					}
					else {
						cout << falscheEingabe << endl;
					}
				}
			}
			else {
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				cout << falscheEingabe << endl;
			}
		}
		else if (eing.length() == 1 && eing[0] == '0') {
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			return;
		}
		else {
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			cout << falscheEingabe << endl;
		}
	}
}

/*	Diese Funktion wird aufgerufen, wenn man einen Eintrag in der Datenbank ändern will.
*/
string KeyManager::eintragAendern(string* txt, const string pfad, const string mpw) {
	string tmp;
	tmp.append(*txt);
	unsigned int iAlt = 0;
	int index = 0;
	string textNeu;
	vector<string> txtLines2;
	char indc;
	string inds;
	unsigned ind;
	unsigned int anzEintr = 0;
	bool eintrEing = true;
	int b = 0;
	while (eintrEing) {
		anzEintr = DBAusgabe(*txt);
		cout << eintragAendernS << " [1-" << anzEintr << "]" << endl;
		cout << abbrechenNull << endl;
		inds.clear();
		cin >> inds;
		b = atoi(inds.c_str());
		if (b != 0 || inds[0] == 48) {
			ind = b;
			if (b < 10 && inds.length() > 1) {
				ind = -1;
			}
			if (b >= 10 && inds.length() > 2) {
				ind = -1;
			}
			if (ind < 0 || inds[0] == 48 && inds.length() > 1) {
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				cout << endl << falscheEingabe << endl << endl << endl;
			}
			else if (ind > anzEintr) {
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				cout << endl << falscheEingabe << endl << endl << endl;
			}
			else if (inds[0] == 48&&inds.length()<=1) {
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				return *txt;
			}
			else {
				//eintrEing = false;
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
			textNeu.clear();
			txtLines2.clear();
			iAlt = 0;
			for (unsigned int i = 0; i < txt->length(); i++) {
				if (txt[0][i] == '\n') {
					string s = txt->substr(iAlt, i);
					s.resize((i - iAlt));
					txtLines2.push_back(s);
					iAlt = i + 1;
					index++;
				}
			}
			textNeu = txtLines2[ind - 1];
			txtLines2[ind - 1].clear();
			char z[4];
			if (ind < 10) {
				z[0] = dbTrenner; z[1] = 0; z[2] = 0; z[3] = 0;
			}
			else if (ind >= 10) {
				z[0] = dbTrenner; z[1] = 0; z[2] = 0; z[3] = 0;
			}
			txtLines2[ind - 1].append(z);
			string titel;
			string name;
			string pw;
			int i1 = 0;
			int i2 = 0;
			int i3 = 0;
			for (int i = 0; i < textNeu.length(); i++) {
				if (textNeu[i] == 3) {
					i1 = i++;
					i = textNeu.length();
				}
			}
			for (int i = i1+1; i < textNeu.length(); i++) {
				if (textNeu[i] == 3) {
					i2 = i++;
					i = textNeu.length();
				}
			}
			titel = textNeu.substr(i1+1, (i2 - i1)-1);
			for (int i = i2+1; i < textNeu.length(); i++) {
				if (textNeu[i] == 3) {
					i3 = i++;
					i = textNeu.length();
				}
			}
			name= textNeu.substr(i2+1, (i3 - i2)-1);
			pw=textNeu.substr(i3+1);

			bool tbp = true;
			while (tbp) {
		
				cout << endl << endl << DBAusg2 << endl;
				cout << DBAusg3 << endl;
				cout << DBAusg2 << endl;
				string t1 = titel.length() > 5 ? "\t" : "\t\t";
				string t2 = name.length() > 5 ? "\t" : "\t\t";
				cout << endl << ind << "\t" << titel << t1 << name << t2 << pw << endl;
				cout << DBAusg2 << endl << endl << endl;
				string was;
				int b = 0;
				cout << "Was möchten Sie ändern?" << endl;
				cout << "1.) Titel" << endl;
				cout << "2.) Benutzername" << endl;
				cout << "3.) Passwort" << endl;
				cout << "4.) Alles" << endl;
				cout << "0.) Speichern" << endl;
				cin >> was;
				if (was.length() > 1) {
		#ifdef _WIN32
					system("cls");
		#elif __linux
					system("clear");
		#endif
					cout << falscheEingabe << endl;
				}
				else if (was[0] == 49) {
#ifdef _WIN32
					system("cls");
#elif __linux
					system("clear");
#endif
					txtLines2[ind - 1].clear();
					cout << neuerEintrag1 << endl;
					cin.ignore();
					getline(cin, titel);
					//cin >> titel;
					txtLines2[ind - 1].append(1,dbTrenner);
					txtLines2[ind - 1].append(titel);
					txtLines2[ind - 1].append(1, dbTrenner);
					txtLines2[ind - 1].append(name);
					txtLines2[ind - 1].append(1,dbTrenner);
					txtLines2[ind - 1].append(pw);
				}
				else if (was[0] == 50) {
#ifdef _WIN32
					system("cls");
#elif __linux
					system("clear");
#endif
					txtLines2[ind - 1].clear();
					cout << neuerEintrag2 << endl;
					cin >> name;
					txtLines2[ind - 1].append(1, dbTrenner);
					txtLines2[ind - 1].append(titel);
					txtLines2[ind - 1].append(1, dbTrenner);
					txtLines2[ind - 1].append(name);
					txtLines2[ind - 1].append(1, dbTrenner);
					txtLines2[ind - 1].append(pw);
				}
				else if (was[0] == 51) {
#ifdef _WIN32
					system("cls");
#elif __linux
					system("clear");
#endif
					txtLines2[ind - 1].clear();
					cout << neuerEintrag3 << endl;
					cin >> pw;
					if (pw.length() == 1 && pw[0] == '1') {
						pw.clear();

						string l;
						bool pwl = true;
						while (pwl) {
							int laenge;
							cout << neuerEintrag4 << endl;
							cin >> l;
							laenge = atoi(l.c_str());
							if (laenge != 0) {
								if (laenge < 10 && l.size() == 1) {
									pw.resize(laenge);
									int tmpr;
									srand(time(0));
									for (unsigned int i = 0; i < laenge;) {
										tmpr = 1 + rand() % 122;
										if ((tmpr > 47 && tmpr < 58) || (tmpr > 64 && tmpr < 91) || (tmpr > 96 && tmpr < 123)) {
											pw[i] = (char)tmpr;
											i++;
										}
									}
									pwl = false;
								}else if(laenge >= 10 && l.size() == 2){
									pw.resize(laenge);
									int tmpr;
									srand(time(0));
									for (unsigned int i = 0; i < laenge;) {
										tmpr = 1 + rand() % 122;
										if ((tmpr > 47 && tmpr < 58) || (tmpr > 64 && tmpr < 91) || (tmpr > 96 && tmpr < 123)) {
											pw[i] = (char)tmpr;
											i++;
										}
									}
									pwl = false;
								}
								else {
									cout << falscheEingabe << endl;
								}
							}
							else {
		#ifdef _WIN32
								system("cls");
		#elif __linux
								system("clear");
		#endif
								cout << falscheEingabe << endl;
							}
						}
					}
					txtLines2[ind - 1].append(1, dbTrenner);
					txtLines2[ind - 1].append(titel);
					txtLines2[ind - 1].append(1, dbTrenner);
					txtLines2[ind - 1].append(name);
					txtLines2[ind - 1].append(1, dbTrenner);
					txtLines2[ind - 1].append(pw);
				}
				else if (was[0] == 52) {
#ifdef _WIN32
					system("cls");
#elif __linux
					system("clear");
#endif
					txtLines2[ind - 1].clear();
					txtLines2[ind - 1].append(NeuerEintrag());
				}
				else if (was[0] == 48) {
#ifdef _WIN32
					system("cls");
#elif __linux
					system("clear");
#endif
					txtLines2[ind - 1].clear();
					txtLines2[ind - 1].append(1, dbTrenner);
					txtLines2[ind - 1].append(titel);
					txtLines2[ind - 1].append(1, dbTrenner);
					txtLines2[ind - 1].append(name);
					txtLines2[ind - 1].append(1, dbTrenner);
					txtLines2[ind - 1].append(pw);
					tbp = false;
				}
			}

			//txtLines2[ind - 1].resize(txtLines2[ind - 1].length() - 1);
			txt->clear();

			for (unsigned int g = 0; g < anzEintr; g++) {
				txt->append(txtLines2[g]);
				txt->append("\n");
			}
			DBAusgabe(*txt);
			string speicherns;
			char speichernc;
			bool spe = true;
			while (spe) {
				cout << endl << speichern << endl;
				speicherns.clear();
				cin >> speicherns;
				if (speicherns.length() == 1) {
					speichernc = speicherns[0];
					switch (speichernc) {
					case 'j':
#ifdef _WIN32
						system("cls");
#elif __linux
						system("clear");
#endif
						DBAufHDDSchreiben(*txt, pfad, mpw, true);
						spe = false;
						break;
					case 'n':
#ifdef _WIN32
						system("cls");
#elif __linux
						system("clear");
#endif
						txt->clear();
						txt->append(tmp);
						spe = false;
						break;
					default:
		#ifdef _WIN32
						system("cls");
		#elif __linux
						system("clear");
		#endif
						cout << endl << falscheEingabe << endl << endl << endl;
						break;
					}
				}
				else {
		#ifdef _WIN32
					system("cls");
		#elif __linux
					system("clear");
		#endif
					cout << endl << falscheEingabe << endl << endl << endl;
				}
			}
				}
		}
		else {
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			cout << endl << falscheEingabe << endl << endl << endl;
		}
	}

	return *txt;
}

/*	Diese Funktion wird aufgerufen, wenn man einen Eintrag hinzufügen will.
*/
string KeyManager::eintragHinzufuegen(string* txt, const string pfad, const string mpw) {
	unsigned int anzEintr = 0;
	bool mehr = true;
	string textNeu;
	vector<string>txtLines2;
	txtLines2.clear();
	unsigned int iAlt = 0;
	int index = 0;
	for (unsigned int i = 0; i < txt->length(); i++) {
		if (txt[0][i] == '\n') {
			string s = txt->substr(iAlt, i);
			s.resize((i - iAlt));
			txtLines2.push_back(s);
			iAlt = i + 1;
			index++;
		}
	}
	anzEintr = DBAusgabe(*txt);
	char wahl;
	while (mehr) {
		anzEintr++;
		textNeu.clear();
		vector<string> txtLinesNeu;
		for (unsigned int i = 0; i < anzEintr - 1; i++) {
			txtLinesNeu.push_back(txtLines2[i]);
		}
		char z[4];
		if (anzEintr < 10) {
			z[0] = dbTrenner; z[1] = 0; z[2] = 0; z[3] = 0;
		}
		else if (anzEintr >= 10) {
			z[0] = dbTrenner; z[1] = 0; z[2] = 0; z[3] = 0;
		}
		txtLinesNeu.push_back(z);
		txtLinesNeu[anzEintr - 1].append(NeuerEintrag());

		txt->clear();
		for (unsigned int i = 0; i < anzEintr; i++) {
			txt->append(txtLinesNeu[i]);
			if (i < anzEintr - 1) {
				txt->append("\n");
			}
		}
#ifdef _WIN32
		system("cls");
#elif __linux
		system("clear");
#endif
		DBAusgabe(*txt);
		string wahls;
		bool mehr2 = true;
		while (mehr2) {
			cout << eintragHinzufuegenS << endl;
			wahls.clear();
			cin >> wahls;
			if (wahls.length() == 1) {
				wahl = wahls[0];
				switch (wahl) {
				case 'j':
					while (wahl == 'j') {
						anzEintr++;
						if (anzEintr < 10) {
							z[0] = dbTrenner; z[1] = 0; z[2] = 0; z[3] = 0;
						}
						else if (anzEintr >= 10) {
							z[0] = dbTrenner; z[1] = 0; z[2] = 0; z[3] = 0;
						}
						textNeu.clear();
						txt->append(z);
						txt->append(NeuerEintrag());
#ifdef _WIN32
						system("cls");
#elif __linux
						system("clear");
#endif
						DBAusgabe(*txt);

						cout << eintragHinzufuegenS << endl;
						wahls.clear();
						cin >> wahls;
						if (wahls.length() == 1) {
							wahl = wahls[0];
							if (wahl != 'n' && wahl != 'j') {
								cout << falscheEingabe << endl;
								bool eing = true;
								while (eing) {
#ifdef _WIN32
									system("cls");
#elif __linux
									system("clear");
#endif
									DBAusgabe(*txt);
									cout << eintragHinzufuegenS << endl;
									wahls.clear();
									cin >> wahls;
									if (wahls.length() == 1) {
										wahl = wahls[0];
										if (wahl == 'j' || wahl == 'n') {
											mehr2 = false;
											eing = false;
										}
									}
									else {
										cout << falscheEingabe << endl;
									}
								}
							}
						}
						else {
							cout << falscheEingabe << endl;
						}
						if (wahl == 'n') {
							DBAufHDDSchreiben(*txt, pfad, mpw, true);
							mehr2 = false;
							mehr = false;
						}
					}
					break;
				case 'n':
					txt->clear();
					for (unsigned int i = 0; i < anzEintr; i++) {
						txt->append(txtLinesNeu[i]);
						if (i < anzEintr - 1) {
							txt->append("\n");
						}
					}
					//DBAusgabe(*txt);
					DBAufHDDSchreiben(*txt, pfad, mpw, true);
					mehr2 = false;
					mehr = false;
					break;
				default:
					cout << endl << falscheEingabe << endl << endl << endl;
					cout << eintragHinzufuegenS << endl;
					break;
				}
			}
			else {

				cout << falscheEingabe << endl;
			}
		}
	}
	return *txt;
}

/*	Diese Funktion wird aufgerufen, wenn man einen Eintrag löschen will.
*/
string KeyManager::eintragLoeschen(string* txt, const string pfad, const string mpw) {
	string backup;
	backup = *txt;
	bool loeschen = true;
	unsigned int anzEintr = 0;
	unsigned int ind = 0;
	char indc;
	string inds;
	bool eintrEing = true;
	int b = 0;
	while (loeschen) {
#ifdef _WIN32
		system("cls");
#elif __linux
		system("clear");
#endif
		anzEintr = DBAusgabe(*txt);
		cout << eintragLoeschenS << " [1-" << anzEintr << "]" << endl;
		cout << zurueckZumHauptmenue << endl;
		inds.clear();
		cin >> inds;
		b = atoi(inds.c_str());
		if (b != 0) {
			ind=b;
			if (b < 10 && inds.length() > 1) {
				ind = -1;
			}
			if (b >= 10 && inds.length() > 2) {
				ind = -1;
			}
		}
		else if (inds.length() <= 1 && inds[0] == 48) {
			ind = 0;
		}
		else { 
			ind = (-1); 
		}

		if ((int)ind < 0) {
			cout << endl << falscheEingabe << endl << endl << endl;
		}
		else if (ind > anzEintr) {
			cout << endl << falscheEingabe << endl << endl << endl;
		}
		else if (ind == 0) {
			loeschen = false;
		}
		else if ((ind > 0) && (ind <= anzEintr)) {
			eintrEing = false;
			anzEintr--;
			vector<string> txtTmp;
			unsigned int iAlt = 0;
			unsigned int index = 0;
			bool hilf = true;
			for (unsigned int i = 0; i < txt->length(); i++) {
				if (txt[0][i] == '\n') {
					string s = txt->substr(iAlt, i);
					s.resize((i - iAlt));
					if (index == ind - 1 && hilf == true) {
						hilf = false;
					}
					else {
						if (hilf == true) {
							txtTmp.push_back(s);
							index++;
						}
						else {
							txtTmp.push_back(s);
							index++;
						}
					}
					iAlt = i + 1;
				}
			}

			txt->clear();
			for (unsigned int i = 0; i < anzEintr; i++) {
				txt->append(txtTmp[i]);
				txt->append("\n");
			}
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			DBAusgabe(*txt);
			bool weiter = true;
			while (weiter) {
				cout << weitereLoeschen << endl;
				char l;
				string ls;
				ls.clear();
				cin >> ls;
				if (ls.length() == 1) {
					l = ls[0];
					switch (l) {
					case 'j':
#ifdef _WIN32
						system("cls");
#elif __linux
						system("clear");
#endif
						weiter = false;
						break;
					case 'n':
						weiter = false;
						loeschen = false;
						break;
					default:
#ifdef _WIN32
						system("cls");
#elif __linux
						system("clear");
#endif
						cout << falscheEingabe << endl;
						DBAusgabe(*txt);
					}
				}
				else {
#ifdef _WIN32
					system("cls");
#elif __linux
					system("clear");
#endif
					cout << falscheEingabe << endl;
				}
			}
		}
	}
	bool save = true;

	while (save) {
		cout << speichern << endl;
		char s;
		string ss;
		ss.clear();
		cin >> ss;
		if (ss.length() == 1) {
			s = ss[0];
			switch (s) {
			case 'j':
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				DBAufHDDSchreiben(*txt, pfad, mpw, true);
				save = false;
				break;
			case 'n':
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				cout << aenderungenVerworfen;
				*txt = backup;
				save = false;
				break;
			default:
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				cout << falscheEingabe;
			}
		}
		else {
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			cout << falscheEingabe;
		}
	}

	//DBAusgabe(*txt);
	loeschen = true;
	save = true;
	return *txt;
}




/*	Diese Funktion kopiert den Benutzernamen oder das Passwort eines Eintrags in die Zwischenablage (Clipboard).
*	Sie funktioniert nur unter Windows.
*/
void KeyManager::inZwischenablage(string* txt) {
	unsigned int anzEintr = 0;
	string txtLinestmp;
#ifdef _WIN32
	system("cls");
#elif __linux
	system("clear");
#endif
	anzEintr = DBAusgabe(*txt);
	unsigned int index = 0;
	unsigned int iAlt = 0;
	txtLinestmp.clear();
	unsigned int begini = 0;
	unsigned int ende = 0;
	char eing;
	string eings;
	int eingi;
	string tmp;
	int counter = 0;
	bool wa = true;
	int b = 0;
#ifdef _WIN32
	system("cls");
#elif __linux
	system("clear");
#endif
	while (wa) {

		DBAusgabe(*txt);
		cout << eintragKopieren << " [1-" << anzEintr << "]" << endl;
		cout << abbrechenNull << endl;
		eings.clear();
		cin >> eings;
		b = atoi(eings.c_str());
		if (b != 0) {
			eingi = b;
			if (b < 10 && eings.length() > 1) {
				eingi = -1;
			}
			if (b >= 10 && eings.length() > 2) {
				eingi = -1;
			}
		}
		else {
			eingi = -1;
		}
		if (eings[0] == 48 && eings.length() <= 1) {
			eingi = 0;
		}
		else if(eings[0] == 48 && eings.length() > 1){
			eingi = -1;
		}
		
	
		index = 0;
		iAlt = 0;
		if (eingi < 0) {
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			cout << falscheEingabe << endl;
		}if (eingi == 0) {
			wa = false;
		}
		else if (eingi > (int)anzEintr) {
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			cout << falscheEingabe << endl;
		}
		else if ((eingi > 0) && (eingi <= (int)anzEintr)) {
			for (unsigned int i = 0; i < txt->length(); i++) {
				if (txt[0][i] == '\n') {
					string s = txt->substr(iAlt, i - iAlt);
					//s.resize(i-iAlt);
					txtLinestmp = s;
					iAlt = i + 1;
					index++;
					if ((int)index == eingi) {
						i = txt->length();
					}
				}
			}
			bool w2 = true;
			char was;
			string wass;
			while (w2) {
				cout << nameInZwisch << endl;
				cout << pwInZwisch << endl;
				cout << zurueck << endl;
				wass.clear();
				cin >> wass;
				if (wass.length() == 1) {
					was = wass[0];
				}
				else {
					was = (-1);
				}
				switch (was) {
				case '1':
					begini = 0;
					ende = 0;
					tmp.clear();
					counter = 0;
					for (unsigned int v = 0; v < txtLinestmp.length(); v++) {
						if (txtLinestmp[v] == dbTrenner && counter == 0) {
							counter++;
							v++;
						}
						if (txtLinestmp[v] == dbTrenner && counter == 1) {
							begini = ++v;
							counter++;
						}
						if (txtLinestmp[v] == dbTrenner && counter == 2) {
							ende = v;
							v = txtLinestmp.length();
						}
					}
					tmp = txtLinestmp.substr(begini, ende - begini);
					cout << tmp << endl;
					insClipboard(tmp);
					cout << endl << kopierName << endl;
					break;
				case '2':
					begini = 0;
					ende = 0;
					tmp.clear();
					counter = 0;
					for (unsigned int v = 0; v < txtLinestmp.length(); v++) {
						if (txtLinestmp[v] == dbTrenner && counter == 0) {
							counter++;
							v++;
						}
						if (txtLinestmp[v] == dbTrenner && counter == 1) {
							counter++;
							v++;
						}
						if (txtLinestmp[v] == dbTrenner && counter == 2) {
							begini = ++v;
							counter++;
							ende = txtLinestmp.length();
							v = txtLinestmp.length();
						}
					}
					tmp = txtLinestmp.substr(begini, ende - begini);
					cout << tmp << endl;
					insClipboard(tmp);
					cout << endl << kopierPW << endl;
					break;
				case '3':
					w2 = false;
					break;
				default:
					cout << falscheEingabe << endl;
					break;
				}
			}
		}

	}
}



/*	Diese Funktion händelt das Öffnen einer Datei.
*/
void KeyManager::openDB(string pfado, string pwo) {
#ifdef _WIN32
	system("cls");
#elif __linux
	system("clear");
#endif
	string pfad;
	ifstream datei;
	bool abbrechen = false;
	if (pfado.length() ==0) {
		bool einlesen = true;
		while (einlesen) {
			abbrechen = dateiEinlesen(pfad);
			if (abbrechen) {
				return;
			}
			datei.open(pfad,ios::binary);
			if (!datei.is_open()) {
				if (abbrechen == false) {

					cout << fehlerBeimOeffnen << endl;
				}
			}
			else {
				einlesen = false;
			}
		}
	}
	else {
		pfad = pfado;
		datei.open(pfad, ios::binary);
		if (!datei.is_open()) {
			if (abbrechen == false) {
				cout << fehlerBeimOeffnen << endl;
			}
			return;
		}
	}

	string mpw;
#ifdef _WIN32
	system("cls");
#elif __linux
	system("clear");
#endif
	mpw = passwortTest(pfad,pwo);
	AES aes(mpw);
	wort w[4];
	char cou[6];
	wort out[4];
	char buf[16];


	string txt;
	string line;

	if (initVector == 0) {
		datei.seekg(32, ios::beg);
	}
	else 	if (initVector == 1 || initVector == 2) {
		datei.seekg(48, ios::beg);
	}


	cou[4] = 0;
	cou[5] = 0;
	bool ende = false;
	if (Cmode == 0) {
		while (!datei.eof()) {
			datei.read(buf, 16);
			if (!datei.eof()) {
				w[0].char0 = buf[0];	w[0].char1 = buf[1];	w[0].char2 = buf[2];	w[0].char3 = buf[3];
				w[1].char0 = buf[4];	w[1].char1 = buf[5];	w[1].char2 = buf[6];	w[1].char3 = buf[7];
				w[2].char0 = buf[8];	w[2].char1 = buf[9];	w[2].char2 = buf[10];	w[2].char3 = buf[11];
				w[3].char0 = buf[12];	w[3].char1 = buf[13];	w[3].char2 = buf[14];	w[3].char3 = buf[15];

				aes.setState(w[0].dasWort, w[1].dasWort, w[2].dasWort, w[3].dasWort);
				aes.entschluesseln();


				if (!datei.eof()) {
					out[0].dasWort = aes.getS(0);
					out[1].dasWort = aes.getS(1);
					out[2].dasWort = aes.getS(2);
					out[3].dasWort = aes.getS(3);
				}

				for (int j = 0; j < 4; j++) {
					if (out[j].byte0 == 0) {
						cou[0] = out[j].byte0; ende = true;
					}
					else {
						if (ende) {
							cou[0] = 0;
						}
						else {
							cou[0] = out[j].byte0;
						}
					}
					if (out[j].byte1 == 0) {
						cou[1] = out[j].byte1;
						ende = true;
					}
					else {
						if (ende) {
							cou[1] = 0;
						}
						else {
							cou[1] = out[j].byte1;
						}
					}
					if (out[j].byte2 == 0) {
						cou[2] = out[j].byte2;
						ende = true;
					}
					else {
						if (ende) {
							cou[2] = 0;
						}
						else {
							cou[2] = out[j].byte2;
						}
					}
					if (out[j].byte3 == 0) {
						cou[3] = out[j].byte3;
						ende = true;
					}
					else {
						if (ende) {
							cou[3] = 0;
						}
						else {
							cou[3] = out[j].byte3;
						}
					}
					txt.append(cou);
				}
			}
		}
	}
	else if (Cmode == 1) {

		wort iv[4];
		for (unsigned int i = 0; i < 4; i++) {
			iv[i].dasWort = 0;
		}
		vector<vector<wort>> tmp;
		if (initVector == 0) {  //NULL-Vector passwort korrekt Signatur ist im 2ten 16-Byte Block
			datei.seekg(16, ios::beg);
		}
		else if (initVector == 1 || initVector == 2) { //Mit IV ist passwort korrekt Signatur im 3ten 16-Byte Block
			datei.seekg(32, ios::beg);
		}

		while (!datei.eof()) {
			datei.read(buf, 16);
			if (!datei.eof()) {
				vector<wort> t;
				for (int v = 0; v < 4; v++) {
					wort w;
					t.push_back(w);
				}

				t[0].char0 = buf[0];	t[0].char1 = buf[1];	t[0].char2 = buf[2];	t[0].char3 = buf[3];
				t[1].char0 = buf[4];	t[1].char1 = buf[5];	t[1].char2 = buf[6];	t[1].char3 = buf[7];
				t[2].char0 = buf[8];	t[2].char1 = buf[9];	t[2].char2 = buf[10];	t[2].char3 = buf[11];
				t[3].char0 = buf[12];	t[3].char1 = buf[13];	t[3].char2 = buf[14];	t[3].char3 = buf[15];

				tmp.push_back(t);
			}

		}

		for (int i = 1; i < tmp.size(); i++) {

			aes.setState(tmp[i][0].dasWort, tmp[i][1].dasWort, tmp[i][2].dasWort, tmp[i][3].dasWort);
			aes.entschluesseln();

			out[0].dasWort = aes.getS(0) ^ tmp[i - 1][0].dasWort;
			out[1].dasWort = aes.getS(1) ^ tmp[i - 1][1].dasWort;
			out[2].dasWort = aes.getS(2) ^ tmp[i - 1][2].dasWort;
			out[3].dasWort = aes.getS(3) ^ tmp[i - 1][3].dasWort;
			if(i==tmp.size()-1){		//wenn padding, dann im letzten block. alle anderen Blöcke sind ohne padding
				for (int j = 0; j < 4; j++) {
					if (out[j].byte0 == 0) {
						cou[0] = out[j].byte0;
						i = tmp.size();
						ende = true;
					}
					else {
						if (ende) {
							cou[0] = 0;
						}
						else {
							cou[0] = out[j].byte0;
						}

					}
					if (out[j].byte1 == 0) {
						cou[1] = out[j].byte1;
						i = tmp.size();
						ende = true;
					}
					else {
						if (ende) {
							cou[1] = 0;
						}
						else {
							cou[1] = out[j].byte1;
						}

					}
					if (out[j].byte2 == 0) {
						cou[2] = out[j].byte2;
						i = tmp.size();
						ende = true;
					}
					else {
						if (ende) {
							cou[2] = 0;
						}
						else {
							cou[2] = out[j].byte2;
						}

					}
					if (out[j].byte3 == 0) {
						cou[3] = out[j].byte3;
						i = tmp.size();
						ende = true;
					}
					else {
						if (ende) {
							cou[3] = 0;
						}
						else {
							cou[3] = out[j].byte3;
						}
					}
					txt.append(cou);
				}
			}
			else {
				for (int j = 0; j < 4; j++) {
					cou[0] = out[j].byte0;
					cou[1] = out[j].byte1;
					cou[2] = out[j].byte2;
					cou[3] = out[j].byte3;
					txt.append(cou);
				}
				
				
			}
		}
	}
	DBAusgabe(txt);
	vector<string> txtLines2;
	unsigned int iAlt = 0;
	unsigned int index = 0;

	for (unsigned int i = 0; i < txt.length(); i++) {
		if (txt[i] == '\n') {
			string s = txt.substr(iAlt, i);
			s.resize((i - iAlt));
			txtLines2.push_back(s);
			iAlt = i + 1;
			index++;
		}
	}

	string textNeu;
	bool aendern = true;

	string backup;
	string tmp;

	string txtLinestmp; //ab hier wird das Menü ausgegeben.
#ifdef _WIN32
	system("cls");
#elif __linux
	system("clear");
#endif
	while (aendern) {

		DBAusgabe(txt);
		char eing;
		string eings;
		ofstream sout;
		cout << endl << endl <<
			menueWasTun << endl;
		cout << menue1 << endl;
		cout << menue2 << endl;
		cout << menue3 << endl;
		cout << menue4 << endl;
		cout << menue5 << endl;
		cout << menue6 << endl;
		if (klartext) {
			cout << "7.) Alle Passwörter verbergen" << endl;
		}
		else {
			cout << "7.) Alle Passwörter als Klartext anzeigen" << endl;
		}

		eings.clear();
		cin >> eings;
		if (eings.length() == 1) {
			eing = eings[0];
		}
		else {
			eing = (-1);
		}
		switch (eing) {
		case '1':
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			eintragAendern(&txt, pfad, mpw);
			break;
		case '2':
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			eintragHinzufuegen(&txt, pfad, mpw);
			break;
		case '3':
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			eintragLoeschen(&txt, pfad, mpw);
			break;
		case '4':
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			//DBAusgabe(txt);
			eintragAnzeigen(&txt);
			break;
		case '5':
			inZwischenablage(&txt);
			break;
		case '6':
			aendern = false;
			return;
		case '7':
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			if (klartext == true) {
				klartext = false;
			}
			else {
				klartext = true;
			}
			break;
		default:
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif
			cout << endl << falscheEingabe << endl << endl << endl;
			break;
		}
	}
}

/*	Diese Funktion händelt das Erstellen einer Datei
*/
void KeyManager::createDB() {
	int menupunkt = 1;
	bool eing = true;
	char eingc = '0';
	string eings;
	while (menupunkt <= 3) {
		/*while (eing) {
			cout << "Welchen Verschlüsselungsalgorithmus möchten Sie verwenden?" << endl;
			cout << "1) AES 256 (empfohlen) - Blockchiffre mit 256 Bit Blockgröße und 256 Bit Schlüsselgröße" << endl;
			eings.clear();
			cin >> eings;
			if (eings.length() == 1) {
				eingc = eings[0];
			}
			else {
				eingc = (-1);
			}
			algo = eingc - 48;
			if (algo == 1) {
				eing = false;
			}
			else {
				cout << falscheEingabe << endl;
			}
		}*/
		string hilfe;
		char hilfec;
		eing = true;
		if (menupunkt == 1) {
			while (eing) {
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif

				cout << neueDB << endl << endl << endl;
				cout << trenner << endl << endl;
				cout << aesAlgo << endl << endl << endl;
				cout << welcheModus << endl;
				cout << vernuefpen << endl << endl;
				cout << trenner << endl << endl;

				cout << ebcs << endl;
				cout << cbcs << endl;
				cout << abbrechenDrei << endl;
				cout << hilfeF << endl;
				//cout << "3) PCBC - Propagating Cypher Block Chaining (Verknüpft die Blöcke und macht daraus eine Stromchiffre)" << endl;
				//cout << "4) CFB - Cypher Feedback (Cypher des Vorgängers wird mit dem Plaintext des Nachvollgers nach der Verschlüsselung verXOdert - nach XOR für nächsten Block)" << endl;
				//cout << "5) OFB - Output Feedback (Cypher des Vorgängers wird mit dem Plaintext des Nachvollgers nach der Verschlüsselung verXOdert - vor XOR für nächsten Block)" << endl;
				//cout << "6) CTR - Counter (Jeder Block einzeln. Eine Zufallszahl wird Verschlüsselt und mit dem Plaintext verXOdert)" << endl;
				eings.clear();
				cin >> eings;
				if (eings.length() == 1) {
					eingc = eings[0];
					if (eingc != 63) {
						menupunkt++;
					}
					else {
#ifdef _WIN32
						system("cls");
#elif __linux
						system("clear");
#endif
						cout << modeHilfe1 << endl << endl;
						cout << modeHilfe2 << endl << endl;
						cout << modeHilfe3 << endl << endl;
						
#ifdef _WIN32
						system("pause");
#elif __linux
						cout << taste << endl;
						cin.ignore();
						cin.get();
#endif
					}
					if (eingc == 51) {
						return;
					}
				}
				else {
					eingc = (-1);
				}
				Cmode = eingc - 49;
				if (Cmode == 0) {
					menupunkt++;
				}
				if (Cmode >= 0 && Cmode <= 1) {
					eing = false;
				}
				else {
					cout << falscheEingabe << endl;
				}
			}
		}
		if (menupunkt == 2) {
			if (Cmode == 1) {
				eing = true;
				while (eing) {
#ifdef _WIN32
					system("cls");
#elif __linux
					system("clear");
#endif
					cout << trenner << endl << endl;
					cout << IV1 << endl;
					cout << IV2 << endl;
					cout << endl << endl << trenner << endl << endl;
					cout << IV2Menue1 << endl;
					cout << IV2Menue2 << endl;
					cout << IV2Menue3 << endl;
					cout << IV2Menue4 << endl;
					cout << IV2Menue5 << endl;
					cout << hilfeF << endl;
					eings.clear();
					cin >> eings;
					if (eings.length() == 1) {
						eingc = eings[0];
						if (eingc == 52) {
							menupunkt--;
						}
						else if (eingc == 53) {
							return;
						}
						else if (eingc == 63) {
#ifdef _WIN32
							system("cls");
#elif __linux
							system("clear");
#endif
							cout << IVHilfe1 << endl << endl;
							cout << IVHilfe2 << endl << endl;
							cout << IVHilfe3 << endl << endl;
							cout << IVHilfe4 << endl << endl;
#ifdef _WIN32
							system("pause");
#elif __linux
							cout << taste << endl;
							cin.ignore();
							cin.get();
#endif
						}
						else {
							menupunkt++;
						}
					}
					else {
						eingc = (-1);
					}
					initVector = eingc - 49;
					if (initVector >= 0 && initVector <= 4) {
						eing = false;
					}
					else {
						cout << falscheEingabe << endl;
					}
				}
			}
		}
		if (menupunkt == 3) {
			eing = true;
			while (eing) {
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				cout << trenner << endl << endl;
				cout << padding1 << endl;
				cout << endl << endl << trenner << endl << endl;
				cout << padding2 << endl;
				cout << padding3 << endl;
				cout << padding4 << endl;
				//cout << "4) ISO 10126 (empfohlen) - Random letztes Byte zeigt länge - BE EF 32 04" << endl;
				//cout << "5) ISO/IEC 7816-4 - Blocksize am anfang Rest mit Null - 16 00 00 00" << endl;
				cout << padding5 << endl;
				cout << padding6 << endl;
				cout << hilfeF << endl;
				eings.clear();
				cin >> eings;
				if (eings.length() == 1) {
					eingc = eings[0];
					
					if (eingc == 52) {
						if (Cmode == 0) {
							menupunkt--;
							menupunkt--;
						}
						else {
							menupunkt--;
						}
						
					}
					else if (eingc == 53) {
						return;
					}
					else if (eingc == 63) {
#ifdef _WIN32
						system("cls");
#elif __linux
						system("clear");
#endif
						cout << paddingHilfe1 << endl;
						cout << paddingHilfe2 << endl;
						cout << paddingHilfe3 << endl << endl;
						cout << paddingHilfe4 << endl << endl;
						cout << paddingHilfe5 << endl << endl;
						cout << paddingHilfe6 << endl << endl;
#ifdef _WIN32
						system("pause");
#elif __linux
						cout << taste << endl;
						cin.ignore();
						cin.get();
#endif
					}
					else {
						menupunkt++;
					}
				}
				else {
					eingc = (-1);
				}
				Pmode = eingc - 49;
				if (Pmode >= 0 && Pmode <= 4) {
					eing = false;
				}
				else {
					cout << falscheEingabe << endl;
				}
			}
		}
	}
	string dateiname;
	bool ort = true;
	while (ort) {
		cout << dateieingabe << endl;
		
		cin >> dateiname;

		if (dateiname == "1") {
			dateiname = "c://pa//test";
			
		}
		else if (dateiname == "0") {
			return;
		}
		ifstream ftest(dateiname);
		if (ftest) {
			cout << dateiBereitsVorhanden << endl;
			ftest.close();
			cout << dateiUeberschreiben << endl;
			string ueberschreiben;
			char ueberschreibenc;
			cin >> ueberschreiben;
			if (ueberschreiben.length() == 1) {
				ueberschreibenc = ueberschreiben[0];
				switch (ueberschreibenc) {
				case 'j':
					ort = false;
					break;
				case 'n':

					break;
				default:
					cout << endl << falscheEingabe << endl << endl << endl;
					break;
				}
			}
		}
		else {
			ort = false;
		}
	}
	string txt;
	bool pwloop = true;
	string pw;

	while (pwloop) {
		cout << masterPasswort << endl;
		pw=pwEingabe();
		if (pw.length() < 3) {
			cout << pwKurz << endl;
		}
		else if (pw.length() > 16) {
			cout << pwLang << endl;
		}
		else {
			pwloop = false;
		}
	}
#ifdef _WIN32
	system("cls");
#elif __linux
	system("clear");
#endif
	cout << neueDB << endl << endl;
	cout << ersterEintrag1 << endl;
	cout << ersterEintrag2 << endl << endl;
	string text;
	//txt.append("1");
	txt.append(1,dbTrenner);
	txt.append(NeuerEintrag());

	int anzEintr = 1;
	bool hinzufuegen = true;
	string tmp;

	while (hinzufuegen) {
		anzEintr++;

		DBAusgabe(txt);

		char eing;
		string eings;
		cout << eintragHinzufuegenS << endl;
		cout << "0.) Passwörter als Klartext anzeigen" << endl;
		eings.clear();
		cin >> eings;
		if (eings.length() == 1){
			eing = eings[0];
			switch (eing) {
			case 'j':
				char z[3];
				/*if (anzEintr < 10) {
					z[0] = (char)anzEintr + 48; z[1] = 0; z[2] = 0;
				}
				else if (anzEintr >= 10) {
					z[0] = (char)(anzEintr / 10) + 48; z[1] = (char)(anzEintr % 10) + 48; z[2] = 0;
				}

				txt.append(z);*/
				txt.append(1,dbTrenner);
				txt.append(NeuerEintrag());
				break;
			case 'n':
				hinzufuegen = false;
				break;
			case '0':
				if (klartext) {
					klartext = false;
				}
				else {
					klartext = true;
				}
			default:
				cout << endl << falscheEingabe << endl << endl << endl;
				break;
			}
		}
		else {
			anzEintr--;
			cout << endl << falscheEingabe << endl << endl << endl;
		}
	}
	DBAufHDDSchreiben(txt, dateiname, pw, false);
	openDB(dateiname,pw);
}

/*	Diese Funktion wurde zu testzwecken implementiert und ist daher auskommentiert
void KeyManager::bfDB() {
	bool DateiEinlesen = true;
	string pfad;
	string pw;
	ifstream datei;
	while (DateiEinlesen) {
		cout << dateieingabe << endl;
		cin >> pfad;

		if (pfad[0] == '1') {
			pfad = "E:\\Projektarbeit\\KeyManagerDB.km";
			cout << "Die Datei " << pfad << " wird eingelesen!" << endl;
			datei.open(pfad.c_str(), ios::in | ios::ate | ios::binary);
			if (datei.is_open()) {
				bool formatKorrekt = true;
				char check[16];
				string kmsign = "KeyManager";
				for (unsigned int i = 0; i < 16; i++) {
					datei >> check[i];
				}
				for (unsigned int i = 0; i < kmsign.length(); i++) {
					if (check[i] != kmsign[i]) {
						formatKorrekt = false;
					}
				}
				if (formatKorrekt) {
					cout << "Datei geöffnet! :)" << endl;
					DateiEinlesen = false;
				}
				else {
					cout << falschesFormat << endl;
				}
			}
			else {
				cout << "*****Fehler beim öffnen der Datei!*****" << endl;
			}
		}
		else {
			cout << "Die Datei " << pfad << " wird eingelesen!" << endl;
			datei.open(pfad.c_str(), ios::in | ios::ate | ios::binary);
			if (!datei.is_open()) {
				cout << "*****Fehler beim öffnen der Datei!*****" << endl;
				return;
			}
			datei.seekg(0, ios::beg);
			if (datei.is_open()) {
				bool formatKorrekt = true;
				char check[16];
				string kmsign = "KeyManager";
				for (unsigned int i = 0; i < 16; i++) {
					datei >> check[i];
				}
				for (unsigned int i = 0; i < kmsign.length(); i++) {
					if (check[i] != kmsign[i]) {
						formatKorrekt = false;
					}
				}
				if (formatKorrekt) {
					cout << "Datei geöffnet! :)" << endl;
					DateiEinlesen = false;
				}
				else {
					cout << falschesFormat << endl;
					datei.close();
				}
			}
			else {
				cout << "*****Fehler beim öffnen der Datei!*****" << endl;
				datei.close();
			}
		}
	}
	string mpw;
	string kmsign = "Passwort korrekt";
	//char keys[16];
	wort keys[4];

	for (unsigned int i = 0; i < 4; i++) {//48-57 , 65-90, 97-122
		keys[i].char0 = 0;
		keys[i].char1 = 0;
		keys[i].char2 = 0;
		keys[i].char3 = 0;
	}
	keys[0].char0 = '0';

	wort w[4];

	wort out[4];
	char buf[16];

	datei.seekg(16, ios::beg);
	datei.read(buf, 16);

	w[0].char0 = buf[0];	w[0].char1 = buf[1];	w[0].char2 = buf[2];	w[0].char3 = buf[3];
	w[1].char0 = buf[4];	w[1].char1 = buf[5];	w[1].char2 = buf[6];	w[1].char3 = buf[7];
	w[2].char0 = buf[8];	w[2].char1 = buf[9];	w[2].char2 = buf[10];	w[2].char3 = buf[11];
	w[3].char0 = buf[12];	w[3].char1 = buf[13];	w[3].char2 = buf[14];	w[3].char3 = buf[15];


	bool gefunden = false;
	while (!gefunden) {
		mpw.clear();
		mpw.resize(16);
		unsigned int g = 0;

		for (unsigned int j = 0; j < 4; j++) {
			mpw[g++] = keys[j].char0;
			mpw[g++] = keys[j].char1;
			mpw[g++] = keys[j].char2;
			mpw[g++] = keys[j].char3;
		}


		cout << mpw << "\t" << keys[0].dasWort << "\t" << keys[1].dasWort << "\t" << keys[2].dasWort << "\t" << keys[3].dasWort << endl;
		AES aes(mpw);


		aes.setState(w[0].dasWort, w[1].dasWort, w[2].dasWort, w[3].dasWort);
		aes.entschluesseln();

		char check[16];
		if (!datei.eof()) {
			out[0].dasWort = aes.getS(0);	check[0] = out[0].char0;	check[1] = out[0].char1;	check[2] = out[0].char2;	check[3] = out[0].char3;
			out[1].dasWort = aes.getS(1);	check[4] = out[1].char0;	check[5] = out[1].char1;	check[6] = out[1].char2;	check[7] = out[1].char3;
			out[2].dasWort = aes.getS(2);	check[8] = out[2].char0;	check[9] = out[2].char1;	check[10] = out[2].char2;	check[11] = out[2].char3;
			out[3].dasWort = aes.getS(3);	check[12] = out[3].char0;	check[13] = out[3].char1;	check[14] = out[3].char2;	check[15] = out[3].char3;
		}


		bool passwortKorrekt = true;

		for (unsigned int i = 0; i < kmsign.length(); i++) {
			if (check[i] != kmsign[i]) {
				passwortKorrekt = false;
				i = kmsign.length();
			}
		}

		if (!passwortKorrekt) {			//primitiver ansatz. Einfach hochzählen ohne auf zeichen zu achten

			keys[0].dasWort++;
			if (keys[0].dasWort == INT32_MAX) {
				keys[1].dasWort++;
				if (keys[1].dasWort == INT32_MAX) {
					keys[2].dasWort++;
					if (keys[2].dasWort == INT32_MAX) {
						keys[3].dasWort++;
						if (keys[3].dasWort == INT32_MAX) {
							cout << "Passwort nicht gefunden... :(" << endl;
							return;
						}
					}
				}
			}
		}

		if (!passwortKorrekt) {//48-57 , 65-90, 97-122
			keys[0].char0++;
			if (keys[0].char0 == 123) {
				if (keys[0].char1 == 0) {
					keys[0].char1 = 47;
				}
				keys[0].char1++;

				if (keys[0].char1 == 123) {
					if (keys[0].char2 == 0) {
						keys[0].char2 = 47;
					}
					keys[0].char2++;
					if (keys[0].char2 == 123) {
						if (keys[0].char3 == 0) {
							keys[0].char3 = 47;
						}
						keys[0].char3++;
						if (keys[0].char3 == 123) {
							if (keys[1].char0 == 0) {
								keys[1].char0 = 47;
							}
							keys[1].char0++;
							if (keys[1].char0 == 123) {
								if (keys[1].char1 == 0) {
									keys[1].char1 = 47;
								}
								keys[1].char1++;

								if (keys[1].char1 == 123) {
									if (keys[1].char2 == 0) {
										keys[1].char2 = 47;
									}
									keys[1].char2++;

									if (keys[1].char2 == 123) {
										if (keys[1].char3 == 0) {
											keys[1].char3 = 47;
										}
										keys[1].char3++;

										if (keys[1].char3 == 123) {
											if (keys[2].char0 == 0) {
												keys[2].char0 = 47;
											}
											keys[2].char0++;

											if (keys[2].char0 == 123) {
												if (keys[2].char1 == 0) {
													keys[2].char1 = 47;
												}
												keys[2].char1++;

												if (keys[2].char1 == 123) {
													if (keys[2].char2 == 0) {
														keys[2].char2 = 47;
													}
													keys[2].char2++;

													if (keys[2].char2 == 123) {
														if (keys[2].char3 == 0) {
															keys[2].char3 = 47;
														}
														keys[2].char3++;

														if (keys[2].char3 == 123) {
															if (keys[3].char0 == 0) {
																keys[3].char0 = 47;
															}
															keys[3].char0++;


															if (keys[3].char0 == 123) {
																if (keys[3].char1 == 0) {
																	keys[3].char1 = 47;
																}
																keys[3].char1++;

																if (keys[3].char1 == 123) {
																	if (keys[3].char2 == 0) {
																		keys[3].char2 = 47;
																	}
																	keys[3].char2++;


																	if (keys[3].char2 == 123) {
																		if (keys[3].char3 == 0) {
																			keys[3].char3 = 47;
																		}
																		keys[3].char3++;


																		if (keys[3].char3 == 58) {
																			keys[3].char3 = 65;
																		}
																		else if (keys[3].char3 == 91) {
																			keys[3].char3 = 97;
																		}
																		else if (keys[3].char3 == 123) {
																			cout << "Passwort nicht gefunden.... :(" << endl;
																			return;
																		}
																	}


																	if (keys[3].char2 == 58) {
																		keys[3].char2 = 65;
																	}
																	else if (keys[3].char2 == 91) {
																		keys[3].char2 = 97;
																	}
																	else if (keys[3].char2 == 123) {
																		keys[3].char2 = 48;
																	}

																}


																if (keys[3].char1 == 58) {
																	keys[3].char1 = 65;
																}
																else if (keys[3].char1 == 91) {
																	keys[3].char1 = 97;
																}
																else if (keys[3].char1 == 123) {
																	keys[3].char1 = 48;
																}
															}

															if (keys[3].char0 == 58) {
																keys[3].char0 = 65;
															}
															else if (keys[3].char0 == 91) {
																keys[3].char0 = 97;
															}
															else if (keys[3].char0 == 123) {
																keys[3].char0 = 48;
															}

														}


														if (keys[2].char3 == 58) {
															keys[2].char3 = 65;
														}
														else if (keys[2].char3 == 91) {
															keys[2].char3 = 97;
														}
														else if (keys[2].char3 == 123) {
															keys[2].char3 = 48;
														}


													}

													if (keys[2].char2 == 58) {
														keys[2].char2 = 65;
													}
													else if (keys[2].char2 == 91) {
														keys[2].char2 = 97;
													}
													else if (keys[2].char2 == 123) {
														keys[2].char2 = 48;
													}


												}

												if (keys[2].char1 == 58) {
													keys[2].char1 = 65;
												}
												else if (keys[2].char1 == 91) {
													keys[2].char1 = 97;
												}
												else if (keys[2].char1 == 123) {
													keys[2].char1 = 48;
												}


											}

											if (keys[2].char0 == 58) {
												keys[2].char0 = 65;
											}
											else if (keys[2].char0 == 91) {
												keys[2].char0 = 97;
											}
											else if (keys[2].char0 == 123) {
												keys[2].char0 = 48;
											}


										}

										if (keys[1].char3 == 58) {
											keys[1].char3 = 65;
										}
										else if (keys[1].char3 == 91) {
											keys[1].char3 = 97;
										}
										else if (keys[1].char3 == 123) {
											keys[1].char3 = 48;
										}


									}

									if (keys[1].char2 == 58) {
										keys[1].char2 = 65;
									}
									else if (keys[1].char2 == 91) {
										keys[1].char2 = 97;
									}
									else if (keys[1].char2 == 123) {
										keys[1].char2 = 48;
									}

								}

								if (keys[1].char1 == 58) {
									keys[1].char1 = 65;
								}
								else if (keys[1].char1 == 91) {
									keys[1].char1 = 97;
								}
								else if (keys[1].char1 == 123) {
									keys[1].char1 = 48;
								}

							}


							if (keys[1].char0 == 58) {
								keys[1].char0 = 65;
							}
							else if (keys[1].char0 == 91) {
								keys[1].char0 = 97;
							}
							else if (keys[1].char0 == 123) {
								keys[1].char0 = 48;
							}

						}

						if (keys[0].char3 == 58) {
							keys[0].char3 = 65;
						}
						else if (keys[0].char3 == 91) {
							keys[0].char3 = 97;
						}
						else if (keys[0].char3 == 123) {
							keys[0].char3 = 48;
						}
					}

					if (keys[0].char2 == 58) {
						keys[0].char2 = 65;
					}
					else if (keys[0].char2 == 91) {
						keys[0].char2 = 97;
					}
					else if (keys[0].char2 == 123) {
						keys[0].char2 = 48;
					}

				}

				if (keys[0].char1 == 58) {
					keys[0].char1 = 65;
				}
				else if (keys[0].char1 == 91) {
					keys[0].char1 = 97;
				}
				else if (keys[0].char1 == 123) {
					keys[0].char1 = 48;
				}

			}
			if (keys[0].char0 == 58) {
				keys[0].char0 = 65;
			}
			else if (keys[0].char0 == 91) {
				keys[0].char0 = 97;
			}
			else if (keys[0].char0 == 123) {
				keys[0].char0 = 48;
			}
		}
		else {
			gefunden = true;
			cout << "Passwort gefunden! :)" << endl;
			cout << mpw << endl;
		}
	}
}
*/

/*	Diese Funktion verschlüsselt und schreibt die Datenbank auf die Festplatte
*/
void KeyManager::DBAufHDDSchreiben(string txt, string dateiname, string pw, bool ueberschreiben) {
	wort w[4];
	wort out[4];
	ofstream sout;
	char mode;
	if (!ueberschreiben) {
		remove(dateiname.c_str());
		sout.open(dateiname.c_str(),  ios::out | ios::ate | ios::app | ios::binary);
		if (sout.is_open()) {
			sout << version;

			mode = 0;
			mode ^= (algo << 6) | (Cmode << 3) | (Pmode);
			sout << mode;
			mode = initVector;
			sout << mode;
		}
		else {
			cout << fehlerBeimSchreiben << endl;
			return;
		}
	}
	else {
		sout.open(dateiname.c_str(), ios::out | ios::binary);
		if (sout.is_open()) {
			sout.seekp(0, ios::beg);
			sout << version;
			mode = 0;
			mode ^= (algo << 6) | (Cmode << 3) | (Pmode);
			sout << mode;
			mode = initVector;
			sout << mode;

		}
		else {
			cout << fehlerBeimSchreiben << endl;
			return;
		}

		sout.close();
		sout.open(dateiname.c_str(), ios::out | ios::ate | ios::app | ios::binary);
		sout.seekp(16, ios::beg);
		if (!sout.is_open()) {
			cout << fehlerBeimSchreiben << endl;
			return;
		}
	}
	AES aes(pw);
	wort PasswortCheck1, PasswortCheck2, PasswortCheck3, PasswortCheck4;
	PasswortCheck1.char0 = 'P';
	PasswortCheck1.char1 = 'a';
	PasswortCheck1.char2 = 's';
	PasswortCheck1.char3 = 's';

	PasswortCheck2.char0 = 'w';
	PasswortCheck2.char1 = 'o';
	PasswortCheck2.char2 = 'r';
	PasswortCheck2.char3 = 't';

	PasswortCheck3.char0 = ' ';
	PasswortCheck3.char1 = 'k';
	PasswortCheck3.char2 = 'o';
	PasswortCheck3.char3 = 'r';

	PasswortCheck4.char0 = 'r';
	PasswortCheck4.char1 = 'e';
	PasswortCheck4.char2 = 'k';
	PasswortCheck4.char3 = 't';
	wort iv[4];
	for (unsigned int i = 0; i < 4; i++) {
		iv[i].dasWort = 0;
	}

	if (initVector == 0) { //CBC initialisierungsvector mit NULL 
		aes.setState(PasswortCheck1.dasWort, PasswortCheck2.dasWort, PasswortCheck3.dasWort, PasswortCheck4.dasWort);
		aes.verschluesseln();

		for (unsigned int i = 0; i < 4; i++) {
			out[i].dasWort = aes.getS(i);
			sout << out[i].char0;
			sout << out[i].char1;
			sout << out[i].char2;
			sout << out[i].char3;
		}
	}
	else if (initVector == 1) { //CBC initialisierungsvector random
		srand(time(0));
		wort r[4];
		for (unsigned int i = 0; i < 4; i++) {
			iv[i].byte0 = rand() % 256;
			iv[i].byte1 = rand() % 256;
			iv[i].byte2 = rand() % 256;
			iv[i].byte3 = rand() % 256;
			r[i].byte0 = rand() % 256;
			r[i].byte1 = rand() % 256;
			r[i].byte2 = rand() % 256;
			r[i].byte3 = rand() % 256;
		}


		aes.setState(r[0].dasWort ^ iv[0].dasWort, r[1].dasWort ^ iv[1].dasWort, r[2].dasWort ^ iv[2].dasWort, r[3].dasWort ^ iv[3].dasWort);
		aes.verschluesseln();

		for (unsigned int i = 0; i < 4; i++) {
			out[i].dasWort = aes.getS(i);
			sout << out[i].char0;
			sout << out[i].char1;
			sout << out[i].char2;
			sout << out[i].char3;
		}
		aes.setState(PasswortCheck1.dasWort ^ aes.getS(0), PasswortCheck2.dasWort^ aes.getS(1), PasswortCheck3.dasWort^ aes.getS(2), PasswortCheck4.dasWort^ aes.getS(3));
		aes.verschluesseln();

		for (unsigned int i = 0; i < 4; i++) {
			out[i].dasWort = aes.getS(i);
			sout << out[i].char0;
			sout << out[i].char1;
			sout << out[i].char2;
			sout << out[i].char3;
		}

	}
	else if (initVector == 2) { //CBC initialisierungsvector selbst eingeben

		char* ivs;
		ivs = new char[16];
		for (int g = 0; g < 16; g++) {
			ivs[g] = 0;
		}
		bool ivloop = true;
		while (ivloop) {
			cout << IVEing1 << endl;
			cout << IVEing2 << endl;
			cout << "1.) Zum automatisch generieren" << endl;

			cin >> ivs;
			int laenge = 0;
			while (ivs[laenge] != 0) {
				laenge++;
			}
			if (ivs[0] == '1'&&ivs[1] == '\0') {
				ivloop = false;
			}
			else if (laenge < 3) {
				cout << IVEing3 << endl;
			}
			else if (laenge > 16) {
				cout << IVEing4 << endl;
			}else {
				ivloop = false;
			}
		}
		srand(time(0));
		if (ivs[0] == '1'&&ivs[1] == '\0') {
			for (unsigned int i = 0; i < 4; i++) {
				iv[i].byte0 = rand() % 256;
				iv[i].byte1 = rand() % 256;
				iv[i].byte2 = rand() % 256;
				iv[i].byte3 = rand() % 256;
			}
		}
		else {
			iv[0].char0 = ivs[0]; iv[1].char0 = ivs[4]; iv[2].char0 = ivs[8]; iv[3].char0 = ivs[12];
			iv[0].char1 = ivs[1]; iv[1].char1 = ivs[5]; iv[2].char1 = ivs[9]; iv[3].char1 = ivs[13];
			iv[0].char2 = ivs[2]; iv[1].char2 = ivs[6]; iv[2].char2 = ivs[10]; iv[3].char2 = ivs[14];
			iv[0].char3 = ivs[3]; iv[1].char3 = ivs[7]; iv[2].char3 = ivs[11]; iv[3].char3 = ivs[15];
		}
		
		wort r[4];
		for (unsigned int i = 0; i < 4; i++) {
			r[i].byte0 = rand() % 256;
			r[i].byte1 = rand() % 256;
			r[i].byte2 = rand() % 256;
			r[i].byte3 = rand() % 256;
		}
		aes.setState(r[0].dasWort ^ iv[0].dasWort, r[1].dasWort ^ iv[1].dasWort, r[2].dasWort ^ iv[2].dasWort, r[3].dasWort ^ iv[3].dasWort);
		aes.verschluesseln();

		for (unsigned int i = 0; i < 4; i++) {
			out[i].dasWort = aes.getS(i);
			sout << out[i].char0;
			sout << out[i].char1;
			sout << out[i].char2;
			sout << out[i].char3;
		}
		aes.setState(PasswortCheck1.dasWort ^ aes.getS(0), PasswortCheck2.dasWort^ aes.getS(1), PasswortCheck3.dasWort^ aes.getS(2), PasswortCheck4.dasWort^ aes.getS(3));
		aes.verschluesseln();

		for (unsigned int i = 0; i < 4; i++) {
			out[i].dasWort = aes.getS(i);
			sout << out[i].char0;
			sout << out[i].char1;
			sout << out[i].char2;
			sout << out[i].char3;
		}
	}
	unsigned int x = 0;

	for (int i = 0; i < txt.length(); i += 16) {
		int txtltmp = txt.length() - 16;
		if (i >= txtltmp) {
			padding(txt, w, i);
		}
		else {
			for (int j = 0, z=0; j < 4; j++) {
				w[j].byte0 = txt[i + z++];
				w[j].byte1 = txt[i + z++];
				w[j].byte2 = txt[i + z++];
				w[j].byte3 = txt[i + z++];
			}
		}

		if (Cmode == 0) {		//ECB Mode
			aes.setState(w[0].dasWort, w[1].dasWort, w[2].dasWort, w[3].dasWort);
		}
		else if (Cmode == 1) {		//CBC Mode
			aes.setState(w[0].dasWort ^ aes.getS(0), w[1].dasWort ^ aes.getS(1), w[2].dasWort ^ aes.getS(2), w[3].dasWort ^ aes.getS(3));
		}
		aes.verschluesseln();

		for (unsigned int j = 0; j < 4; j++) {
			out[j].dasWort = aes.getS(j);
			sout << out[j].char0;
			sout << out[j].char1;
			sout << out[j].char2;
			sout << out[j].char3;
		}
	}
	sout.close();
}


/*	Diese Funktion händelt das Padding je nach Einstellung
*/
void KeyManager::padding(string& txt, wort* w, int i) {
	int x;
	x = txt.length();
	
		if (Pmode == 0) {//NULL-Padding
			if (i >= x) {
				w[0].char0 = 0;
			}
			else {
				w[0].char0 = txt[i];
			}
			if (i + 1 >= x) {
				w[0].char1 = 0;
			}
			else {
				w[0].char1 = txt[i + 1];
			}
			if (i + 2 >= x) {
				w[0].char2 = 0;
			}
			else {
				w[0].char2 = txt[i + 2];
			}
			if (i + 3 >= x) {
				w[0].char3 = 0;
			}
			else {
				w[0].char3 = txt[i + 3];
			}
			if (i + 4 >= x) {
				w[1].char0 = 0;
			}
			else {
				w[1].char0 = txt[i + 4];
			}
			if (i + 5 >= x) {
				w[1].char1 = 0;
			}
			else {
				w[1].char1 = txt[i + 5];
			}
			if (i + 6 >= x) {
				w[1].char2 = 0;
			}
			else {
				w[1].char2 = txt[i + 6];
			}
			if (i + 7 >= x) {
				w[1].char3 = 0;
			}
			else {
				w[1].char3 = txt[i + 7];
			}
			if (i + 8 >= x) {
				w[2].char0 = 0;
			}
			else {
				w[2].char0 = txt[i + 8];
			}
			if (i + 9 >= x) {
				w[2].char1 = 0;
			}
			else {
				w[2].char1 = txt[i + 9];
			}
			if (i + 10 >= x) {
				w[2].char2 = 0;
			}
			else {
				w[2].char2 = txt[i + 10];
			}
			if (i + 11 >= x) {
				w[2].char3 = 0;
			}
			else {
				w[2].char3 = txt[i + 11];
			}
			if (i + 12 >= x) {
				w[3].char0 = 0;
			}
			else {
				w[3].char0 = txt[i + 12];
			}
			if (i + 13 >= x) {
				w[3].char1 = 0;
			}
			else {
				w[3].char1 = txt[i + 13];
			}
			if (i + 14 >= x) {
				w[3].char2 = 0;
			}
			else {
				w[3].char2 = txt[i + 14];
			}
			if (i + 15 >= x) {
				w[3].char3 = 0;
			}
			else {
				w[3].char3 = txt[i + 15];
			}
		}
		if (Pmode == 1) { //ansi x.923 Padding 00 00 00 04
			int counter = 0;
			if (i >= x) {
				w[0].char0 = 0; counter++;
			}
			else {
				w[0].char0 = txt[i];
			}
			if (i + 1 >= x) {
				w[0].char1 = 0; counter++;
			}
			else {
				w[0].char1 = txt[i + 1];
			}
			if (i + 2 >= x) {
				w[0].char2 = 0; counter++;
			}
			else {
				w[0].char2 = txt[i + 2];
			}
			if (i + 3 >= x) {
				w[0].char3 = 0; counter++;
			}
			else {
				w[0].char3 = txt[i + 3];
			}
			if (i + 4 >= x) {
				w[1].char0 = 0; counter++;
			}
			else {
				w[1].char0 = txt[i + 4];
			}
			if (i + 5 >= x) {
				w[1].char1 = 0; counter++;
			}
			else {
				w[1].char1 = txt[i + 5];
			}
			if (i + 6 >= x) {
				w[1].char2 = 0; counter++;
			}
			else {
				w[1].char2 = txt[i + 6];
			}
			if (i + 7 >= x) {
				w[1].char3 = 0; counter++;
			}
			else {
				w[1].char3 = txt[i + 7];
			}
			if (i + 8 >= x) {
				w[2].char0 = 0; counter++;
			}
			else {
				w[2].char0 = txt[i + 8];
			}
			if (i + 9 >= x) {
				w[2].char1 = 0; counter++;
			}
			else {
				w[2].char1 = txt[i + 9];
			}
			if (i + 10 >= x) {
				w[2].char2 = 0; counter++;
			}
			else {
				w[2].char2 = txt[i + 10];
			}
			if (i + 11 >= x) {
				w[2].char3 = 0; counter++;
			}
			else {
				w[2].char3 = txt[i + 11];
			}
			if (i + 12 >= x) {
				w[3].char0 = 0; counter++;
			}
			else {
				w[3].char0 = txt[i + 12];
			}
			if (i + 13 >= x) {
				w[3].char1 = 0; counter++;
			}
			else {
				w[3].char1 = txt[i + 13];
			}
			if (i + 14 >= x) {
				w[3].char2 = 0; counter++;
			}
			else {
				w[3].char2 = txt[i + 14];
			}
			if (i + 15 >= x) {
				w[3].char3 = counter;
			}
			else {
				w[3].char3 = txt[i + 15];
			}
		}
		if (Pmode == 2) { //PKCS7 Padding 04 04 04 04 
			int counter = 0;
			int tmp = i;
			for (int j = 0; j < 16; j++, tmp++) {
				if (tmp > x) {
					counter++;
				}
			}

			if (i >= x) {
				if (counter == 15) {
					w[0].char0 = 0;
				}
				else {
					w[0].char0 = counter;
				}
			}
			else {
				w[0].char0 = txt[i];
			}
			if (i + 1 >= x) {
				if (counter == 14) {
					w[0].char1 = 0;
				}
				else {
					w[0].char1 = counter;
				}
			}
			else {
				w[0].char1 = txt[i + 1];
			}
			if (i + 2 >= x) {
				if (counter == 13) {
					w[0].char2 = 0;
				}
				else {
					w[0].char2 = counter;
				}
			}
			else {
				w[0].char2 = txt[i + 2];
			}
			if (i + 3 >= x) {
				if (counter == 12) {
					w[0].char3 = 0;
				}
				else {
					w[0].char3 = counter;
				}
			}
			else {
				w[0].char3 = txt[i + 3];
			}
			if (i + 4 >= x) {
				if (counter == 11) {
					w[1].char0 = 0;
				}
				else {
					w[1].char0 = counter;
				}
			}
			else {
				w[1].char0 = txt[i + 4];
			}
			if (i + 5 >= x) {
				if (counter == 10) {
					w[1].char1 = 0;
				}
				else {
					w[1].char1 = counter;
				}
			}
			else {
				w[1].char1 = txt[i + 5];
			}
			if (i + 6 >= x) {
				if (counter == 9) {
					w[1].char2 = 0;
				}
				else {
					w[1].char2 = counter;
				}
			}
			else {
				w[1].char2 = txt[i + 6];
			}
			if (i + 7 >= x) {
				if (counter == 8) {
					w[1].char3 = 0;
				}
				else {
					w[1].char3 = counter;
				}
			}
			else {
				w[1].char3 = txt[i + 7];
			}
			if (i + 8 >= x) {
				if (counter == 7) {
					w[2].char0 = 0;
				}
				else {
					w[2].char0 = counter;
				}
			}
			else {
				w[2].char0 = txt[i + 8];
			}
			if (i + 9 >= x) {
				if (counter == 6) {
					w[2].char1 = 0;
				}
				else {
					w[2].char1 = counter;
				}
			}
			else {
				w[2].char1 = txt[i + 9];
			}
			if (i + 10 >= x) {
				if (counter == 5) {
					w[2].char2 = 0;
				}
				else {
					w[2].char2 = counter;
				}
			}
			else {
				w[2].char2 = txt[i + 10];
			}
			if (i + 11 >= x) {
				if (counter == 4) {
					w[2].char3 = 0;
				}
				else {
					w[2].char3 = counter;
				}
			}
			else {
				w[2].char3 = txt[i + 11];
			}
			if (i + 12 >= x) {
				if (counter == 3) {
					w[3].char0 = 0;
				}
				else {
					w[3].char0 = counter;
				}
			}
			else {
				w[3].char0 = txt[i + 12];
			}
			if (i + 13 >= x) {
				if (counter == 2) {
					w[3].char1 = 0;
				}
				else {
					w[3].char1 = counter;
				}
			}
			else {
				w[3].char1 = txt[i + 13];
			}
			if (i + 14 >= x) {
				if (counter == 1) {
					w[3].char2 = 0;
				}
				else {
					w[3].char2 = counter;
				}
			}
			else {
				w[3].char2 = txt[i + 14];
			}
			if (i + 15 >= x) {
				if (counter == 0) {
					w[3].char3 = 0;
				}
				else {
					w[3].char3 = counter;
				}
			}
			else {
				w[3].char3 = txt[i + 15];
			}
		}
	
}



/*	Diese Funktion gibt alle Einträge in der Datenbank aus.
*/
unsigned int KeyManager::DBAusgabe(string txt) {
	unsigned int anzEintr = 0; 
	cout << endl << endl << trenner << endl;
	cout << DBAusg1 << endl;
	cout << DBAusg2 << endl;
	cout << DBAusg3 << endl;
	int counter = 0;
	int spalte = 0;
	vector<string> tmp;
	int iAlt = 0;
	for (unsigned int i = 0; i < txt.length(); i++) {
		if (txt[i] == '\n') {
			tmp.push_back(txt.substr(iAlt, i-iAlt));
			iAlt = i;
		}
	}
	int i1 = 0;
	int i2 = 0;
	int i3 = 0;

	for (int i = 0; i < tmp.size(); i++) {
		cout << i+1;
		cout << '\t';
		for (int j = 0; j < tmp[i].length(); j++) {
			if (tmp[i][j] == 3) {
				i1 = ++j;
				j = tmp[i].length();
			}
		}
		for (int j = i1; j < tmp[i].length(); j++) {
			if (tmp[i][j] == 3) {
				i2 = ++j;
				j = tmp[i].length();
			}
		}
		for (int j = i2; j < tmp[i].length(); j++) {
			if (tmp[i][j] == 3) {
				i3 = ++j;
				j = tmp[i].length();
			}
		}
		for (int j = i1; j < i2-1; j++) {
			cout << tmp[i][j];
		}
		if (i2 - i1 > 8&& i2 - i1 <= 16) {
			cout << '\t' << '\t';
		}
		else if (i2 - i1 <= 8) {
			cout << '\t' << '\t' << '\t';
		}
		else {
			cout << '\t';
		}
		
		for (int j = i2; j < i3-1; j++) {
			cout << tmp[i][j];
		}
		if (i3 - i2 > 8 && i3 - i2 <= 16) {
			cout << '\t' << '\t';
		}
		else if (i3 - i2 <= 8) {
			cout << '\t' << '\t' << '\t';
		}
		else {
			cout << '\t';
		}

		for (int j = i3; j < tmp[i].length(); j++) {
			if (klartext) {
				cout << tmp[i][j];
			}
			else {
				cout << '*';
			}
		}
		cout << endl;
	}

	cout << endl << endl << trenner << endl;
	for (unsigned int i = 0; i < txt.length(); i++) {
		if (txt[i] == '\n') {
			anzEintr++;
		}
	}
	if (anzEintr == 1) {
		cout << DBAusg4 << anzEintr << DBAusg5 << endl;
	}
	else {
		cout << DBAusg6 << anzEintr << DBAusg7 << endl;
	}
	cout << trenner << endl << endl << endl;
	return anzEintr;
}

/*	Diese Funktion händelt das hinzufügen neuer Einträge.
*/
string KeyManager::NeuerEintrag() {
	string tmp, text, pw;

	cout << neuerEintrag1 << endl;
	cin.ignore();
	getline(cin, text);
	tmp.append(text); tmp.append(1,dbTrenner);
	cout << neuerEintrag2 << endl;
	cin >> text; tmp.append(text); tmp.append(1,dbTrenner);
	cout << neuerEintrag3 << endl;
	cin >> text;
	if (text != "1") {
		tmp.append(text); tmp.append("\n");
	}
	else {
		text.clear();

		string l;
		bool pwl = true;
		while (pwl) {
			int laenge;
			cout << neuerEintrag4 << endl;
			cin >> l;
			laenge = atoi(l.c_str());
			if (laenge != 0) {
				if (laenge < 10 && l.size() == 1) {
					text.resize(laenge);
					int tmpr;
					srand(time(0));
					for (unsigned int i = 0; i < laenge;) {
						tmpr = 1 + rand() % 122;
						if ((tmpr > 47 && tmpr < 58) || (tmpr > 64 && tmpr < 91) || (tmpr > 96 && tmpr < 123)) {
							text[i] = (char)tmpr;
							i++;
						}
					}
					pwl = false;
				}
				else if (laenge >= 10 && l.size() == 2) {
					text.resize(laenge);
					int tmpr;
					srand(time(0));
					for (unsigned int i = 0; i < laenge;) {
						tmpr = 1 + rand() % 122;
						if ((tmpr > 47 && tmpr < 58) || (tmpr > 64 && tmpr < 91) || (tmpr > 96 && tmpr < 123)) {
							text[i] = (char)tmpr;
							i++;
						}
					}
					pwl = false;
				}
				else {
					cout << falscheEingabe << endl;
				}
			}
			else {
#ifdef _WIN32
				system("cls");
#elif __linux
				system("clear");
#endif
				cout << falscheEingabe << endl;
			}
		}

		tmp.append(text); tmp.append("\n");
	}

	return tmp;
}

/*	Diese Funkton händelt das kopieren eines Strings in die Zwischenablage unter Windows
*/

void KeyManager::insClipboard(string s) {
#ifdef _WIN32
	if (!OpenClipboard(NULL)) {
		return;
	}

	EmptyClipboard();

	HGLOBAL hClipboardData;
	hClipboardData = GlobalAlloc(GMEM_DDESHARE, s.length() + 1);

	char *pchData;
	pchData = (char*)GlobalLock(hClipboardData);

	strcpy_s(pchData, s.length() + 1, s.c_str());

	GlobalUnlock(hClipboardData);

	SetClipboardData(CF_TEXT, hClipboardData);

	CloseClipboard();
	

#elif __linux
	string tmp;
	tmp.append("echo ");
	tmp.append(s);
	tmp.append(" | xclip");
	
	system(tmp.c_str);
	
#endif
}



/*	Diese Funktion ermöglicht die Eingabe eines Passworts, ohne dass es als Klartext auf dem Bildschrim angezeigt wird.
*	Stattdessen werden Sternchen ausgegeben.
*/
string KeyManager::pwEingabe() {
	string e;
#ifdef _WIN32
	bool pweing = true;
	
	char ch;
	int pwcount = 0;
	int pwcountmax = 0;
	while (pweing) {
		ch = _getch();
		if (ch == '\r') {
			pweing = false;
		}
		else if (ch == '\b'&&pwcount>0) {
			pwcount--;
			e.pop_back();
			cout << "\r";
			for (int i = 0; i < pwcount; i++) {
				cout << '*';
			}for (int i = pwcount; i < pwcountmax; i++) {
				cout << ' ';
			}
		}
		else if (ch != '\b') {
			e += ch;
			pwcount++;
			cout << "\r";
			for (int i = 0; i < pwcount; i++) {
				cout << '*';
			}
		}
		if (pwcount > pwcountmax) {
			pwcountmax = pwcount;
		}
	}
	cout << endl;
	

#elif __linux
	initscr();
	noecho();
	cbreak();
	bool pweing = true;
	int ch;
	int pwcount = 0;
	int pwcountmax = 0;
	move(0, 0);
	refresh();
	cout << endl << masterPasswort << endl;
	refresh();
	while (pweing) {
		move(0, 0);
		ch = getch();
		if (ch == 10) {
			pweing = false;
		}
		else if (ch == 127 && pwcount > 0) {
			pwcount--;
			e.pop_back();
			move(0, 0);
		}
		else if (ch != 10 && ch != 127) {
			move(0, 0);
			e += ch;
			pwcount++;
		}if (pwcount > pwcountmax) {
			pwcountmax = pwcount;
		}
		for (int i = 0; i < 2; i++) {
			clear();
			refresh();
		}
		cout << endl << masterPasswort;
		refresh();
		for (int i = 0; i < pwcount; i++) {
			cout << '*';
			refresh();
		}
		for (int i = 0; i < 1; i++) {
			refresh();
			move(0, 0);
			cout << endl;
		}
	}
	endwin();
	cout << endl;
	
#endif
	return e;
}




int main() {
#ifdef _WIN32
	system("CHCP 28591");
	//system("CHCP 850");
	system("cls");
#endif // #ifdef _WIN32

	int Cmode = 0; //0=ECB 1=CBC 2=CFB 3=OFB
	int Pmode = 0; //0=Zero-Patting 1=ANSI X.923 2=pkcs7 3=ISO 10126 4=ISO/IEC 7816-4

	string pfad;
	string pwo;
	KeyManager km(Cmode, Pmode);
	cout << km.welcome1 << endl;
	cout << km.welcome2 << endl;
	cout << km.welcome3 << endl;
	cout << km.welcome2 << endl;
	cout << km.welcome1 << endl << endl << endl << endl << endl;
	string eingabe;
	
	bool test = true;
	bool first = true;
	while (test) {
		if (!first) {
#ifdef _WIN32
			system("cls");
#elif __linux
			system("clear");
#endif // _WIN32
		}
		first = false;
		cout << km.hauptMenu1 << endl;
		cout << km.hauptMenu2 << endl;
		cout << km.hauptMenu3 << endl;
		cout << km.hauptMenu4 << endl;
		//cout << km.hauptMenu5 << endl;
		eingabe.clear();
		cin >> eingabe;
		if (eingabe.length() == 1) {
			char tmp = eingabe[0];
			switch (tmp) {
				case '1': km.createDB(); break;
				case '2': km.openDB(pfad, pwo); break;
				case '3': return 0;
				//case '4': km.bfDB(); break;
				default: cout << endl << km.falscheEingabe << endl << endl << endl; break;
			}
		}
		else {
			cout << endl << km.falscheEingabe << endl << endl << endl; 
		}

	}

	return 0;
}