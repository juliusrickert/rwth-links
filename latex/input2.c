// race.c ist nicht vor gemeinsamen Speicherzugriff geschützt.
// Siehe dazu no_race.c
// Diese Programm soll lediglich die Implementierung
// von  Shared Memory in C zeigen

#include <sys/types.h> /* shmget(), shmat(), shmdt(), shmctl() */
#include <sys/ipc.h>   /* shmget(), shmat(), shmdt(), shmctl() */
#include <sys/shm.h>   /* shmget(), shmat(), shmdt(), shmctl() */
#include <unistd.h>    /* fork() */
#include <sys/wait.h>  /* waitpid() */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */

#define MAXCOUNT 1000000
#define NUM_OF_CHILDS 4
#define SEGSIZE sizeof(int)

int main(void) {
  int i, id, *shar_mem;
  int pid[NUM_OF_CHILDS]; // PIDs der Kindprozesse

  // Anlegen eines Shared Memory Segments
  // Hier genau die Größe eines ints
  // Hinterlege Shared Memory Identifier
  id = shmget(IPC_PRIVATE, SEGSIZE, IPC_CREAT|0644);

  // Segment an den eigenen Adressraum des aufzurufenden Prozesses anhängen
  // Liefert Anfangsadresse des Speichers zurück
  shar_mem = (int *)shmat(id, 0, 0);

  // Initialisiere Segment mit 0
  *shar_mem = 0;

  // Erzeuge Kindprozesse
  for (size_t i = 0; i < NUM_OF_CHILDS; i++) {
    pid[i] = fork();

    if(pid[i] == -1) {
      printf("Kindprozess konnte nicht erzeugt werden!\n");
      exit(1);
    }

    if(pid[i] == 0) {
      // Diese Variable ist unabhängig von den anderen Prozessen
      int count = 0;
      while(*shar_mem < MAXCOUNT) {
        *shar_mem += 1;
        count++;
      }
      printf("%d Durchläufe wurden benötigt.\n", count);

      // Terminiere erflogreich Kindprozess
      exit(0);
    }
  }

  for (size_t i = 0; i < NUM_OF_CHILDS; i++) {
    waitpid(pid[i], NULL, 0);
  }

  printf("Ergebnis: %d\n", *shar_mem);

  // Segment aus dem Adressraum des aufzurufenden Prozess entfernen
  shmdt(shar_mem);

  // Freigeben des Shared Memory Segments und gleichzeitges löschen aller Daten
  shmctl(id, IPC_RMID, 0);

  return 0;
}
