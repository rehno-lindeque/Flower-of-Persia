#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

extern class Keyboard
{
public:
  bool keys[255];

  inline void clear() { memset(keys, 0, sizeof(keys)); }
  inline Keyboard() { clear(); }
} keyboard;

#endif
