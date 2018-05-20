#ifndef _WINDOW_H_
#define _WINDOW_H_

struct GLFWwindow;

class Window
{
public:
    Window(void) = default;
    Window(const char* title, int32_t width, int32_t height);

    void Init(void);
    void Update(void);
    const bool Open(void) const;
private:
    GLFWwindow* m_window;
    
    const char* m_title;
    int32_t m_width;
    int32_t m_height;
};

#endif /* _WINDOW_H_ */
