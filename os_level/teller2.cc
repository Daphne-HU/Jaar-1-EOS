#include <iostream>
#include <string>

char count(std::string line, char argument){
  int count = 0;
  for(unsigned int i=0 ; i < line.size() ; i++ ){
    if(line[i] == argument){
      count += 1;
    };
  };
  char result = char(count);
  return result;
}

int main(int argc, char *argv[]){
  std::string line;

  if(argc != 2){
    std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl>
    return -1;
  }

  while(std::getline(std::cin, line)){
    std::cout << count(line, argv[1]) << std::endl;
  }

  return 0;
}
