#ifndef GAME_DINO_UI_FIELD_MENU_NUMBER3_H
#define GAME_DINO_UI_FIELD_MENU_NUMBER3_H

#include "m/indicator.h"
#include "m/task.h"

// A number of up to 3 digits in text, or one of four marks.
class DinoUiFieldMenuNumber3 : public MProcess {
public:
    enum {
        VALUE_MANY = 0xFFFF,     // "+++"
        VALUE_MORE = 0xFFFE,     // "  +"
        VALUE_UNKNOWN = 0xFFFD,  // "???"
        VALUE_UNKNOWN2 = 0xFFFC, // " ??"
    };

    DinoUiFieldMenuNumber3(MPoint pos);
    virtual ~DinoUiFieldMenuNumber3();
    virtual void Idle();
    void SetValue(u32 value);
    void SetPriority(int priority);
    void Show();
    void Hide();

    MIndicator *mText; // 0x74
};

#endif
