#ifndef THEME_H
#define THEME_H

#include <wx/wx.h>

struct Theme
{
    static wxColour Primary() { return wxColour(0, 123, 255); }      // bleu principal
    static wxColour PrimaryAlt() { return wxColour(0, 102, 204); }   // bleu alternatif
    static wxColour Accent() { return wxColour(255, 140, 0); }       // orange
    static wxColour Success() { return wxColour(40, 167, 69); }     // vert
    static wxColour Danger() { return wxColour(220, 53, 69); }      // rouge
    static wxColour Warning() { return wxColour(255, 193, 7); }     // jaune
    static wxColour Background() { return wxColour(245, 245, 250); } // fond clair
    static wxColour Muted() { return wxColour(100, 100, 100); }     // texte secondaire
    static wxColour Neutral() { return wxColour(108, 117, 125); }   // gris neutre
};

#endif // THEME_H
