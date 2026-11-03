
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 5

#define MUTEX 0
#define EMPTY 1
#define FULL 2

#define KEY_PATH "producer.c"
#define PROJ_ID 65

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

struct SharedData {
  int buffer[SIZE];
  int in;
  int out;
};

void wait_sem(int semid, int semnum) {
  struct sembuf sb;
  sb.sem_num = semnum;
  sb.sem_op = -1;
  sb.sem_flg = 0;
  semop(semid, &sb, 1);
}

void signal_sem(int semid, int semnum) {
  struct sembuf sb;
  sb.sem_num = semnum;
  sb.sem_op = 1;
  sb.sem_flg = 0;
  semop(semid, &sb, 1);
}

int main() {
  int shmid;
  int semid;
  int i;
  key_t key;
  struct SharedData *data;
  union semun u;

  key = ftok(KEY_PATH, PROJ_ID);
  if (key == -1) {
    perror("ftok");
    return 1;
  }

  shmid = shmget(key, sizeof(struct SharedData), IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("shmget");
    return 1;
  }

  data = (struct SharedData *)shmat(shmid, NULL, 0);
  if (data == (struct SharedData *)-1) {
    perror("shmat");
    return 1;
  }

  data->in = 0;
  data->out = 0;

  semid = semget(key, 3, IPC_CREAT | 0666);
  if (semid == -1) {
    perror("semget");
    return 1;
  }

  u.val = 1;
  semctl(semid, MUTEX, SETVAL, u);

  u.val = SIZE;
  semctl(semid, EMPTY, SETVAL, u);

  u.val = 0;
  semctl(semid, FULL, SETVAL, u);

  for (i = 0; i < 10; i++) {
    int num = i + 1;

    wait_sem(semid, EMPTY);
    wait_sem(semid, MUTEX);

    data->buffer[data->in] = num;
    printf("Producer produced: %d\n", num);
    data->in = (data->in + 1) % SIZE;

    signal_sem(semid, MUTEX);
    signal_sem(semid, FULL);
    
    sleep(1);
  }

  shmdt(data);
  return 0;
}




#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#define SIZE 5

#define MUTEX 0
#define EMPTY 1
#define FULL 2

#define KEY_PATH "producer.c"
#define PROJ_ID 65

struct SharedData {
  int buffer[SIZE];
  int in;
  int out;
};

void wait_sem(int semid, int semnum) {
  struct sembuf sb;
  sb.sem_num = semnum;
  sb.sem_op = -1;
  sb.sem_flg = 0;
  semop(semid, &sb, 1);
}

void signal_sem(int semid, int semnum) {
  struct sembuf sb;
  sb.sem_num = semnum;
  sb.sem_op = 1;
  sb.sem_flg = 0;
  semop(semid, &sb, 1);
}

int main() {
  int shmid;
  int semid;
  int i;
  key_t key;
  struct SharedData *data;

  key = ftok(KEY_PATH, PROJ_ID);
  if (key == -1) {
    perror("ftok");
    return 1;
  }

  shmid = shmget(key, sizeof(struct SharedData), 0666);
  if (shmid == -1) {
    perror("shmget");
    return 1;
  }

  data = (struct SharedData *)shmat(shmid, NULL, 0);
  if (data == (struct SharedData *)-1) {
    perror("shmat");
    return 1;
  }

  semid = semget(key, 3, 0666);
  if (semid == -1) {
    perror("semget");
    return 1;
  }

  for (i = 0; i < 10; i++) {
    int num;

    wait_sem(semid, FULL);
    wait_sem(semid, MUTEX);

    num = data->buffer[data->out];
    data->out = (data->out + 1) % SIZE;

    signal_sem(semid, MUTEX);
    signal_sem(semid, EMPTY);

    printf("Consumer read: %d\tSquare: %d\n", num, num * num);
  }

  shmdt(data);
  shmctl(shmid, IPC_RMID, NULL);
  semctl(semid, 0, IPC_RMID);

  return 0;
}
