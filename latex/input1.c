// no_race.c ist vor gemeinsamen Speicherzugriff geschützt.
// Siehe Gegenbeispiel race.c
// Implementierung von  Shared Memory in C

#include <sys/types.h> /* shmget(), shmat(), shmdt(), shmctl() */
#include <sys/ipc.h>   /* shmget(), shmat(), shmdt(), shmctl() */
#include <sys/shm.h>   /* shmget(), shmat(), shmdt(), shmctl() */
#include <unistd.h>    /* fork() */
#include <sys/wait.h>  /* waitpid() */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */

#include <pthread.h>   /* (NEU HINZUGEFÜGT) -> Mutexte */
#include <errno.h>     /* Codes von Systemfehlern */

#define MAXCOUNT 1000000
#define NUM_OF_CHILDS 4
#define SEGSIZE sizeof(int)

int main(void) {
  int i, id, *shar_mem;
  int pid[NUM_OF_CHILDS]; // PIDs der Kindprozesse
  pthread_mutex_t mutex; // Mutex kann als lokale Variable implementiert werden

  // Initialisieren des Mutex mit Default Attributen
  if(pthread_mutex_init(&mutex, NULL)) {
    printf("pthread_mutex_init fehlgeschlagen mit error code: %d\n", errno);
    return errno;
  }

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

      while(1) {
        pthread_mutex_lock(&mutex); // Sperre kritischen Bereich
        if(*shar_mem < MAXCOUNT) {
          *shar_mem += 1;
          pthread_mutex_unlock(&mutex); // Gib kritischen Bereich frei
          count++;
        } else {
          break;
        }
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

  // Mutex löschen
  pthread_mutex_destroy(&mutex);

  // Segment aus dem Adressraum des aufzurufenden Prozess entfernen
  shmdt(shar_mem);

  // Freigeben des Shared Memory Segments und gleichzeitges löschen aller Daten
  shmctl(id, IPC_RMID, 0);

  return 0;
}
