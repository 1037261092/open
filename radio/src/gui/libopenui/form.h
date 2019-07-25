/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _FORM_H_
#define _FORM_H_

#include "window.h"

class FormField: public Window {
  friend class FormWindow;

  public:
    FormField(Window * parent, const rect_t & rect, uint8_t flags=0);

    inline void setNextField(FormField *field)
    {
      next = field;
    }

    inline void setPreviousField(FormField * field)
    {
      previous = field;
    }

    inline static void setCurrentField(FormField * field)
    {
      current = field;
    }

    inline static void clearCurrentField()
    {
      setCurrentField(nullptr);
    };

    inline static FormField * getCurrentField()
    {
      return current;
    };

    inline FormField * getPreviousField()
    {
      return previous;
    };

    inline FormField * getNextField()
    {
      return next;
    };

    inline static void link(FormField * previous, FormField * next)
    {
      previous->setNextField(next);
      next->setPreviousField(previous);
    }

    void paint(BitmapBuffer * dc) override;

#if defined(HARDWARE_KEYS)
    void onKeyEvent(event_t event) override;
#endif

  protected:
    static FormField * current;
    FormField * next = nullptr;
    FormField * previous = nullptr;
    bool editMode = false;
};

class FormGroup: public FormField {
  public:
    FormGroup(Window * parent, const rect_t & rect, uint8_t flags=0) :
      FormField(parent, rect, flags)
    {
      FormField::current = nullptr;
    }

#if defined(DEBUG_WINDOWS)
    std::string getName() override
    {
      return "FormGroup";
    }
#endif

    void clear()
    {
      Window::clear();
      first = nullptr;
      FormField::current = nullptr;
    }

    inline void setFirstField(FormField * field)
    {
      first = field;
    }

    inline void setLastField(FormField * field = nullptr)
    {
      if (!field)
        field = getCurrentField();
      FormField::link(field, first);
      // FormField::setCurrentField(this);
    }

    FormField * getFirstField()
    {
      return first;
    }

  protected:
    FormField * first = nullptr;

#if defined(HARDWARE_KEYS)
    void onKeyEvent(event_t event) override;
#endif

    void paint(BitmapBuffer * dc) override;
};

class FormWindow: public Window {
  public:
    FormWindow(Window * parent, const rect_t & rect) :
      Window(parent, rect)
    {
      FormField::current = nullptr;
    }

#if defined(DEBUG_WINDOWS)
    std::string getName() override
    {
      return "FormWindow";
    }
#endif

    void clear()
    {
      first = nullptr;
      Window::clear();
    }

    FormField * getFirstField()
    {
      return first;
    }

    inline void setFirstField(FormField * field)
    {
      first = field;
      field->setFocus();
    }

    inline void setLastField(FormField * field = nullptr)
    {
      if (!field)
        field = FormField::getCurrentField();
      FormField::link(field, first);
    }

  protected:

#if defined(HARDWARE_KEYS)
    void onKeyEvent(event_t event) override;
#endif

    void paint(BitmapBuffer * dc) override
    {
    }

  protected:
    FormField * first = nullptr;
};

#endif
