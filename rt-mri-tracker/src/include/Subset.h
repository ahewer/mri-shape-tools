#ifndef __SUBSET_H__
#define __SUBSET_H__

#include <set>
#include <stdexcept>

#include <json/json.h>

class Subset{

public:

  static std::set<int>  read(const std::string& fileName) {
  
  
        std::set<int> subset;
  
        // try to open file
        std::ifstream inFile(fileName);
  
        if(inFile.is_open() == false) {
  
          throw std::runtime_error("Could not open subset file " + fileName + ".");
  
        }
  
        // parse JSON file
        Json::Value vertexList;
        inFile >> vertexList;
        inFile.close();
  
        // iterate through list
        for(const Json::Value& value: vertexList) {
  
          subset.insert( value.asInt() );
  
        } // end for vertexList
  
      return subset;
  
    }

private:

  // make constructor private -> this is a class
  // only offering a static method
  Subset();

};

#endif
