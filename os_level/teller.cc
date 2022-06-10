#include <iostream>
#include <string>

int count(std::string line, char *argument){
  int count = 0;
  for(unsigned int i=0 ; i < line.size() ; i++ ){
    if(line[i] == *argument){
      count += 1;
    };
  };
  return count;
};

int main(int argc, char *argv[]){
  std::string line;

  if(argc != 2){
    std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl;
    return -1;
  };

  while(std::getline(std::cin, line)){
    int i = count(line, argv[1]);
    std::cout << i << std::endl;
  };

  return 0;
};
