/** Programm.: code-studio2.c
  * Autor....: Nico Anders
  * Version..: 2.1
  * Datum....: 18.11.2011
  * Stand....: 30.09.2012
  * Status...: vorgelegt
  */

/** Praeprozessor
  * Bibliothek-Einbindung fuer Ansi Ein- und Ausgaben (scanf, printf)
  */
  #include <stdio.h>  						      /* Standard-I/O */
  #include <stdlib.h> 						      /* Nuetzliche Funktionen */
  #include <string.h> 						      /* Zeichenkettenoperationen */
  #include <time.h>                               /* Datum und Uhrzeit */
  #include <ctype.h>                              /* Zeichenkonvertierung */
  #include <sys/stat.h>
  #include "./dat/in.cpp"
  #include "./dat/out.cpp"

/********************** Hauptprogramm Code-Studio *****************************/

/** Funktion....: Startbildschirm anzeigen 
  * Status......: vorgelegt
  */
  int iCodegen()
  {
	  char sEingabe[12];
	  /* Woerter aus in.h in Struct laden */
	  iInitialisiere_Datenbank();
	  iSetze_Pause(3);
	  iMenue();
	  do
	  {
		  /* abgesicherte Eingabe */
		  fgets(sEingabe, sizeof(sEingabe), stdin);
		  sscanf(sEingabe, "%s", stInteraktion.sAbfrage);
		  /* Eingabe in Kleinbuchstaben umwandeln und Strings aus 
		   * Initialisiere_Datenbank() vergleichen. */
		  for(stInteraktion.iLoop = 0; 
		  stInteraktion.sAbfrage[stInteraktion.iLoop];stInteraktion.iLoop++)
		  {
			  stInteraktion.sAbfrage[stInteraktion.iLoop] = 
			  tolower(stInteraktion.sAbfrage[stInteraktion.iLoop]);
		  }
		  /* das Menue aufklappen
		   * wiederhole die Schleife, um das Menue von
		   * allen Funktionen aus aufrufen zu koennen */
		  if((strncmp(stDatenbank.sHallo, stInteraktion.sAbfrage, 4)) == 0)
		  {
			  iMenue();
			  continue;
		  }
		  if((strncmp(stDatenbank.sHilfe, 
		  stInteraktion.sAbfrage, 4)) == 0)
		  {
			  iHilfe();
			  continue;
		  }
		  else if((strncmp(stDatenbank.sVersion, 
		  stInteraktion.sAbfrage, 4)) == 0)
		  {
			  iVersion();
			  continue;
		  }
          else if((strncmp(stDatenbank.sFormular, 
          stInteraktion.sAbfrage, 1)) == 0)
		  {
			  iFormular();
			  continue;
		  }
		  else if((strncmp(stDatenbank.sIncludes, 
		  stInteraktion.sAbfrage, 4)) == 0)
		  {
			  iHeader();
			  continue;
		  }
		  else if((strncmp(stDatenbank.sGenerieren, 
		  stInteraktion.sAbfrage, 4)) == 0)
		  {
			  iGenerieren();
			  continue;
		  }
		  else if((strncmp(stDatenbank.sBeenden, 
		  stInteraktion.sAbfrage, 4)) == 0)
		  {
			  iSetze_Pause(3);
			  /* Codegenerator sauber beenden */
			  break;
		  }
		  else
		  {
			  iHinweis();
			  continue;
		  }
		  /* Ende der Schleifenbedingung */
	  }while(stDatenbank.sBeenden != stInteraktion.sAbfrage);  
	  return EXIT_SUCCESS;
  }

/** Steuerprogramm                                                          
  */
  int iController()
  {
      iCodegen();
	  return EXIT_SUCCESS;
  }

/** Hauptprogramm
  */
  int main()
  {
	  iController();
	  return EXIT_SUCCESS;
  }

/** Programmende
  */
