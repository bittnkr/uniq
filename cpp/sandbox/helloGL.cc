#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
 
/* GLUT-like event callbacks */
void redraw(void);
void reshape(int x, int y);
void keypress(int key, int x, int y);
void mouse_button(int bn, int state, int x, int y);
void mouse_motion(int x, int y);
 
/* event handler, calls callbacks as needed */
void handle_event(XEvent *ev);
 
/* X helper functions */
Window create_window(int xsz, int ysz);
void set_window_title(Window win, const char *title);
 
void set_material(float r, float g, float b, float shin);
void draw_torus(float rad, float rrad, int usub, int vsub);
 
Display *dpy;
Window win;
GLXContext ctx;
int width, height;
int mapped;
int redisp_pending;
Atom xa_wm_prot, xa_wm_del_win;
 
int quit;
int torus;
float ldir[] = {-1, 1, 2, 0};
 
/* camera control */
float cam_theta, cam_phi = 25, cam_dist = 6;
int mbutton[8], prev_mx, prev_my;
 
int main(int argc, char **argv)
{
    if(!(dpy = XOpenDisplay(0))) {
        fprintf(stderr, "failed to open display\n");
        return 1;
    }
    xa_wm_prot = XInternAtom(dpy, "WM_PROTOCOLS", False);
    xa_wm_del_win = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
 
    if(!(win = create_window(800, 600))) {
        return 1;
    }
 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
 
    torus = glGenLists(1);
    glNewList(torus, GL_COMPILE);
    draw_torus(1.0, 0.3, 32, 16);
    glEndList();
 
    for(;;) {
        XEvent ev;
 
        do {
            XNextEvent(dpy, &ev);
            handle_event(&ev);
            if(quit) {
                goto done;
            }
        } while(XPending(dpy));
 
        if(redisp_pending) {
            redisp_pending = 0;
            redraw();
        }
    }
 
done:
    glDeleteLists(torus, 1);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return 0;
}
 
void redraw(void)
{
    glClearColor(0.1, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -cam_dist);
    glRotatef(cam_phi, 1, 0, 0);
    glRotatef(cam_theta, 0, 1, 0);
 
    glLightfv(GL_LIGHT0, GL_POSITION, ldir);
 
    set_material(0.5, 0.7, 0.5, 0);
 
    glPushMatrix();
    glTranslatef(0, -1.3, 0);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-5, 0, 5);
    glVertex3f(5, 0, 5);
    glVertex3f(5, 0, -5);
    glVertex3f(-5, 0, -5);
    glEnd();
    glPopMatrix();
 
    set_material(1.0, 0.2, 0.15, 60.0);
 
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glCallList(torus);
    glPopMatrix();
 
    glXSwapBuffers(dpy, win);
}
 
void reshape(int x, int y)
{
    glViewport(0, 0, x, y);
 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (float)x / (float)y, 0.5, 500.0);
}
 
void keypress(int key, int x, int y)
{
    switch(key) {
    case 27:
    case 'q':
    case 'Q':
        quit = 1;
        break;
    }
}
 
void mouse_button(int bn, int state, int x, int y)
{
    mbutton[bn] = state;
    prev_mx = x;
    prev_my = y;
}
 
void mouse_motion(int x, int y)
{
    int dx = x - prev_mx;
    int dy = y - prev_my;
    prev_mx = x;
    prev_my = y;
 
    if(!dx && !dy) return;
 
    if(mbutton[0]) {
        cam_theta += dx * 0.5;
        cam_phi += dy * 0.5;
        if(cam_phi < -90) cam_phi = -90;
        if(cam_phi > 90) cam_phi = 90;
        redisp_pending = 1;
    }
    if(mbutton[2]) {
        cam_dist += dy * 0.1;
        if(cam_dist < 0.0) cam_dist = 0.0;
        redisp_pending = 1;
    }
}
 
void handle_event(XEvent *ev)
{
    switch(ev->type) {
    case MapNotify:
    case UnmapNotify:
        mapped = ev->type == MapNotify ? 1 : 0;
        break;
 
    case Expose:
        redisp_pending = 1;
        break;
 
    case KeyPress:
        {
            KeySym sym = XLookupKeysym(&ev->xkey, 0);
            keypress(sym & 0xff, ev->xkey.x, ev->xkey.y);
        }
        break;
 
    case ConfigureNotify:
        if(ev->xconfigure.width != width || ev->xconfigure.height != height) {
            width = ev->xconfigure.width;
            height = ev->xconfigure.height;
            reshape(width, height);
        }
        break;
 
    case ClientMessage:
        if(ev->xclient.message_type == xa_wm_prot) {
            if(ev->xclient.data.l[0] == xa_wm_del_win) {
                quit = 1;
            }
        }
        break;
 
    case ButtonPress:
        mouse_button(ev->xbutton.button - Button1, 1, ev->xbutton.x, ev->xbutton.y);
        break;
 
    case ButtonRelease:
        mouse_button(ev->xbutton.button - Button1, 0, ev->xbutton.x, ev->xbutton.y);
        break;
 
    case MotionNotify:
        mouse_motion(ev->xmotion.x, ev->xmotion.y);
        break;
    }
}
 
Window create_window(int xsz, int ysz)
{
    Window w, root;
    XVisualInfo *vis;
    XClassHint chint;
    XSetWindowAttributes xattr;
    unsigned int evmask, xattr_mask;
    int scr;
    int glxattr[] = {
        GLX_RGBA, GLX_DOUBLEBUFFER,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_USE_GL, 1,
        None
    };
 
    scr = DefaultScreen(dpy);
    root = RootWindow(dpy, scr);
 
    if(!(vis = glXChooseVisual(dpy, scr, glxattr))) {
        printf("failed to find a suitable visual\n");
        return 0;
    }
    if(!(ctx = glXCreateContext(dpy, vis, 0, True))) {
        XFree(vis);
        return 0;
    }
 
    xattr.background_pixel = xattr.border_pixel = BlackPixel(dpy, scr);
    xattr.colormap = XCreateColormap(dpy, root, vis->visual, AllocNone);
    xattr_mask = CWColormap | CWBackPixel | CWBorderPixel;
 
    if(!(w = XCreateWindow(dpy, root, 0, 0, xsz, ysz, 0, vis->depth, InputOutput,
                    vis->visual, xattr_mask, &xattr))) {
        printf("failed to create window\n");
        glXDestroyContext(dpy, ctx);
        XFree(vis);
        return 0;
    }
    XFree(vis);
 
    evmask = StructureNotifyMask | VisibilityChangeMask | KeyPressMask |
        ExposureMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask;
    XSelectInput(dpy, w, evmask);
 
    XSetWMProtocols(dpy, w, &xa_wm_del_win, 1);
 
    chint.res_name = chint.res_class = "glx-example";
    XSetClassHint(dpy, w, &chint);
 
    set_window_title(w, "GLX example");
 
    glXMakeCurrent(dpy, w, ctx);
    XMapWindow(dpy, w);
    return w;
}
 
void set_window_title(Window win, const char *title)
{
    XTextProperty wm_name;
    XStringListToTextProperty((char**)&title, 1, &wm_name);
    XSetWMName(dpy, win, &wm_name);
    XSetWMIconName(dpy, win, &wm_name);
    XFree(wm_name.value);
}
 
void set_material(float r, float g, float b, float shin)
{
    float white[] = {1, 1, 1, 1}, black[] = {0, 0, 0, 1};
    float col[4];
 
    col[0] = r;
    col[1] = g;
    col[2] = b;
    col[3] = 1.0f;
 
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col);
    if(shin > 0.0) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shin);
    } else {
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
    }
}
 
static void torus_vertex(float rad, float rrad, float u, float v)
{
    float theta = u * M_PI * 2.0;
    float phi = v * M_PI * 2.0;
    float rx, ry, rz, x, y, z, cx, cy, cz;
 
    cx = sin(theta) * rad;
    cy = 0.0;
    cz = -cos(theta) * rad;
 
    rx = -cos(phi) * rrad + rad;
    ry = sin(phi) * rrad;
    rz = 0.0;
 
    x = rx * sin(theta) + rz * cos(theta);
    y = ry;
    z = -rx * cos(theta) + rz * sin(theta);
 
    glTexCoord2f(u, v);
    glNormal3f((x - cx) / rrad, (y - cy) / rrad, (z - cz) / rrad);
    glVertex3f(x, y, z);
}
 
void draw_torus(float rad, float rrad, int usub, int vsub)
{
    int i, j;
    float u, v;
    float du = 1.0 / (float)usub;
    float dv = 1.0 / (float)vsub;
 
    glBegin(GL_QUADS);
 
    u = 0.0;
    for(i=0; i<usub; i++) {
        v = 0.0;
        for(j=0; j<vsub; j++) {
            torus_vertex(rad, rrad, u, v);
            torus_vertex(rad, rrad, u + du, v);
            torus_vertex(rad, rrad, u + du, v + dv);
            torus_vertex(rad, rrad, u, v + dv);
 
            v += dv;
        }
        u += du;
    }
 
    glEnd();
}
// https://www.reddit.com/r/opengl/comments/6c63zc/minimal_code_to_get_a_triangle_on_screen_with/di9soyc?utm_source=share&utm_medium=web2x&context=3
