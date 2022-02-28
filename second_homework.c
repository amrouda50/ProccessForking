#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <math.h>
#include <time.h>
typedef struct person
{
  char name[50];
  int yearofbirth;
  char number[50];
  bool decision;
  bool vaccined;
} person;
typedef struct names
{
  char *name[50];
  int size;
} names;
void handler(int signum);
char *get_name();
void add_person();
void modify_data(char *name);
void delete_data(char *name);
void addperson2(char *name, int yearofbirth, char *number, bool decision , bool vaccined);
void read_file();
int name_validation(char *a);
void vaccinate_people();
void shuffleNames(char *arr[50], int n);
void vaccine_him(char arr[]);
void reset();

int main(int argc, char **argv)
{

  int i;
  while (1)
  {
    printf("Press 1 to add a person data\n");
    printf("Press 2 to modify a person's data\n");
    printf("Press 3 to delete a person's data\n");
    printf("Press 4 to read the file's data\n");
    printf("Press 5 to vaccinate the people\n");
    printf("Press 6 to reset all the data\n");
    printf("Press 7 to exit\n");
    scanf("%d", &i);
    switch (i)
    {
    case 1:
      add_person();
      break;
    case 2:
      modify_data(get_name());
      break;
    case 3:
      delete_data(get_name());
      break;
    case 4:
      read_file();
      break;
    case 5:
      vaccinate_people();
      break;
    case 6:
      reset();
     break;
    case 7:
      exit(1);
      break;
    }
  }
}

void handler(int signum)
{
  printf("FIGHT UP\n");
}

void vaccinate_people()
{
  int file_output = open("personinformation.bin", O_RDONLY | O_CREAT, 0777);

  int pipefd[2];
  int pipefd_1[2];
  if (pipe(pipefd_1) == -1)
  {
    perror("Opening Error!\n");
    exit(0);
  }
  if (pipe(pipefd) == -1)
  {
    perror("Opening Error!\n");
    exit(0);
  }
  signal(SIGTERM, handler);
  pid_t child = fork();

  if (child == -1)
  {
    perror("Forking failed\n");
    exit(0);
  }

  else if (child > 0)
  {
    printf("Operation Committee waiting for the signal\n");
    pause();

    printf("Fight Up Signal recieved By Operating Committee\n");
    person *info = (person *)malloc(sizeof(person));
    printf("Listing names to be vaccinated(operating committie)\n");
    close(pipefd[0]);
    while (read(file_output, info, sizeof(person)))
    {
      if(info->vaccined == 0){
      printf("%s\n", info->name);
      write(pipefd[1], info, sizeof(person));
      }
    }
    close(pipefd[1]);

    close(pipefd_1[1]);
    sleep(3.5);
    printf("vaccined people(operating committie)\n");
    while (read(pipefd_1[0], info->name, sizeof(person)))
    {
      vaccine_him(info->name);
    }
    close(pipefd_1[0]);
    wait(NULL);
    printf("Operation Committee Process Ended\n");
  }
  else
  {

    printf("Buses sending FIGHT UP signal in 3 seconds\n");
    sleep(3);
    kill(getppid(), SIGTERM);
    sleep(2);
    person *info = (person *)malloc(sizeof(person));
    names *present_list = (names *)malloc(sizeof(names));
    printf("Listing names to be vaccinated(Buses)\n");
    close(pipefd[1]);
    present_list->size = 0;
    while (read(pipefd[0], info, sizeof(person)))
    {
      if(info->vaccined == 0){
      printf("%s\n", info->name);
      present_list->name[present_list->size] = malloc(strlen(info->name) + 1);
      strcpy(present_list->name[present_list->size], info->name);
      present_list->size++;
      }
    }
     close(pipefd[0]);
    if(present_list->size < 5){ 
      printf("Not Enough people for a bus to go out\n");
    }else if(present_list->size >= 5 && present_list->size < 10 ){
    srand(time(NULL));
    shuffleNames(present_list->name, present_list->size);
    present_list->size = trunc((float)5 * 0.90);
    close(pipefd_1[0]);
     printf("Bus 1 transporting people\n");
      for (int i = 0; i < 5 ; i++)
    {
     printf("%s\n" , present_list->name[i]);
    }
    for (int i = 0; i < present_list->size ; i++)
    {
      write(pipefd_1[1], present_list->name[i], sizeof(person));
    }
    printf("Buses 1 Process Ended\n");
    }
    else{
        int id = fork();
   
    srand(time(NULL));
    shuffleNames(present_list->name, present_list->size);
    present_list->size = trunc((float) 10 * 0.90);
    close(pipefd_1[0]);
     if(id == -1){
    perror("Forking Failed\n");
    error(0);
    }
    else if(id > 0){

     printf("Bus 1 transporting people\n");
      for (int i = 0; i < 5 ; i++)
    {
     printf("%s\n" , present_list->name[i]);
    }
    for (int i = 0; i < 5 ; i++)
    {
      write(pipefd_1[1], present_list->name[i], sizeof(person));
    }
    printf("Bus 1 Process Ended\n");
    }
    else
    {
     printf("Bus 2 transporting people\n");
      for (int i = 5; i < 10 ; i++)
    {
     printf("%s\n" , present_list->name[i]);
    }
    for (int i = 5 ; i < present_list->size  ; i++)
    {
      write(pipefd_1[1], present_list->name[i], sizeof(person));
    } 
    printf("Bus 2 Process Ended\n");
    }


    }
    close(pipefd_1[1]);
   


    exit(1);
  }
}

void add_person()
{
  int file_output = open("personinformation.bin", O_RDWR | O_CREAT, 0777);
  person *info = (person *)malloc(sizeof(person));
  fflush(stdin);
  char decision[5];

  while (read(file_output, info, sizeof(person)))
  {
  }
  info->vaccined = 0;
  printf("Enter the person name:\n");
  scanf("\n");
  scanf("%[^\n]s", info->name);
  if (name_validation(info->name) == 0)
  {
    perror("Error it is not a proper name\n");
    exit(1);
  }
  else if (info->name)
    printf("Enter the yearofbirth:\n");
  scanf("%d", &info->yearofbirth);
  if (yearofbirth_validation(info->yearofbirth) == 0)
  {
    perror("Wrong Age Entered\n");
    exit(1);
  }
  printf("Enter the person Phone number;\n");
  scanf("%s", info->number);
  if (number_validation(info->number) == 0)
  {
    perror("Wrong input\n");
    exit(1);
  }
  printf("Free vacination?\n");
  printf("Please answer yes or no:\n");
  scanf("%s", decision);
  if (sizeof(decision) / sizeof(char) > 4)
  {
    if (strcmp(decision, "yes") == 0)
    {
      info->decision = 1;
    }
    else if (strcmp(decision, "no") == 0)
    {
      info->decision = 0;
    }

    else
    {
      perror("unkown decision given\n");
      exit(1);
    }
  }
  else
  {
    perror("invalid decision\n");
    exit(1);
  }
  if (write(file_output, info, sizeof(person)) != sizeof(person))
  {
    perror("Something went wrong while writing\n");
    exit(0);
  }
  free(info);
  close(file_output);
}

void delete_data(char *name)
{
  int file_output = open("personinformation.bin", O_RDWR | O_CREAT, 0777);
  int helper_file = open("helper.bin", O_RDWR | O_APPEND | O_CREAT, 0777);
  person *deleted = (person *)malloc(sizeof(person));
  while (read(file_output, deleted, sizeof(person)))
  {
    if (strcmp(deleted->name, name) != 0)
    {
      write(helper_file, deleted, sizeof(person));
    }
  }
  close(file_output);
  close(helper_file);
  remove("personinformation.bin");
  rename("helper.bin", "personinformation.bin");
}

void modify_data(char *name)
{
  person *new = (person *)malloc(sizeof(person));
  int file_output = open("personinformation.bin", O_RDONLY | O_CREAT, 0777);
  int i = 0;
  printf("what would you like to modify?\n");
  printf("1.Name\n");
  printf("2.DateOfBirth\n");
  printf("3.PhoneNumber\n");
  printf("4.vaccinationStatus\n");
  scanf("%d", &i);
  switch (i)
  {
  case 1:
  {
    char *new_name = get_name();
    if (name_validation(new_name) == 0)
    {
      perror("Wrong name format entered\n");
      exit(1);
    }
    else
    {
      while (read(file_output, new, sizeof(person)))
      {
        if (strcmp(new->name, name) == 0)
        {
          strcpy(new->name, new_name);
          break;
        }
      }
      delete_data(name);
      addperson2(new->name, new->yearofbirth, new->number, new->decision , 0);
      break;
    }
  }
  case 2:
  {
    printf("Please Write the new date of birth\n");
    int newdate;
    scanf("%d", &newdate);
    while (read(file_output, new, sizeof(person)))
    {
      if (strcmp(new->name, name) == 0)
      {
        new->yearofbirth = newdate;
        break;
      }
    }
    printf("%s \n", new->name);
    printf("%d \n", new->yearofbirth);
    printf("%s \n", new->number);

    delete_data(name);
    addperson2(new->name, new->yearofbirth, new->number, new->decision , 0);
    break;
  }

  case 3:
  {
    printf("Please Write the new PhoneNumber\n");
    char *new_number = (char *)malloc(12);
    scanf("\n");
    scanf("%[^\n]s", new_number);
    while (read(file_output, new, sizeof(person)))
    {
      if (strcmp(new->name, name) == 0)
      {
        strcpy(new->number, new_number);
        break;
      }
    }
    delete_data(name);
    addperson2(new->name, new->yearofbirth, new->number, new->decision , 0);
    break;
  }

  case 4:
  {
    char decision[5];
    printf("Please Enter the new vaccinationStatus\n");
    scanf("%s", decision);
    while (read(file_output, new, sizeof(person)))
    {
      if (strcmp(new->name, name) == 0)
      {
        if (strcmp(decision, "yes") == 0)
        {
          new->decision = 1;
        }
        else if (strcmp(decision, "no") == 0)
        {
          new->decision = 0;
        }
        break;
      }
    }
    delete_data(name);
    addperson2(new->name, new->yearofbirth, new->number, new->decision , 0);
    break;
  }
  }
}

char *get_name()
{
  char *name = (char *)malloc(50);
  printf("Enter the person's name\n");
  scanf("\n");
  scanf("%[^\n]s", name);
  return name;
}

void addperson2(char *name, int yearofbirth, char *number, bool decision , bool vaccined)
{
  int file_output = open("personinformation.bin", O_RDWR | O_CREAT, 0777);
  person *info = (person *)malloc(sizeof(person));
  while (read(file_output, info, sizeof(person)))
  {
  }

  strcpy(info->name, name);
  info->yearofbirth = yearofbirth;
  strcpy(info->number, number);
  info->decision = decision;
  info->vaccined = vaccined;
  if (write(file_output, info, sizeof(person)) != sizeof(person))
  {
    perror("Something went wrong while writing\n");
    exit(0);
  }
  free(info);
  close(file_output);
}

void read_file()
{
  int file = open("personinformation.bin", O_RDONLY);
  if (file == -1)
  {
    printf("File not found(you need to add data)\n");
  }
  else
  {
    person *output = (person *)malloc(sizeof(person));
    while (read(file, output, sizeof(person)))
    {
      printf("%s,", output->name);
      printf("%d,", output->yearofbirth);
      printf("%s,", output->number);
      if (output->decision == 1)
      {
        printf("yes,");
      }
      else
      {
        printf("no,");
      }
      if (output->vaccined == 1)
      {
        printf("yes\n");
      }
      else
      {
        printf("no\n");
      }
    }
  }

  close(file);
}

int name_validation(char *a)
{
  int bool_ = 1;

  for (int i = 0; a[i] != '\0'; i++)
  {
    if (((int)a[i] >= 97 && (int)a[i] <= 122 || (int)a[i] == 32 || (int)a[i] >= 65 && (int)a[i] <= 90))
    {
    }
    else
    {
      bool_ = 0;
    }
  }
  return bool_;
}

int number_validation(char *a)
{
  int bool_ = 1;
  if (atoi(a) > 99999999999)
  {
    bool_ = 0;
  }

  for (int i = 0; a[i] != '\0'; i++)
  {
    if (((int)a[i] >= 48 && (int)a[i] <= 57))
    {
    }
    else
    {
      bool_ = 0;
    }
  }
  return bool_;
}
void shuffleNames(char *arr[50], int n)
{
  int size = n;
  if (size > 1)
  {
    int i;
    for (i = 0; i < size - 1; i++)
    {
      int j = rand() % n;
      char *temp = arr[j];
      arr[j] = arr[i];
      arr[i] = temp;
    }
  }
}

int yearofbirth_validation(int a)
{
  int bool_ = 1;
  if (a < 1932 || a > 2021)
  {
    bool_ = 0;
  }
  return bool_;
}

void vaccine_him(char arr[]){
int file_output = open("personinformation.bin", O_RDWR , 0777);
bool t = 0;
person *new = (person *)malloc( sizeof(person));
while(read(file_output , new , sizeof(person))){
  
if(strcmp(arr , new->name) == 0){
  printf("%s\n" , arr , new->name );
  new->vaccined = 1;
  t = 1;
  break;
}
}
if(t == 1){
delete_data(arr);
addperson2(new->name, new->yearofbirth, new->number, new->decision , new->vaccined);
close(file_output);
free(new);
}

}
 void reset(){
if(system("rm personinformation.bin")){
printf("File Does not exist\n");
}
else{ 
  printf("File Successfully deleted\n");
}
 }