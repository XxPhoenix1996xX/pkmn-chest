#include "speciesConverter.hpp"

int SpeciesConverter::getG3Species(int g3index) {
	return SpeciesConverter::table3National[g3index];
}

int SpeciesConverter::setG3Species(int g4index) {
	return SpeciesConverter::table3Internal[g4index];
}
