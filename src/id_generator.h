/*
 * id_generator.h
 *
 *  Created on: 15 mars 2016
 *      Author: mohammed
 */

#ifndef SRC_ID_GENERATOR_H_
#define SRC_ID_GENERATOR_H_
#include "identifiert.h"
namespace CT{

class IDGenerator {
public:
      static IDGenerator * instance ();
      CT::identifier_t next () { return _id++; }
private:
      IDGenerator () : _id(0) {}
      static IDGenerator * only_copy;
      CT::identifier_t _id;
};

#endif /* SRC_ID_GENERATOR_H_ */
}
