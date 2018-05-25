#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "event_handler.h"

struct GLFWwindow;

class Window
{
public:
    Window(void) = default;
    Window(const char* title, int32_t width, int32_t height);

    void Init(void);
    void Update(void);
    const bool Open(void) const;

	void MBCallback(int32_t button);
    void EForwarder(const mulgame::MULGEventForwarder& ef);
private:
    void PollKeys(void);
    void PollMouse(void);
private:
    GLFWwindow* m_window;
    
    const char* m_title;
    int32_t m_width;
    int32_t m_height;

    mulgame::MULGEventForwarder m_eventForwarder;
};

#endif /* _WINDOW_H_ */
