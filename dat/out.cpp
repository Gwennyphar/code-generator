/********************************* Generator **********************************/

/** Funktion : Quelldateien erzeugen
  * Status   : vorgelegt
  */
  int iGenerieren()
  {
	  time_t tDatum;
	  FILE   *FPSchreiben;
	  FILE   *FPLesen1;
	  FILE   *FPLesen2;
	  FILE   *FPLesen3;
	  char   *sDatum;
	  char   *cZeichen;
	  char   sPfad[68]  = {"../Code-Studio2/output/main_"};
	  char   sDatei[68] = {"main_"};
	  int    iZeile_Name         = 0;
	  int    iZeile_ToDo         = 0;
	  int    iZeile_Beschreibung = 0;
	  char   sQuellcode[5000];
	  char   sProgrammkopf[5000];
	  float  fStart = 0;
	  float  fEnde  = 0;
	  float  fDiff  = 0;
	  /* Zeitmessung startten */
	  fStart = clock();
	  /* Neues Verzeichnis erstellen */
	  if(stat != NULL)
	  {  
		  #ifdef __linux__
		  mkdir(PFAD, 2000);
		  #elif __WIN32__ || _MSC_VER__
		  mkdir(PFAD);
		  #endif
	  }
	  /* Datum und Zeit vergeben */
	  time(&tDatum);
	  sDatum = ctime(&tDatum);
	  /* Zeilenumbruch entfernen */
	  sDatum[strlen(sDatum)-1] = '\0';
	  /* Leerzeichen im Datum entfernen */
	  while((cZeichen = strchr(sDatum, ' '))!=NULL)
	  {
		  *cZeichen = '_';
	  }
	  /* Doppelpunkte in Uhrzeit entfernen */
	  while((cZeichen = strchr(sDatum, ':'))!=NULL)
	  {
		  *cZeichen = '-';
	  }
	  /* Ausgabe verzoegern */
	  iSetze_Pause(2);
	  strncat(sPfad, sDatum, sizeof(sPfad));
	  strncat(sDatei, sDatum, sizeof(sDatei));
	  strncat(sPfad, ".c", sizeof(sPfad));
	  strncat(sDatei, ".c", sizeof(sDatei));
	  /* Dateien lesen */
      FPLesen1 = fopen(CODE, "rb");
      FPLesen2 = fopen(HEAD, "rb");
      FPLesen3 = fopen(FNKT, "rb");
      /* Quellldatei anlegen */
      FPSchreiben = fopen(sPfad, "w");
      /* Schreib und Leserechte pruefen */
      if(FPLesen1 == NULL)
	  {
		  iBildschirmausgabe();
		  fprintf(stderr, "\tFehler: '%s'\n\tkann nicht gelesen werden.", CODE);
		  iSetze_Eingabefeld(8);
		  return EXIT_FAILURE;
	  }
	  else if(FPLesen2 == NULL)
	  {
		  iBildschirmausgabe();
		  fprintf(stderr, "\tFehler: '%s'\n\tkann nicht gelesen werden.", HEAD);
		  iSetze_Eingabefeld(8);
		  return EXIT_FAILURE;
	  }
	  else if(FPLesen3 == NULL)
	  {
		  iBildschirmausgabe();
		  fprintf(stderr, "\tFehler: '%s'\n\tkann nicht gelesen werden.", FNKT);
		  iSetze_Eingabefeld(8);
		  return EXIT_FAILURE;
	  }
	  else if(FPSchreiben == NULL)
	  {
          iBildschirmausgabe();
		  fprintf(stderr, "\tFehler: Keine Schreibrechte in\n\t'%s'.", sPfad);
		  iSetze_Eingabefeld(8);
		  return EXIT_FAILURE;
	  }
	  else
	  {
		  /* Unterstriche im Datum wieder entfernen */
		  while((cZeichen = strchr(sDatum, '_'))!=NULL)
		  {
			  *cZeichen = ' ';
		  }
		  /* Bindestriche in Uhrzeit wieder entfernen */
		  while((cZeichen = strchr(sDatum, '-'))!=NULL)
		  {
			  *cZeichen = ':';
		  }
		  snprintf(sProgrammkopf+strlen(sProgrammkopf), sizeof(sProgrammkopf), 
		  "/** Programm....: %s"
		  "\n  * Entwickler..:", sDatei);
		  /*  Name aus Eingabe auslesen */
		  if((stEingabe.sName[iZeile_Name][0] != '\0'))
		  {
			  /* Benutzername zeilenweise auslesen */
			  for(iZeile_Name = 0; iZeile_Name
			  < stEingabe.iPos_Name; iZeile_Name++)
			  {
				  /* Anfangsbuchstaben in Grossbuchstaben umwandeln */
				  iKonvertiere_Buchstaben();
				  /* Eingabe zur Verarbeitung in Variable speichern */
				  snprintf(sProgrammkopf+strlen(sProgrammkopf),
				  sizeof(sProgrammkopf), " %s", stEingabe.sName[iZeile_Name]);
			  }
		  }
		  snprintf(sProgrammkopf+strlen(sProgrammkopf), sizeof(sProgrammkopf),
		  "\n  * Version.....: 0.1"
		  "\n  * Datum.......: %s" 
		  "\n  * Status......: in Arbeit"
		  "\n  * ToDo........: ", sDatum);
		  /* ToDo's aus Array auslesen */
		  for(iZeile_ToDo = 0; iZeile_ToDo < stEingabe.iPos_ToDo; iZeile_ToDo++)
		  {
			  /* Eingabe zur Verarbeitung in Variable anheangen */
			  snprintf(sProgrammkopf+strlen(sProgrammkopf), 
			  sizeof(sProgrammkopf), "%s ", stEingabe.sToDo[iZeile_ToDo]);
		  }
		  /* Ab hier beginnt der Quellcode aus sProgrammkopf */
		  snprintf(sProgrammkopf+strlen(sProgrammkopf), sizeof(sProgrammkopf),
		  "\n  */\n\n"
		  "/** Praeprozessor\n"
		  "  */\n"
		  "  #include <stdio.h>                       /* Standard-I/O */\n"
		  "  #include <stdlib.h>                      /* Nuetzliche Funktionen"
		  " */\n");
		  /* Bibliotheken aus include.bin lesen */
		  if(stInteraktion.iPos_Include != 0)
		  {
			  while (fread(sQuellcode, 1, sizeof(sQuellcode), FPLesen2))
			  {
				  /* Header zur Verarbeitung in Variable anheangen */
				  snprintf(sProgrammkopf+strlen(sProgrammkopf), 
				  sizeof(sProgrammkopf), "%s", sQuellcode);
			  }
		  }
		  /* Umlaute aus controller.bin lesen */
		  while (fread(sQuellcode, 1, sizeof(sQuellcode), FPLesen3))
		  {
			  /* Header zur Verarbeitung in Variable anheangen */
			  snprintf(sProgrammkopf+strlen(sProgrammkopf),
			  sizeof(sProgrammkopf), "%s", sQuellcode);
		  }
		  snprintf(sProgrammkopf+strlen(sProgrammkopf), sizeof(sProgrammkopf),  
		  "\n/** Funktion....: Steuerfunktion fuer das Hauptmenue"
		  "\n  * Beschreibung: ");
		  for(iZeile_Beschreibung = 0; iZeile_Beschreibung < 
		  stEingabe.iPos_Beschreibung; iZeile_Beschreibung++)
		  {
			  /* Eingabe zur Verarbeitung in Variable anheangen */
			  snprintf(sProgrammkopf+strlen(sProgrammkopf), 
			  sizeof(sProgrammkopf), "%s ", 
			  stEingabe.sBeschreibung[iZeile_Beschreibung]);
		  }
		  /* Menuestruktur aus template.bin lesen */
		  while (fread(sQuellcode, 1, sizeof(sQuellcode), FPLesen1))
		  {
			  /* Menuestruktur zur Verarbeitung in Variable anheangen */
			  snprintf(sProgrammkopf+strlen(sProgrammkopf), 
			  sizeof(sProgrammkopf), "%s", sQuellcode);
		  }    
		  /* Gesamten Quellcode in Datei schreiben */
		  fputs(sProgrammkopf, FPSchreiben);
		  /* Dateien aus Sicherheitsgruenden schliessen */
		  fclose(FPLesen1);
		  fclose(FPLesen2);
		  fclose(FPLesen3);
		  fclose(FPSchreiben);
		  /* Zeitmessung beenden */
		  fEnde = clock();
		  /* Differenz berechnen */
		  fDiff = (fEnde - fStart) / CLOCKS_PER_SEC;
		  /* Ausgabe loeschen */
		  iLoesche_Ausgabe(25);
		  iBildschirmausgabe();
		  printf("\tVielen Dank.\n\n"
		  "\tSie finden ihre Quelldatei im Verzeichnis unter\n"
		  "\t'%s'\n\n\tBen%stigte Zeit: %.2lf [ms]", sPfad, oe, fDiff);
	  }
	  /* Eingabefeld wieder anzeigen */
	  iSetze_Eingabefeld(4);
	  /* Belegte Positionen im Speicher wieder freigeben */
	  for(iZeile_Name = 0; iZeile_Name < stEingabe.iPos_Name; iZeile_Name++)
	  {
		  stEingabe.sName[iZeile_Name][0] = '\0';
	  }
	  for(iZeile_ToDo = 0; iZeile_ToDo < stEingabe.iPos_ToDo; iZeile_ToDo++)
	  {
		  stEingabe.sToDo[iZeile_ToDo][0] = '\0';
	  }
	  for(iZeile_Beschreibung = 0; iZeile_Beschreibung < 
	  stEingabe.iPos_Beschreibung; iZeile_Beschreibung++)
	  {
		  stEingabe.sBeschreibung[iZeile_Beschreibung][0] = '\0';
	  }
	  sProgrammkopf[0] = '\0';
	  /* Alle Zaehler wieder zuruecksetzen und von Null beginnen */
	  stEingabe.iPos_Name         = 0;
	  stEingabe.iPos_ToDo         = 0;
	  stEingabe.iPos_Beschreibung = 0;
	  stInteraktion.iPos_Include  = 0;
	  return EXIT_SUCCESS;
  }
