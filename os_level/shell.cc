#include "shell.hh"


void new_file(){ 
  std::string text;
  std::string regel;

  std::string bestandsnaam;
  std::cout << "bestandsnaam: " << std::endl;
  std::getline(std::cin, bestandsnaam);
  const char* bestand[] = {bestandsnaam.c_str()};

  int fd = syscall(SYS_creat, bestand[0], 0755);

  std::cout << "Tekst met <EOF> op het einde: " << std::endl;
  while(std::getline(std::cin, text)){
    if(text == "<EOF>"){
      break;
    }
    else{
      regel += text + "\n";
    }
  }
  const char* nieuwe_regel[] = {regel.c_str()};
  syscall(SYS_write, fd, nieuwe_regel[0], regel.size()); 
}





void list(){ 
  if (syscall(SYS_fork) == 0){
    const char* flags[] = {"/bin/ls", "-l", "-a", NULL};
    syscall(SYS_execve, flags[0], flags, NULL);
  }
  else{
    syscall(SYS_wait4, 0, NULL, NULL, NULL);
  }
}




void find(){
  std::cout << "Wat wilt u zoeken: " << std::endl;
  std::string input = "";
  std::getline(std::cin, input);
  int fd[2];
  syscall(SYS_pipe, &fd);
  if(syscall(SYS_fork) == 0){
    syscall(SYS_close, fd[0]);
    syscall(SYS_dup2, fd[1], 1);
    const char* flags[] = {"/usr/bin/find", ".", NULL};
    syscall(SYS_execve, flags[0], flags, NULL);
  }
  else{
    syscall(SYS_wait4, 0, NULL, NULL, NULL);
  }
  if(syscall(SYS_fork) == 0){
    syscall(SYS_close, fd[1]);
    syscall(SYS_dup2, fd[0], 0);
    const char* flags2[] = {"/bin/grep", input.c_str(), NULL};
    syscall(SYS_execve, flags2[0], flags2, NULL);
  }
  else{
    syscall(SYS_close, fd[1]);
    syscall(SYS_close, fd[0]);
    syscall(SYS_wait4, 0, NULL, NULL);
  }
}





void seek(){ 
  int seek = syscall(SYS_creat, "seek", 0755);
  int loop = syscall(SYS_creat, "loop", 0755);
  char byte1[1] = {'x'};
  char byte2[1] = {'\0'};
  int mil5 = 5000000;

  syscall(SYS_write, seek, byte1, 1);
  syscall(SYS_lseek, seek, mil5, 1);
  syscall(SYS_write, seek, byte1, 1);
  syscall(SYS_write, loop, byte1, 1);
    
  for(int i=0; i < mil5; i++){
    syscall(SYS_write, loop, byte2, 1);
  }
  syscall(SYS_write, loop, byte1, 1); 

}










void src(){ // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
  int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
    std::cout << byte; }                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.





int main(){
  std::string input;
  std::string prompt;
  int fd = syscall(SYS_open, "prompt.txt", O_RDONLY, 0755); // fd = file descriptor, 0755 = juiste rechten geven (leesbaar).
  char byte[1]; // variabele byte, grootte van 1 byte, datatype char.
  while(syscall(SYS_read, fd, byte, 1)){ // Blijft SYS_read loopen totdat het bestand compleet gelezen is.
    prompt.append(byte); // append de byte aan het prompt.txt bestand.
  }


  while(true){
    std::cout << prompt;                   // Print het prompt
    std::getline(std::cin, input);         // Lees een regel
    if (input == "new_file") new_file();   // Kies de functie
    else if (input == "ls") list();        //   op basis van
    else if (input == "src") src();        //   de invoer
    else if (input == "find") find();
    else if (input == "seek") seek();
    else if (input == "exit") return 0;
    else if (input == "quit") return 0;
    else if (input == "error") return 1;

    if (std::cin.eof()) return 0; // eof is een exit
  }
}
