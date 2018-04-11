#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Nice_Slider.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Chart.H>

template<class T>
const T& min(const T&a, const T&b)
{
        return a<b?a:b;
}

template<class T>
const T& max(const T&a, const T&b)
{
        return a>b?a:b;
}

class Fl_MinWidth : public Fl_Box
{
    public:
        Fl_MinWidth(int x, int y, int w, int h);
        void resize(int x, int y, int w, int h);
    private:
        int wmin;
};

Fl_MinWidth::Fl_MinWidth(int x, int y, int w, int h)
    :Fl_Box(x,y,w,h), wmin(w)
{
}

void Fl_MinWidth::resize(int x, int y, int w, int h)
{
    //printf("%s@%p: %d %d %d %d\n", __PRETTY_FUNCTION__ ,this,x,y,w,h);
    Fl_Box::resize(x,y,max(w,wmin),h);
}

class MainWindow:public Fl_Double_Window
{
    public:
        MainWindow(int w, int h, const char *t);
        virtual ~MainWindow(void);
        virtual void resize(int x, int y, int w, int h);
    private:
        int wmin;
};

void MainWindow::resize(int x, int y, int w, int h)
{
    printf("%s@%p: %d %d %d %d\n", __PRETTY_FUNCTION__ ,this,x,y,w,h);
    printf("%d\n", max(w,wmin));
    Fl_Double_Window::resize(x,y,max(w,wmin),h);
}
//void MainWindow::draw(void)
//{
//    //resize(x(),y(),w(),h());
//    Fl_Double_Window::draw();
//}
//
class FormantUI:public Fl_Group
{
    public:
        FormantUI(int x, int y, int w, int h);
        void resize(int x, int y, int w, int h);
};

class FormantGroup:public Fl_Group
{
    public:
        FormantGroup(int x, int y, int w, int h);
        void resize(int x, int y, int w, int h);
    private:
        int wmin;
};

FormantUI::FormantUI(int x, int y, int w, int h)
    :Fl_Group(x,y,w,h)
{
    const int PAD = 20;
    //dial locations
    const int dial_size   = min(w-PAD,h/3-PAD);
    const int dial_offset = x+PAD/2+(w-h/3)/2;

    //Fl_Group *localbox = new Fl_Group(X,Y,w/4-PPAD,h);
    Fl_Dial *dial      = new Fl_Dial(dial_offset,y+PAD,
            dial_size,dial_size,"Bw");
    Fl_Slider *slider  = new Fl_Slider(x+PAD,y+PAD+h/3,
            w-2*PAD,h*2/3-2*PAD,"Fc");

    //localbox->box(FL_GTK_UP_BOX);
    box(FL_GTK_DOWN_BOX);
    dial->box(FL_GTK_ROUND_UP_BOX);
    //localbox->end();
    resizable(0);
    //localbox->resizable(0);
    end();
}

void FormantUI::resize(int x, int y, int w, int h)
{
    //printf("%s@%p: %d %d %d %d\n", __PRETTY_FUNCTION__ ,this,x,y,w,h);
    Fl_Group::resize(x,y,w,h);
}

void FormantGroup::resize(int x, int y, int w, int h)
{
    //printf("%s@%p: %d %d %d %d\n", __PRETTY_FUNCTION__ ,this,x,y,w,h);
    Fl_Group::resize(x,y,max(w,wmin),h);
}

FormantGroup::FormantGroup(int x, int y, int w, int h)
    :Fl_Group(x,y,w,h), wmin(w)
{
    type(1);
    Fl_Group *formant[4];
    const int PPAD = 1;

    formant[0] = new Fl_Group(x,y,w/4,h);
    new FormantUI(x,y,w/4-1,h);
    formant[0]->resizable(new Fl_Box(x+w/4-1,y,1,h));
    formant[0]->end();

    formant[1] = new Fl_Group(x+w/4,y,w/4,h);
    new FormantUI(x+w/4,y,w/4-1,h);
    formant[1]->resizable(new Fl_Box(x+w*2/4-1,y,1,h));
    formant[1]->end();

    formant[2] = new Fl_Group(x+w*2/4,y,w/4,h);
    new FormantUI(x+w*2/4,y,w/4-1,h);
    formant[2]->resizable(new Fl_Box(x+w*3/4-1,y,1,h));
    formant[2]->end();

    formant[3] = new FormantUI(x+3*w/4,y,w/4,h);
    formant[3]->end();
//
//    formant[0] = new Fl_Group(x,y,w,h);
//    new FormantUI(x+3*w/4,y,w/4,h);
//    formant[1] = new Fl_Group(x,y,w*3/4,h);
//    formant[0]->resizable(formant[1]);
//    new FormantUI(x,y,w/4,h);
//    formant[2] = new Fl_Group(x+w/4,y,w/2,h);
//    formant[1]->resizable(new Fl_Box(x+w/4,y,w/2,h));
//    formant[2]->resizable(new Fl_Box(x+w/2,y,0,h));
//    new FormantUI(x+w/4,y,w/4-1,h);
//    new FormantUI(x+w/2,y,w/4-1,h);
    resizable(new Fl_Box(x,y,w*3/4,h));
    end();
}

MainWindow::MainWindow(int w, int h, const char *t)
:Fl_Double_Window(w,h,t),wmin(w)
{
    Fl_Chart     *chart    = new Fl_Chart(0,0,w,h/2);
    FormantGroup *formants = new FormantGroup(0,h/2,w,h/2);
    resizable(new Fl_MinWidth(0,0,w,h/2));
    size_range(w,h);
}

MainWindow::~MainWindow(void)
{}

int main(int argc, char **argv)
{
    Fl_Window *window = new MainWindow(300,300,"test_window");

    window->show(1, argv);

    Fl::scheme("gtk+");
    return Fl::run();
}
