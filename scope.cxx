#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Pack.H>
#include <FL/fl_draw.H>

#include <cmath>

const int trace_max = 1024;
const int view_width = 512;
static int trace_pos = 0;
static int view_pos = 0;
static int view_break = 0;
static float trace[trace_max];
static float view[view_width];

class scope_view: public Fl_Box
{
    public:
        scope_view(int x, int y,int w,int h, const char *l=0)
            :Fl_Box(x,y,w,h,l) {}
    private:
        void draw();
};

void scope_view::draw()
{
    int wd = w();
    int ht = h();
    int xo = x();
    int yo = y();

    fl_color(FL_BLACK);
    fl_rectf(xo, yo, wd, ht);
    fl_color(FL_GREEN);

    fl_push_matrix();
    fl_translate(xo, (yo + (ht/2)));
    fl_scale(wd, ht/2);

    fl_begin_line();
    for (int i = 0; i < view_width; i++) {
        if(i == view_break) {
            fl_end_line();
            fl_begin_line();
        }
        fl_vertex(((float)i/(float)view_width), view[i]);
    }
    fl_end_line();

    fl_pop_matrix();
} /* end of draw() method */

static scope_view *scope = 0;

void update_scope(void *)
{
    for (int i = 0; i < 7; i++) {
        view[view_pos] = trace[trace_pos];
        trace_pos++;
        if(trace_pos >= trace_max)
            trace_pos = 0;

        view_pos++;

        if(view_pos >= view_width)
            view_pos = 0;
    }
    view_break = view_pos;
    Fl::repeat_timeout(0.5, update_scope);
    scope->redraw();
    printf("/get/data p\n");
}

int main(int argc, char **argv)
{
    Fl_Double_Window *main_win = new Fl_Double_Window(522, 210,
            "Scope Window");
    main_win->begin();
    //Fl_Pack *pack = new Fl_Pack(0,0,522,210,"label");
    //pack->begin();
    scope = new scope_view(5, 5, 512, 200);
    //pack->end();
    main_win->end();
    main_win->resizable(scope);

    // now create the trace - a sine wave
    for (int i = 0; i < trace_max; i++) {
        trace[i] = -sin((double)(i*5*2*M_PI/(double)trace_max));
    }
    for (int i = 0; i < view_width; i++) {
        view[i] = trace[i];
    }
    trace_pos = view_width;
    main_win->show(argc, argv);

    Fl::add_timeout(0.5, update_scope);

    return Fl::run();
}
