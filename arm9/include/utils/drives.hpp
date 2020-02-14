#ifndef DRIVES_HPP
#define DRIVES_HPP

#include <nds.h>

bool sdFound(void);
bool flashcardFound(void);
bool bothSDandFlashcard(void);

TWL_CODE bool nandMount(void);

#endif //FLASHCARD_HPP
