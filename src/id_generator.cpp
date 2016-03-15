/*
 * id_generator.cpp
 *
 *  Created on: 15 mars 2016
 *      Author: mohammed
 */

#include "id_generator.h"
namespace CT{
IDGenerator * IDGenerator::instance () {
   if (!only_copy) {
      only_copy = new IDGenerator();
   }
   return only_copy;
}

}
