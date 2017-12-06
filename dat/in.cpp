/*********************************** Parser ***********************************/

/** Umlaute ASCII UTF8 Tabelle
  * \x81 \x84 \x94 \x8E \x99 \x9A \xE1"); ü ä ö Ä Ö Ü ß
  */
  #define START "../Code-Studio2/dat/screen.bin"
  #define PFAD  "../Code-Studio2/output"
  #define CODE  "../Code-Studio2/dat/main.bin"
  #define HEAD  "../Code-Studio2/dat/incl.bin"
  #define FNKT  "../Code-Studio2/dat/ctrl.bin"
  #define USER  "../Code-Studio2/dat/user.bin"
  #define VERS  "2.0"
  #define DATUM "14.07.2012"
  #ifdef __linux__
  #define Ae "Ä"
  #define ae "ä"
  #define Ue "Ü"
  #define ue "ü"
  #define Oe "Ö"
  #define oe "ö"
  #define ss "ß"
  #define TASTE "[D]"
  #elif __WIN32__ || _MSC_VER__
  #define Ae "\x8E"
  #define ae "\x84"
  #define Ue "\x9A"
  #define ue "\x81"
  #define Oe "\x99"
  #define oe "\x94"
  #define ss "\xE1"
  #define TASTE "[Z]+[Enter]"
  #endif

/** Datenstruktur fuer Interaktionen
  */
  struct interaktion
  {
	  int  iLoop;
	  int  iLine;
	  int  iPos_Include;
	  char sAbfrage[12];
	  /* Bildschirmausgabe speichern */
	  char sDisplay[256];
  }stInteraktion;

/** Datenstruktur fuer Abfragen
  */
  struct datenbank
  {
	  char sHallo[5];
	  char sHilfe[5];
	  char sVersion[7];
	  char sFormular[2];
	  char sIncludes[8];
	  char sGenerieren[10];
	  char sBeenden[7];
	  char sTageszeit[13];
  }stDatenbank;

/** Datenstruktur fuer Benutzereingaben
  */
  struct eingabe
  {
	  int  iPos_Name;
	  int  iPos_ToDo;
	  int  iPos_Beschreibung;
	  /* 300 Zeichen Textlaenge moeglich */
      char sName[2][62];
      char sToDo[5][62];
      char sBeschreibung[3][62];
  }stEingabe;

/** Funktion : interne Datenbank fuer Abfragen
  * Status   : vorgelegt 
  */
  void iInitialisiere_Datenbank()
  {
	  time_t tUhrzeit;
	  struct tm *stUhrzeit;
	  char sUhrzeit[3];
	  int  iUhrzeit = 0;
	  /* Systemzeit auslesen */
	  time (&tUhrzeit);
	  stUhrzeit = localtime (&tUhrzeit);
	  /* einstellige Stundezahl aus stUhrzeit auswerten */
	  strftime(sUhrzeit, sizeof(sUhrzeit), "%H", stUhrzeit);
	  /* Stunde als integer umwandeln */
	  sscanf(sUhrzeit, "%i", &iUhrzeit);
	  /* Tageszeiten abfragen */
	  if(iUhrzeit >= 10 && iUhrzeit <= 17)                                              
	  {                                                                             
		  snprintf(stDatenbank.sTageszeit, 
		  sizeof(stDatenbank.sTageszeit), "Guten Tag");                                                        
	  }
	  else if(iUhrzeit >= 17 && iUhrzeit <= 23)                                              
	  {                                                                             
		  snprintf(stDatenbank.sTageszeit, 
		  sizeof(stDatenbank.sTageszeit), "Guten Abend");                                                     
	  }
	  else                                              
	  {                                                                             
		  snprintf(stDatenbank.sTageszeit, 
		  sizeof(stDatenbank.sTageszeit), "Guten Morgen");                                                    
	  }
	  /* Parameter in Datenbank Struktur speichern */
	  snprintf(stDatenbank.sHallo, 
	  sizeof(stDatenbank.sHallo), "hallo");
	  snprintf(stDatenbank.sHilfe, 
	  sizeof(stDatenbank.sHilfe), "hilfe");
	  snprintf(stDatenbank.sVersion, 
	  sizeof(stDatenbank.sVersion), "version");
	  snprintf(stDatenbank.sFormular, 
	  sizeof(stDatenbank.sFormular), "#");
	  snprintf(stDatenbank.sIncludes, 
	  sizeof(stDatenbank.sIncludes), "include");
	  snprintf(stDatenbank.sGenerieren, 
	  sizeof(stDatenbank.sGenerieren), "generieren");
	  snprintf(stDatenbank.sBeenden, 
	  sizeof(stDatenbank.sBeenden), "beenden");
  }

/** Funktion : Bildschirmausgabe loeschen
  * Status   : vorgelegt 
  */
  void iLoesche_Ausgabe(int iZeile)
  {
	  while(stInteraktion.iLoop++ < iZeile)
	  {
		  printf("\n");
	  }
  }

/** Funktion : Eingabfeld aufrufen
  * Status   : vorgelegt
  */
  int iSetze_Eingabefeld(int iZeile)
  {
	  for(stInteraktion.iLoop = 0; stInteraktion.iLoop 
	  < iZeile; stInteraktion.iLoop++)
	  {
		  printf("\n");
	  }
	  /* Eingabemaske im Formular sperren */
	  if(stInteraktion.sAbfrage[0] != '#')
	  {
		  printf("\tWas m%schten Sie machen?\n"
		  "\tMit \"Hilfe\" %sbersicht anzeigen lassen: ", oe, Ue);
	  }
	  return EXIT_SUCCESS;
  }

/** Funktion : Gibt die Menueausgabe auf dem Bildschirm aus
  * Status   : vorgelegt
  */
  int iBildschirmausgabe()
  {
	  FILE *FPLesen;
	  FPLesen = fopen(START, "rb");    
	  /* pruefe, ob Datei existiert */   
	  if(FPLesen == NULL)
	  {
		  fprintf(stderr, "\tFehler: '%s'\n\tkann nicht angezeigt werden.", 
		  START);
		  iSetze_Eingabefeld(8);
		  return EXIT_FAILURE;
	  }
	  else
	  {
		  /* lese zeilenweise aus Datei */
		  while(fread(stInteraktion.sDisplay, 1, 
		  sizeof(stInteraktion.sDisplay), FPLesen))
		  {
			  printf("%s", stInteraktion.sDisplay);
		  }
	  }
	  /* Datei aus Sicherheitsgruenden schliessen */
	  fclose(FPLesen);
	  return EXIT_SUCCESS;
  }

/** Funktion : Anfangsbuchstaben in Grossbuchstaben umwandeln
  * Status   : vorgelegt
  */
  void iKonvertiere_Buchstaben()
  {
	  /* Anfangsbuchstaben in Großbuchstaben umwandeln */
	  for(stInteraktion.iLoop = 0; stInteraktion.iLoop
	  < stEingabe.iPos_Name; stInteraktion.iLoop++)
	  {
		  stEingabe.sName[stInteraktion.iLoop][0]
		  = toupper(stEingabe.sName[stInteraktion.iLoop][0]);
	  }
  }

/** Funktion : Bildschirmausgabe pausieren
  * Status   : vorgelegt 
  */
  void iSetze_Pause(int iZeit)
  {
	  time_t tStart;
	  time_t tStop;
	  double dDiff;
	  /* Startbildschirm wartet 3 Sekunden, 
	   * soll aber vor dem Aufruf nicht vorher geloescht werden, im Gegensatz
	   * zum Beenden- und Generierenbildschirm */
	  if((strncmp(stDatenbank.sGenerieren, stInteraktion.sAbfrage, 4)) == 0
	  || (strncmp(stDatenbank.sIncludes, stInteraktion.sAbfrage, 4)) == 0
	  || (strncmp(stDatenbank.sBeenden, stInteraktion.sAbfrage, 4)) == 0)
	  {
		  iLoesche_Ausgabe(25);
	  }
	  iBildschirmausgabe();
	  if((strncmp(stDatenbank.sGenerieren, stInteraktion.sAbfrage, 4)) == 0 
	  || (strncmp(stDatenbank.sIncludes, stInteraktion.sAbfrage, 4)) == 0)
	  {
		  printf("\tEinen Augenblick bitte...\n\n");
	  }
	  else if((strncmp(stDatenbank.sBeenden, stInteraktion.sAbfrage, 4)) == 0)
	  {
		  printf("\t\t    V I E L E N  D A N K  F %s R"
		  "  D I E  N U T Z U N G\n\n",Ue);
	  }
	  else
	  {
		  printf("\t\t    N I C O  A N D E R S  (c) 2 0 1 1 - 2 0 1 2");
	  }
	  /* notwendig fuer die Position des Abschlussbildschirms */
	  for(stInteraktion.iLoop = 0; stInteraktion.iLoop < 8; 
	  stInteraktion.iLoop++)
	  {
		  printf("\n");
	  }
	  tStart = time(NULL);
	  while((dDiff = difftime(tStop = time(NULL),tStart)) != iZeit);
}

/** Funktion : Hinweis aufrufen
  * Status   : vorgelegt
  */
  int iHinweis()
  {
	  /* Bildschirm fuer neue Ausgabe leeren */
	  iLoesche_Ausgabe(25);
	  iBildschirmausgabe();
	  if(stInteraktion.sAbfrage[0] == '\0')
	  {
		  printf("\tTut mir leid, leere Eingaben kann ich nicht verarbeiten."
		  "\n");
	  }
	  else
	  {
		  printf("\tTut mir leid, den Parameter '%s' verstehe ich noch nicht."
		  "\n", stInteraktion.sAbfrage);
	  }		  
	  printf("\tVersuchen Sie es doch mal mit einem freundlichen Hallo.\n"
	  "\tBen%stigen Sie Unterst%stzung, geben Sie bitte \"Hilfe\" ein.",
	  oe, ue);
	  iSetze_Eingabefeld(7);
	  return EXIT_SUCCESS;
  }

/** Funktion : Hilfe anzeigen
  * Status   : vorgelegt
  */
  int iHilfe()
  {
	  iLoesche_Ausgabe(25);
	  iBildschirmausgabe();
	  printf("\tGeben Sie einen der folgenden Befehle in das Eingabefeld ein:"
	  "\n\n"
	  "\t- Hilfe......: %sbersicht %sffnen\n"
	  "\t- Version....: Entwicklungsstand ansehen\n"
	  "\t- [#]-Taste..: Quellcode kommentieren\n"
	  "\t- Includes...: Headerdateien hinzuf%sgen\n"
	  "\t- Generieren.: Compilate erzeugen\n"
	  "\t- Beenden....: Generator schlie%sen",Ue, oe, ue, ss);
	  iSetze_Eingabefeld(2);
	  return EXIT_SUCCESS;
  }

/** Funktion : Standard ANSI C Bibliotheken einbinden
  * Status   : vorgelegt
  */
  int iHeader()
  {
	  iSetze_Pause(1);
	  /* Bildschirm fuer neue Ausgabe leeren */ 
	  iLoesche_Ausgabe(25);
	  iBildschirmausgabe();
	  /* Zaehler zuruecksetzen und von Null beginnen */
	  stInteraktion.iPos_Include  = 0;
	  printf("\tFolgende Standard C-Bibliotheken wurden hinzugef%sgt.\n\n"
	  "\t- string.h..: Zeichenkettenoperationen\n"
	  "\t- math.h....: Mathematische Funktionen\n"
	  "\t- time.h....: Datum und Uhrzeit\n"
	  "\t- ctype.h...: Zeichenkonvertierung\n"
	  "\t- signal.h..: Signale behandeln", ue);
	  stInteraktion.iPos_Include++;
      iSetze_Eingabefeld(3);
	  return EXIT_SUCCESS;
  }

/** Funktion : Version anzeigen
  * Status   : vorgelegt
  */
  int iVersion()
  {
	  /* Bildschirm fuer neue Ausgabe leeren */
	  iLoesche_Ausgabe(25);
	  iBildschirmausgabe();
	  printf("\tHallo, ich wurde an der AIK f%sr Softwaretechnik entwickelt."
	  "\n\n"
	  "\t- Programm....: Code Studio\n"
	  "\t- Entwickler..: Nico Anders < nicoanders@freenet.de >\n"
	  "\t- Version.....: %s\n"
	  "\t- Datum.......: %s\n\n"
	  "\t< All rights reserved >", ue, VERS, DATUM);
	  iSetze_Eingabefeld(2);
	  return EXIT_SUCCESS;
  }

/** Funktion : Hauptmenue aufklappen
  * Status   : vorgelegt
  */
  int iMenue()
  {
	  /* Bildschirm fuer neue Ausgabe leeren */
	  iLoesche_Ausgabe(25);
	  iBildschirmausgabe();
	  printf("\t%s, ich bin ihr interaktiver Codegenerator %s.\n" 
	  "\tMit mir k%snnen Sie schnell und bequem Quellcodes erzeugen.\n"
	  "\tGeben Sie dazu den Befehl \"Generieren\""
	  " in das Eingabefeld ein."
      ,stDatenbank.sTageszeit, VERS, oe);
      iSetze_Eingabefeld(7);
	  return EXIT_SUCCESS;
  }

/** Funktion : Benutzerformular oeffnen
  * Status   : vorgelegt
  */
  int iFormular()
  {
	  /* Bildschirm fuer neue Ausgabe leeren */
      iLoesche_Ausgabe(25);
	  iBildschirmausgabe();
	  /* Zaehler zuruecksetzen und von Null beginnen */
	  stEingabe.iPos_Name         = 0;
	  stEingabe.iPos_ToDo         = 0;
	  stEingabe.iPos_Beschreibung = 0;
	  stInteraktion.iLine         = 2;
	  /* Autor eingeben */
	  printf("\tGeben Sie bitte ihren NAMEN (max. %i Zeilen, 60 Zeichen) ein.\n"
	  "\tDr%scken Sie zum Fortsetzen die Tastenkombination [Strg]+%s."
	  , stInteraktion.iLine, ue, TASTE);
	  iSetze_Eingabefeld(9);
	  printf("\t");
	  while(fgets(stEingabe.sName[stEingabe.iPos_Name], 
	  sizeof(stEingabe.sName[stEingabe.iPos_Name]), stdin))
	  {
		  printf("\t");
		  /* Neue Zeilen werden zu einer zusammengefasst */
		  stEingabe.sName[stEingabe.iPos_Name]
		  [strlen(stEingabe.sName[stEingabe.iPos_Name])-1] = '\0';
		  /* Benutzername in neuer Zeile erfassen */
		  stEingabe.iPos_Name++;
		  /* Eingabe beenden, wenn Zeilenende erreicht */
		  if(stEingabe.iPos_Name == stInteraktion.iLine)
		  {
			  printf("\n\tSie haben das Zeilenende erreicht."
			  " Dr%scken Sie die Eingabetaste.", ue);
			  fgetc(stdin);
			  break;
		  }
	  }
	  /* Bildschirm fuer neue Ausgabe leeren */
	  iLoesche_Ausgabe(25);
	  iBildschirmausgabe();
	  /* ToDo's eingeben */
	  printf("\tGeben Sie bitte ihre TO DO's (max. %i Zeilen, 60 Zeichen) ein.\n"
	  "\tDr%scken Sie zum Fortsetzen die Tastenkombination [Strg]+%s."
	  , stInteraktion.iLine, ue, TASTE);
	  iSetze_Eingabefeld(9);
	  printf("\t");
	  while(fgets(stEingabe.sToDo[stEingabe.iPos_ToDo], 
	  sizeof(stEingabe.sToDo[stEingabe.iPos_ToDo]), stdin))
	  {
		  printf("\t");
		  /* Neue Zeilen werden zu einer zusammengefasst */
		  stEingabe.sToDo[stEingabe.iPos_ToDo]
		  [strlen(stEingabe.sToDo[stEingabe.iPos_ToDo])-1] = '\0';
		  /* ToDo's in neuer Zeile erfassen */
		  stEingabe.iPos_ToDo++;
		  /* Eingabe beenden wenn Zeilenende erreicht */
		  if(stEingabe.iPos_ToDo == stInteraktion.iLine)
		  {
			  printf("\n\tSie haben das Zeilenende erreicht."
			  " Dr%scken Sie die Eingabetaste.", ue);
			  fgetc(stdin);
			  break;
		  }
	  }
	  /* Bildschirm fuer neue Ausgabe leeren */
	  iLoesche_Ausgabe(25); 
	  iBildschirmausgabe();
	  /* Beschreibung eingeben */
	  printf("\tGeben Sie bitte eine BESCHREIBUNG "
	  "(max. %i Zeilen, 60 Zeichen) ein.\n"
	  "\tDr%scken Sie zum Fortsetzen die Tastenkombination [Strg]+%s."
	  , stInteraktion.iLine, ue, TASTE);
	  iSetze_Eingabefeld(9);
	  printf("\t");
	  while(fgets(stEingabe.sBeschreibung[stEingabe.iPos_Beschreibung], 
	  sizeof(stEingabe.sBeschreibung[stEingabe.iPos_Beschreibung]), stdin))
	  {
		  printf("\t");
		  /* Neue Zeilen werden zu einer zusammengefasst */
		  stEingabe.sBeschreibung[stEingabe.iPos_Beschreibung]
		  [strlen(stEingabe.sBeschreibung
		  [stEingabe.iPos_Beschreibung])-1] = '\0';
		  /* Beschreibung in neuer Zeile erfassen */
		  stEingabe.iPos_Beschreibung++;
		  /* Eingabe beenden wenn Zeilenende erreicht */
		  if(stEingabe.iPos_Beschreibung == stInteraktion.iLine)
		  {
			  printf("\n\tSie haben das Zeilenende erreicht."
			  " Dr%scken Sie die Eingabetaste.", ue);
			  fgetc(stdin);
			  break;
		  }   
	  }
	  /* Eingabefeld mit 0 reaktivieren */
	  stInteraktion.sAbfrage[0] = '\0';
	  iSetze_Eingabefeld(1);
	  return EXIT_SUCCESS;
  }
